// beacon.h — Generador de beacons APRS del iGate
//
// Formato: CALLSIGN>APRS,TCPIP*:!DDMM.mmN/DDDMM.mmW&COMMENT

#pragma once
#include <Arduino.h>

namespace Beacon {
    String build(const String& callsign,
                 float lat, float lon,
                 char symTable, char symCode,
                 const String& comment);
}
