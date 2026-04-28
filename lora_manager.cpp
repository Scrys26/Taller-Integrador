/**
 * lora_manager.cpp — Implementación LoRaManager
 * Dependencia: arduino-LoRa (sandeepmistry)
 */

#include "lora_manager.h"
#include "config.h"
#include <SPI.h>
#include <LoRa.h>

namespace LoRaManager {

static bool _ready = false;

bool init() {
    SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);
    LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

    if (!LoRa.begin(LORA_FREQUENCY)) {
        Serial.println(F("[LoRa] begin() falló — verificar cableado"));
        _ready = false;
        return false;
    }

    LoRa.setSpreadingFactor(LORA_SF);
    LoRa.setSignalBandwidth(LORA_BANDWIDTH);
    LoRa.setCodingRate4(LORA_CR);
    LoRa.setSyncWord(LORA_SYNC_WORD);
    LoRa.enableCrc();

    Serial.printf("[LoRa] Init OK — %.3f MHz  SF%d  BW%.0fk\n",
                  LORA_FREQUENCY / 1E6,
                  LORA_SF,
                  LORA_BANDWIDTH / 1E3);

    _ready = true;
    return true;
}

bool receive(String& packet, int& rssi, float& snr) {
    int packetSize = LoRa.parsePacket();
    if (packetSize == 0) return false;

    packet = "";
    packet.reserve(packetSize);

    while (LoRa.available()) {
        packet += (char)LoRa.read();
    }

    rssi = LoRa.packetRssi();
    snr  = LoRa.packetSnr();

    // Filtrar paquetes vacíos o demasiado cortos
    if (packet.length() < 5) {
        Serial.println(F("[LoRa] Paquete demasiado corto — descartado"));
        return false;
    }

    return true;
}

bool isReady() {
    return _ready;
}

} // namespace LoRaManager
