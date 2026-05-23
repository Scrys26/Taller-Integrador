// aprs_parser.h — Parsing de tramas LoRa-APRS
//
// Formato recibido por LoRa:  SOURCE>DEST,PATH:PAYLOAD
// Formato para APRS-IS:       SOURCE>APRS,qAR,IGATE:PAYLOAD\r\n

#pragma once
#include <Arduino.h>

struct APRSFrame {
    String raw;
    String source;
    String destination;
    String path;
    String payload;
    bool   valid;
};

namespace APRSParser {
    bool   decode(const String& raw, APRSFrame& frame);
    String toAPRSIS(const String& raw, const String& igate);
}
