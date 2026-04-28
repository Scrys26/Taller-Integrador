/**
 * ============================================================
 *  iGate APRS-IS — Firmware Propio
 *  Máquina de Estados: START → LISTEN → DECODE → UPLINK → LISTEN
 *  Plataforma: ESP32 + LoRa SX1278 (o SX1276)
 * ============================================================
 *  Estados:
 *    START      — Inicialización de hardware y WiFi
 *    LISTEN     — Espera de paquetes LoRa
 *    DECODE     — Decodificación y validación de trama
 *    UPLINK     — Envío al servidor APRS-IS
 *    BEACON_TX  — Transmisión periódica de beacon
 *    ERROR      — Manejo de fallos y recuperación
 * ============================================================
 */

#include <Arduino.h>
#include "config.h"
#include "wifi_manager.h"
#include "lora_manager.h"
#include "aprs_parser.h"
#include "aprs_is_client.h"
#include "beacon.h"
#include "display.h"
#include "state_machine.h"

// ──────────────────────────────────────────────────────────────
//  Variables globales de estado
// ──────────────────────────────────────────────────────────────
SystemState currentState = STATE_START;
SystemState previousState = STATE_START;

String    lastLoRaPacket   = "";
bool      packetAvailable  = false;
bool      packetValid      = false;
uint32_t  beaconTimer      = 0;
uint8_t   errorRetryCount  = 0;

// ──────────────────────────────────────────────────────────────
//  Prototipos
// ──────────────────────────────────────────────────────────────
void handleStateStart();
void handleStateListen();
void handleStateDecode();
void handleStateUplink();
void handleStateBeaconTx();
void handleStateError();
void transitionTo(SystemState newState);

// ──────────────────────────────────────────────────────────────
//  setup()
// ──────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    Serial.println(F("\n=== iGate APRS-IS — Firmware Propio ==="));

    Display::init();
    Display::showBoot();

    currentState = STATE_START;
}

// ──────────────────────────────────────────────────────────────
//  loop() — Motor principal de la máquina de estados
// ──────────────────────────────────────────────────────────────
void loop() {
    switch (currentState) {
        case STATE_START:     handleStateStart();    break;
        case STATE_LISTEN:    handleStateListen();   break;
        case STATE_DECODE:    handleStateDecode();   break;
        case STATE_UPLINK:    handleStateUplink();   break;
        case STATE_BEACON_TX: handleStateBeaconTx(); break;
        case STATE_ERROR:     handleStateError();    break;
        default:
            Serial.println(F("[FSM] Estado desconocido — reiniciando"));
            transitionTo(STATE_ERROR);
    }
}

// ──────────────────────────────────────────────────────────────
//  Transición entre estados con log
// ──────────────────────────────────────────────────────────────
void transitionTo(SystemState newState) {
    previousState = currentState;
    currentState  = newState;
    Serial.printf("[FSM] %s → %s\n",
                  stateName(previousState),
                  stateName(newState));
    Display::showState(newState);
}

// ══════════════════════════════════════════════════════════════
//  ESTADO: START
//  Inicializa WiFi y LoRa. Si cualquiera falla → ERROR.
// ══════════════════════════════════════════════════════════════
void handleStateStart() {
    Serial.println(F("[START] Iniciando subsistemas..."));

    // — Inicializar LoRa —
    if (!LoRaManager::init()) {
        Serial.println(F("[START] ERROR: fallo al iniciar LoRa"));
        transitionTo(STATE_ERROR);
        return;
    }
    Serial.println(F("[START] LoRa OK"));

    // — Conectar WiFi —
    if (!WiFiManager::connect(WIFI_SSID, WIFI_PASSWORD, WIFI_TIMEOUT_MS)) {
        Serial.println(F("[START] ERROR: no se pudo conectar al WiFi"));
        transitionTo(STATE_ERROR);
        return;
    }
    Serial.printf("[START] WiFi OK — IP: %s\n", WiFiManager::localIP().c_str());

    // — Conectar APRS-IS —
    if (!APRSISClient::connect(APRS_SERVER, APRS_PORT,
                               CALLSIGN, APRS_PASSCODE, APRS_FILTER)) {
        Serial.println(F("[START] WARN: no se pudo conectar al servidor APRS-IS"));
        // No es fatal en el arranque; se reintentará en UPLINK
    }

    errorRetryCount = 0;
    beaconTimer     = millis();

    transitionTo(STATE_LISTEN);   // ← evento: init
}

// ══════════════════════════════════════════════════════════════
//  ESTADO: LISTEN
//  Escucha el canal LoRa. Eventos: lora_int, timerbeacon, error
// ══════════════════════════════════════════════════════════════
void handleStateListen() {
    // — Comprobar beacon periódico (timerbeacon) —
    if (millis() - beaconTimer >= BEACON_INTERVAL_MS) {
        transitionTo(STATE_BEACON_TX);
        return;
    }

    // — Verificar conexión WiFi —
    if (!WiFiManager::isConnected()) {
        Serial.println(F("[LISTEN] WiFi perdido → ERROR"));
        transitionTo(STATE_ERROR);
        return;
    }

    // — Escuchar LoRa (lora_int) —
    String packet;
    int    rssi;
    float  snr;

    if (LoRaManager::receive(packet, rssi, snr)) {
        lastLoRaPacket  = packet;
        packetAvailable = true;
        Serial.printf("[LISTEN] Paquete recibido (%d bytes) RSSI=%d SNR=%.1f\n",
                      packet.length(), rssi, snr);
        transitionTo(STATE_DECODE);   // ← evento: lora_int
    }

    // Pequeño yield para no bloquear el watchdog
    delay(10);
}

