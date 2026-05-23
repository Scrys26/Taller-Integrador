// wifi_manager.h — Manejo de conexión WiFi

#pragma once
#include <Arduino.h>

namespace WiFiManager {
    bool   connect(const char* ssid, const char* password, uint32_t timeoutMs);
    bool   isConnected();
    String localIP();
    void   disconnect();
}
