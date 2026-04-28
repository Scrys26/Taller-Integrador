/**
 * aprs_is_client.cpp — Implementación APRSISClient
 */

#include "aprs_is_client.h"
#include <WiFiClient.h>

namespace APRSISClient {

static WiFiClient _client;
static bool       _loggedIn = false;

// ──────────────────────────────────────────────────────────────
//  connect()
// ──────────────────────────────────────────────────────────────
bool connect(const char* server, uint16_t port,
             const char* callsign, const char* passcode,
             const char* filter) {

    _loggedIn = false;

    if (!_client.connect(server, port)) {
        Serial.printf("[APRS-IS] No se pudo conectar a %s:%d\n", server, port);
        return false;
    }

    Serial.printf("[APRS-IS] Conectado a %s:%d\n", server, port);

    // Esperar el banner del servidor
    uint32_t t = millis();
    while (!_client.available() && millis() - t < 5000) delay(100);

    if (_client.available()) {
        String banner = _client.readStringUntil('\n');
        Serial.printf("[APRS-IS] Banner: %s\n", banner.c_str());
    }

    // Enviar LOGIN
    String loginStr = String("user ") + callsign
                    + " pass " + passcode
                    + " vers iGate-TEC 1.0"
                    + " filter " + filter
                    + "\r\n";

    _client.print(loginStr);
    Serial.printf("[APRS-IS] Login enviado: %s", loginStr.c_str());

    // Esperar respuesta de login
    t = millis();
    while (!_client.available() && millis() - t < 5000) delay(100);

    if (_client.available()) {
        String resp = _client.readStringUntil('\n');
        Serial.printf("[APRS-IS] Resp login: %s\n", resp.c_str());

        // El servidor confirma con "# logresp CALL verified, ..."
        if (resp.indexOf("verified") >= 0 || resp.indexOf("unverified") >= 0) {
            _loggedIn = true;
            Serial.println(F("[APRS-IS] Login OK"));
        }
    }

    return _loggedIn;
}

// ──────────────────────────────────────────────────────────────
//  send()
// ──────────────────────────────────────────────────────────────
bool send(const String& line) {
    if (!isConnected()) return false;

    size_t written = _client.print(line);
    if (written != line.length()) {
        Serial.println(F("[APRS-IS] Error de escritura parcial"));
        return false;
    }

    return true;
}

// ──────────────────────────────────────────────────────────────
//  isConnected()
// ──────────────────────────────────────────────────────────────
bool isConnected() {
    return _client.connected() && _loggedIn;
}

// ──────────────────────────────────────────────────────────────
//  disconnect()
// ──────────────────────────────────────────────────────────────
void disconnect() {
    _client.stop();
    _loggedIn = false;
    Serial.println(F("[APRS-IS] Desconectado"));
}

// ──────────────────────────────────────────────────────────────
//  flush() — Leer keepalives del servidor sin bloquear
// ──────────────────────────────────────────────────────────────
void flush() {
    while (_client.available()) {
        String line = _client.readStringUntil('\n');
        if (line.startsWith("#")) {
            // Keepalive normal — ignorar silenciosamente
        } else if (line.length() > 0) {
            Serial.printf("[APRS-IS] Rx: %s\n", line.c_str());
        }
    }
}

} // namespace APRSISClient