// ══════════════════════════════════════════════════════════════
//  ESTADO: DECODE
//  Valida y parsea la trama LoRa-APRS.
//  Eventos: valid_lora → UPLINK | invalid_lora → LISTEN
// ══════════════════════════════════════════════════════════════
void handleStateDecode() {
    if (!packetAvailable) {
        transitionTo(STATE_LISTEN);
        return;
    }
    packetAvailable = false;

    APRSFrame frame;
    bool ok = APRSParser::decode(lastLoRaPacket, frame);

    if (!ok) {
        Serial.println(F("[DECODE] Trama inválida — descartando"));
        Display::showRx(lastLoRaPacket, false);
        transitionTo(STATE_LISTEN);   // ← evento: invalid_lora
        return;
    }

    Serial.printf("[DECODE] Trama válida: %s>%s:%s\n",
                  frame.source.c_str(),
                  frame.destination.c_str(),
                  frame.payload.c_str());
    Display::showRx(frame.raw, true);

    packetValid = true;
    transitionTo(STATE_UPLINK);       // ← evento: valid_lora
}

// ══════════════════════════════════════════════════════════════
//  ESTADO: UPLINK
//  Envía la trama al servidor APRS-IS.
//  Eventos: valid_link → LISTEN | error → ERROR
// ══════════════════════════════════════════════════════════════
void handleStateUplink() {
    if (!packetValid) {
        transitionTo(STATE_LISTEN);
        return;
    }
    packetValid = false;

    // Reconectar si el socket cayó
    if (!APRSISClient::isConnected()) {
        Serial.println(F("[UPLINK] Reconectando APRS-IS..."));
        if (!APRSISClient::connect(APRS_SERVER, APRS_PORT,
                                   CALLSIGN, APRS_PASSCODE, APRS_FILTER)) {
            Serial.println(F("[UPLINK] Fallo de reconexión → ERROR"));
            transitionTo(STATE_ERROR);   // ← evento: error
            return;
        }
    }

    // Construir trama APRS-IS y enviar
    String aprsPkt = APRSParser::toAPRSIS(lastLoRaPacket, CALLSIGN);
    if (!APRSISClient::send(aprsPkt)) {
        Serial.println(F("[UPLINK] Fallo de envío → ERROR"));
        transitionTo(STATE_ERROR);       // ← evento: error
        return;
    }

    Serial.printf("[UPLINK] Enviado: %s\n", aprsPkt.c_str());
    Display::showUplink(aprsPkt);

    errorRetryCount = 0;
    transitionTo(STATE_LISTEN);          // ← evento: valid_link
}

// ══════════════════════════════════════════════════════════════
//  ESTADO: BEACON_TX
//  Transmite el beacon de presencia del iGate.
//  Siempre vuelve a LISTEN.
// ══════════════════════════════════════════════════════════════
void handleStateBeaconTx() {
    Serial.println(F("[BEACON_TX] Enviando beacon..."));

    String beaconPkt = Beacon::build(CALLSIGN, LATITUDE, LONGITUDE,
                                     SYMBOL_TABLE, SYMBOL_CODE,
                                     BEACON_COMMENT);

    // Enviar al servidor APRS-IS
    if (APRSISClient::isConnected()) {
        APRSISClient::send(beaconPkt);
        Serial.printf("[BEACON_TX] %s\n", beaconPkt.c_str());
    } else {
        Serial.println(F("[BEACON_TX] Sin conexión APRS-IS — beacon no enviado"));
    }

    Display::showBeacon();
    beaconTimer = millis();   // Reiniciar temporizador

    transitionTo(STATE_LISTEN);
}

// ══════════════════════════════════════════════════════════════
//  ESTADO: ERROR
//  Intenta recuperación. Eventos: recovery → LISTEN | error → ERROR
// ══════════════════════════════════════════════════════════════
void handleStateError() {
    errorRetryCount++;
    Serial.printf("[ERROR] Intento de recuperación #%d\n", errorRetryCount);
    Display::showError(errorRetryCount);

    if (errorRetryCount > MAX_ERROR_RETRIES) {
        Serial.println(F("[ERROR] Reinicio del sistema (esp_restart)"));
        delay(2000);
        ESP.restart();
        return;
    }

    delay(ERROR_RETRY_DELAY_MS);

    // — Intentar reconexión WiFi —
    bool wifiOk = WiFiManager::isConnected();
    if (!wifiOk) {
        Serial.println(F("[ERROR] Reconectando WiFi..."));
        wifiOk = WiFiManager::connect(WIFI_SSID, WIFI_PASSWORD, WIFI_TIMEOUT_MS);
    }

    if (!wifiOk) {
        Serial.println(F("[ERROR] WiFi no disponible — reintentando"));
        return;   // Permanece en ERROR (event: error)
    }

    // — Intentar reconexión APRS-IS —
    APRSISClient::disconnect();
    APRSISClient::connect(APRS_SERVER, APRS_PORT,
                          CALLSIGN, APRS_PASSCODE, APRS_FILTER);

    // — Reiniciar LoRa si es necesario —
    if (!LoRaManager::isReady()) {
        Serial.println(F("[ERROR] Reiniciando LoRa..."));
        LoRaManager::init();
    }

    errorRetryCount = 0;
    transitionTo(STATE_LISTEN);   // ← evento: recovery
}
