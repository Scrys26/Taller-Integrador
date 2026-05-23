// beacon.cpp

#include "beacon.h"

namespace Beacon {

// Convertir latitud decimal a formato APRS: DDMM.mmN/S
static String encodeLatitude(float lat) {
    char  hemi   = (lat >= 0) ? 'N' : 'S';
    float absLat = fabsf(lat);
    int   deg    = (int)absLat;
    float min    = (absLat - deg) * 60.0f;

    char buf[12];
    snprintf(buf, sizeof(buf), "%02d%05.2f%c", deg, min, hemi);
    return String(buf);
}

// Convertir longitud decimal a formato APRS: DDDMM.mmE/W
static String encodeLongitude(float lon) {
    char  hemi   = (lon >= 0) ? 'E' : 'W';
    float absLon = fabsf(lon);
    int   deg    = (int)absLon;
    float min    = (absLon - deg) * 60.0f;

    char buf[13];
    snprintf(buf, sizeof(buf), "%03d%05.2f%c", deg, min, hemi);
    return String(buf);
}

String build(const String& callsign,
             float lat, float lon,
             char symTable, char symCode,
             const String& comment) {

    String position = "!";
    position += encodeLatitude(lat);
    position += symTable;
    position += encodeLongitude(lon);
    position += symCode;
    position += comment;

    return callsign + ">APRS,TCPIP*:" + position + "\r\n";
}

} // namespace Beacon
