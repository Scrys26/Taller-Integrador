

#pragma once
#include <Arduino.h>

// ──────────────────────────────────────────────────────────────
//  Estructura de una trama APRS decodificada
// ──────────────────────────────────────────────────────────────
struct APRSFrame {
    String raw;          // Trama original recibida por LoRa
    String source;       // Indicativo origen (ej: "LU1XYZ-9")
    String destination;  // Destino APRS 
    String path;         // Digi-path
    String payload;      // Información del paquete
    bool   valid;        // true si se parseó correctamente
};

namespace APRSParser {

    /**
     * Decodifica un paquete LoRa-APRS crudo.
     * @param raw    Bytes recibidos del radio
     * @param frame  Estructura a llenar
     * @return true si la trama es válida
     */
    bool decode(const String& raw, APRSFrame& frame);

    /**
     * Construye la línea APRS-IS lista para enviar al servidor TCP.
     * Formato: SOURCE>APRS,qAR,IGATE:PAYLOAD\r\n
     *
     * @param raw      Trama LoRa original
     * @param igate    Callsign del iGate
     * @return String lista para enviar por TCP
     */
    String toAPRSIS(const String& raw, const String& igate);

} 
