
#pragma once
#include <Arduino.h>

namespace Beacon {

    /**
     * Construye la trama APRS-IS de beacon del iGate.
     *
     * @param callsign    Indicativo del iGate (ej: "TI2XYZ-10")
     * @param lat         Latitud decimal (+ Norte)
     * @param lon         Longitud decimal (+ Este)
     * @param symTable    Tabla de símbolos ('/' primaria)
     * @param symCode     Código de símbolo ('&' = iGate)
     * @param comment     Texto libre
     * @return Trama lista para APRSISClient::send()
     */
    String build(const String& callsign,
                 float lat, float lon,
                 char symTable, char symCode,
                 const String& comment);

} 
