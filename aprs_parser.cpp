/**
 * aprs_parser.cpp — Implementación APRSParser
 */

#include "aprs_parser.h"

namespace APRSParser {

// ──────────────────────────────────────────────────────────────
//  decode()
//  Los paquetes LoRa-APRS tienen el formato:
//    <FRAME_HEADER 1 byte 0x3C> SOURCE>DEST,PATH:PAYLOAD
//  o simplemente:
//    SOURCE>DEST,PATH:PAYLOAD
// ──────────────────────────────────────────────────────────────
bool decode(const String& raw, APRSFrame& frame) {
    frame.raw   = raw;
    frame.valid = false;

    // Eliminar byte de cabecera LoRa si existe (0x3C '<')
    String data = raw;
    if (data.length() > 0 && (uint8_t)data[0] == 0x3C) {
        data = data.substring(1);
    }
    data.trim();

    // Buscar separador '>' que divide SOURCE del resto
    int gtPos = data.indexOf('>');
    if (gtPos < 1) {
        Serial.println(F("[APRS] Sin '>' — trama inválida"));
        return false;
    }

    frame.source = data.substring(0, gtPos);
    frame.source.trim();

    // Validar longitud del callsign (3-9 caracteres)
    if (frame.source.length() < 3 || frame.source.length() > 9) {
        Serial.printf("[APRS] Callsign inválido: '%s'\n", frame.source.c_str());
        return false;
    }

    // Separar "DEST,PATH:PAYLOAD"
    String rest = data.substring(gtPos + 1);
    int colonPos = rest.indexOf(':');
    if (colonPos < 1) {
        Serial.println(F("[APRS] Sin ':' — trama inválida"));
        return false;
    }

    // Separar destino y path
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

    // Validar que haya payload mínimo
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

// ──────────────────────────────────────────────────────────────
//  toAPRSIS()
//  Genera la línea para enviar por TCP al servidor APRS-IS.
//  Formato estándar:  SOURCE>APRS,qAR,IGATE:PAYLOAD\r\n
// ──────────────────────────────────────────────────────────────
String toAPRSIS(const String& raw, const String& igate) {
    // Limpiar byte de cabecera LoRa si existe
    String data = raw;
    if (data.length() > 0 && (uint8_t)data[0] == 0x3C) {
        data = data.substring(1);
    }
    data.trim();

    // Si ya tiene formato SOURCE>..., reemplazar el path por qAR,IGATE
    int gtPos = data.indexOf('>');
    int colonPos = data.indexOf(':');

    if (gtPos < 1 || colonPos < 1 || colonPos < gtPos) {
        // No se puede reformatear; devolver vacío → error
        return "";
    }

    String source  = data.substring(0, gtPos);
    String payload = data.substring(colonPos); // incluye ':'

    // Construir trama APRS-IS
    String line = source + ">APRS,qAR," + igate + payload + "\r\n";
    return line;
}

} // namespace APRSParser
