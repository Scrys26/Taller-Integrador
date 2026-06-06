/**
 * lora_manager.h — Abstracción del chip LoRa (SX1276/SX1278)
 */

#pragma once
#include <Arduino.h>

namespace LoRaManager {

    /** Inicializar SPI y configurar el chip LoRa según config.h */
    bool init();

    /**
     * Consultar si hay un paquete disponible y leerlo.
     * @param[out] packet  Contenido del paquete (raw bytes como String)
     * @param[out] rssi    RSSI en dBm
     * @param[out] snr     SNR en dB
     * @return true si se recibió un paquete completo
     */
    bool receive(String& packet, int& rssi, float& snr);

    /** Devuelve true si el chip respondió correctamente en el último init */
    bool isReady();

} // namespace LoRaManager
