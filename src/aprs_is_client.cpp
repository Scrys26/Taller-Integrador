// aprs_is_client.cpp

#include "aprs_is_client.h"
#include <WiFiClient.h>

namespace APRSISClient {

static WiFiClient _client;
static bool       _loggedIn = false;

bool connect(const char* server, uint16_t port,
             const char* callsign, const char* passcode,
             const char* filter) {

    _loggedIn = false;

    if (!_client.connect(server, port)) {
        Serial.printf("[APRS-IS] No se pudo conectar a %s:%d\n", server, port);
        return false;
    }

    Serial.printf("[APRS-IS] Conectado a %s:%d\n", server, port);

    // Esperar banner del servidor
    uint32_t t = millis();
    while (!_client.available() && millis() - t < 5000) delay(100);

    if (_client.available()) {
        String banner = _client.readStringUntil('\n');
        Serial.printf("[APRS-IS] Banner: %s\n", banner.c_str());
    }

    // Login
    String loginStr = String("user ") + callsign
                    + " pass " + passcode
                    + " vers iGate-TEC 1.0"
                    + " filter " + filter
                    + "\r\n";

    _client.print(loginStr);
    Serial.printf("[APRS-IS] Login enviado: %s", loginStr.c_str());

    // Esperar respuesta
    t = millis();
    while (!_client.available() && millis() - t < 5000) delay(100);

    if (_client.available()) {
        String resp = _client.readStringUntil('\n');
        Serial.printf("[APRS-IS] Resp login: %s\n", resp.c_str());

        if (resp.indexOf("verified") >= 0 || resp.indexOf("unverified") >= 0) {
            _loggedIn = true;
            Serial.println(F("[APRS-IS] Login OK"));
        }
    }

    return _loggedIn;
}

bool send(const String& line) {
    if (!isConnected()) return false;

    size_t written = _client.print(line);
    if (written != line.length()) {
        Serial.println(F("[APRS-IS] Error de escritura parcial"));
        return false;
    }

    return true;
}

bool isConnected() {
    return _client.connected() && _loggedIn;
}

void disconnect() {
    _client.stop();
    _loggedIn = false;
    Serial.println(F("[APRS-IS] Desconectado"));
}

// Leer keepalives del servidor para no llenar el buffer
void flush() {
    while (_client.available()) {
        String line = _client.readStringUntil('\n');
        if (!line.startsWith("#") && line.length() > 0) {
            Serial.printf("[APRS-IS] Rx: %s\n", line.c_str());
        }
    }
}

} // namespace APRSISClient
