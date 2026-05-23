// lora_manager.h — Driver del chip LoRa SX1276/SX1278

#pragma once
#include <Arduino.h>

namespace LoRaManager {
    bool init();
    bool receive(String& packet, int& rssi, float& snr);
    bool isReady();
}
