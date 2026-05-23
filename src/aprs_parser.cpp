// aprs_parser.cpp

#include "aprs_parser.h"

namespace APRSParser {

// Elimina el byte de encabezado LoRa (0x3C) y caracteres no imprimibles
static String stripHeader(const String& raw) {
    String data = raw;
    while (data.length() > 0) {
        char c = data[0];
        if ((uint8_t)c == 0x3C || (uint8_t)c < 32 || (uint8_t)c > 126) {
            data = data.substring(1);
        } else {
            break;
        }
    }
    data.trim();
    return data;
}

bool decode(const String& raw, APRSFrame& frame) {
    frame.raw   = raw;
    frame.valid = false;

    String data = stripHeader(raw);

    int gtPos = data.indexOf('>');
    if (gtPos < 1) {
        Serial.println(F("[APRS] Sin '>' — trama inválida"));
        return false;
    }

    frame.source = data.substring(0, gtPos);
    frame.source.trim();

    // Callsign válido: entre 3 y 9 caracteres
    if (frame.source.length() < 3 || frame.source.length() > 9) {
        Serial.printf("[APRS] Callsign inválido: '%s'\n", frame.source.c_str());
        return false;
    }

    String rest = data.substring(gtPos + 1);
    int colonPos = rest.indexOf(':');
    if (colonPos < 1) {
        Serial.println(F("[APRS] Sin ':' — trama inválida"));
        return false;
    }

    String destPath = rest.substring(0, colonPos);
    int commaPos    = destPath.indexOf(',');
    if (commaPos > 0) {
        frame.destination = destPath.substring(0, commaPos);
        frame.path        = destPath.substring(commaPos + 1);
    } else {
        frame.destination = destPath;
        frame.path        = "";
    }

    frame.payload = rest.substring(colonPos + 1);

    if (frame.payload.length() < 1) {
        Serial.println(F("[APRS] Payload vacío"));
        return false;
    }

    frame.valid = true;
    Serial.printf("[APRS] Decoded  SRC=%-9s  DST=%-6s  PLD=%s\n",
                  frame.source.c_str(),
                  frame.destination.c_str(),
                  frame.payload.substring(0, 40).c_str());
    return true;
}

String toAPRSIS(const String& raw, const String& igate) {
    String data = stripHeader(raw);

    int gtPos    = data.indexOf('>');
    int colonPos = data.indexOf(':');

    if (gtPos < 1 || colonPos < 1 || colonPos < gtPos) {
        return "";
    }

    String source  = data.substring(0, gtPos);
    String payload = data.substring(colonPos); // incluye ':'

    return source + ">APRS,qAR," + igate + payload + "\r\n";
}

} // namespace APRSParser
