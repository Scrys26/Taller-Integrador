/**
 * wifi_manager.cpp — Implementación WiFiManager
 */

#include "wifi_manager.h"
#include <WiFi.h>

namespace WiFiManager {

bool connect(const char* ssid, const char* password, uint32_t timeoutMs) {
    Serial.printf("[WiFi] Conectando a '%s'", ssid);

    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.begin(ssid, password);

    uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - start > timeoutMs) {
            Serial.println(F("\n[WiFi] Timeout"));
            return false;
        }
        Serial.print('.');
        delay(500);
    }

    Serial.printf("\n[WiFi] Conectado — IP: %s  RSSI: %d dBm\n",
                  WiFi.localIP().toString().c_str(),
                  WiFi.RSSI());
    return true;
}

bool isConnected() {
    return (WiFi.status() == WL_CONNECTED);
}

String localIP() {
    return WiFi.localIP().toString();
}

void disconnect() {
    WiFi.disconnect(true);
}

} // namespace WiFiManager
