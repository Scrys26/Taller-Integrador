// main.cpp — iGate APRS-IS, Firmware Propio
// FSM: START → LISTEN → DECODE → UPLINK → LISTEN
// Plataforma: ESP32 + LoRa SX1278/SX1276

#include <Arduino.h>
#include "config.h"
#include "wifi_manager.h"
#include "lora_manager.h"
#include "aprs_parser.h"
#include "aprs_is_client.h"
#include "beacon.h"
#include "display.h"
#include "state_machine.h"

SystemState currentState  = STATE_START;
SystemState previousState = STATE_START;

String   lastLoRaPacket  = "";
bool     packetAvailable = false;
bool     packetValid     = false;
uint32_t beaconTimer     = 0;
uint8_t  errorRetryCount = 0;

void handleStateStart();
void handleStateListen();
void handleStateDecode();
void handleStateUplink();
void handleStateBeaconTx();
void handleStateError();
void transitionTo(SystemState newState);

void setup() {
    Serial.begin(115200);
    Serial.println(F("\n=== iGate APRS-IS — Firmware ==="));

    Display::init();
    Display::showBoot();

    currentState = STATE_START;
}

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
            break;
    }
}

void transitionTo(SystemState newState) {
    previousState = currentState;
    currentState  = newState;
    Serial.printf("[FSM] %s → %s\n", stateName(previousState), stateName(newState));
    Display::showState(newState);
}

// START: inicializa LoRa, WiFi y conexión APRS-IS
void handleStateStart() {
    Serial.println(F("[START] Iniciando subsistemas..."));

    if (!LoRaManager::init()) {
        Serial.println(F("[START] ERROR: fallo al iniciar LoRa"));
        transitionTo(STATE_ERROR);
        return;
    }
    Serial.println(F("[START] LoRa OK"));

    if (!WiFiManager::connect(WIFI_SSID, WIFI_PASSWORD, WIFI_TIMEOUT_MS)) {
        Serial.println(F("[START] ERROR: no se pudo conectar al WiFi"));
        transitionTo(STATE_ERROR);
        return;
    }
    Serial.printf("[START] WiFi OK — IP: %s\n", WiFiManager::localIP().c_str());

    if (!APRSISClient::connect(APRS_SERVER, APRS_PORT,
                               CALLSIGN, APRS_PASSCODE, APRS_FILTER)) {
        Serial.println(F("[START] WARN: no se pudo conectar al servidor APRS-IS"));
        // No es fatal; se reintenta en UPLINK
    }

    errorRetryCount = 0;
    beaconTimer     = millis();
    transitionTo(STATE_LISTEN);
}

// LISTEN: espera paquetes LoRa, verifica WiFi y timer de beacon
void handleStateListen() {
    if (millis() - beaconTimer >= BEACON_INTERVAL_MS) {
        transitionTo(STATE_BEACON_TX);
        return;
    }

    if (!WiFiManager::isConnected()) {
        Serial.println(F("[LISTEN] WiFi perdido → ERROR"));
        transitionTo(STATE_ERROR);
        return;
    }

    // Leer keepalives del servidor para no bloquear el socket
    APRSISClient::flush();

    String packet;
    int    rssi;
    float  snr;

    if (LoRaManager::receive(packet, rssi, snr)) {
        lastLoRaPacket  = packet;
        packetAvailable = true;
        Serial.printf("[LISTEN] Paquete recibido (%d bytes) RSSI=%d SNR=%.1f\n",
                      packet.length(), rssi, snr);
        transitionTo(STATE_DECODE);
        return;
    }

    delay(10);
}

// DECODE: valida y parsea la trama LoRa-APRS
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
        transitionTo(STATE_LISTEN);
        return;
    }

    Serial.printf("[DECODE] Trama válida: %s>%s:%s\n",
                  frame.source.c_str(),
                  frame.destination.c_str(),
                  frame.payload.c_str());
    Display::showRx(frame.raw, true);

    packetValid = true;
    transitionTo(STATE_UPLINK);
}

// UPLINK: reenvía la trama al servidor APRS-IS
void handleStateUplink() {
    if (!packetValid) {
        transitionTo(STATE_LISTEN);
        return;
    }
    packetValid = false;

    if (!APRSISClient::isConnected()) {
        Serial.println(F("[UPLINK] Reconectando APRS-IS..."));
        if (!APRSISClient::connect(APRS_SERVER, APRS_PORT,
                                   CALLSIGN, APRS_PASSCODE, APRS_FILTER)) {
            Serial.println(F("[UPLINK] Fallo de reconexión → ERROR"));
            transitionTo(STATE_ERROR);
            return;
        }
    }

    String aprsPkt = APRSParser::toAPRSIS(lastLoRaPacket, CALLSIGN);
    if (aprsPkt.length() == 0) {
        Serial.println(F("[UPLINK] Trama APRS-IS vacía → ERROR"));
        transitionTo(STATE_ERROR);
        return;
    }

    if (!APRSISClient::send(aprsPkt)) {
        Serial.println(F("[UPLINK] Fallo de envío → ERROR"));
        transitionTo(STATE_ERROR);
        return;
    }

    Serial.printf("[UPLINK] Enviado: %s", aprsPkt.c_str());
    Display::showUplink(aprsPkt);

    errorRetryCount = 0;
    transitionTo(STATE_LISTEN);
}

// BEACON_TX: transmite la posición del iGate cada 15 minutos
void handleStateBeaconTx() {
    Serial.println(F("[BEACON_TX] Enviando beacon..."));

    String beaconPkt = Beacon::build(CALLSIGN, LATITUDE, LONGITUDE,
                                     SYMBOL_TABLE, SYMBOL_CODE,
                                     BEACON_COMMENT);

    if (APRSISClient::isConnected()) {
        if (APRSISClient::send(beaconPkt)) {
            Serial.printf("[BEACON_TX] %s", beaconPkt.c_str());
        } else {
            Serial.println(F("[BEACON_TX] Fallo de envío del beacon"));
        }
    } else {
        Serial.println(F("[BEACON_TX] Sin conexión APRS-IS — beacon no enviado"));
    }

    Display::showBeacon();
    beaconTimer = millis();
    transitionTo(STATE_LISTEN);
}

// ERROR: intenta reconectar WiFi y APRS-IS; reinicia el ESP tras varios fallos
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

    bool wifiOk = WiFiManager::isConnected();
    if (!wifiOk) {
        Serial.println(F("[ERROR] Reconectando WiFi..."));
        wifiOk = WiFiManager::connect(WIFI_SSID, WIFI_PASSWORD, WIFI_TIMEOUT_MS);
    }

    if (!wifiOk) {
        Serial.println(F("[ERROR] WiFi no disponible — reintentando"));
        return;
    }

    APRSISClient::disconnect();
    bool aprsOk = APRSISClient::connect(APRS_SERVER, APRS_PORT,
                                        CALLSIGN, APRS_PASSCODE, APRS_FILTER);
    if (!aprsOk) {
        Serial.println(F("[ERROR] APRS-IS no disponible — reintentando"));
        return;
    }

    if (!LoRaManager::isReady()) {
        Serial.println(F("[ERROR] Reiniciando LoRa..."));
        if (!LoRaManager::init()) {
            Serial.println(F("[ERROR] LoRa no disponible — reintentando"));
            return;
        }
    }

    errorRetryCount = 0;
    transitionTo(STATE_LISTEN);
}
