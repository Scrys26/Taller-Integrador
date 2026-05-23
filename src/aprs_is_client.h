// aprs_is_client.h — Cliente TCP para servidor APRS-IS
//
// Login: user CALLSIGN pass PASSCODE vers iGate-TEC 1.0 filter FILTER\r\n
// El servidor envía keepalives '#' cada ~20 s; hay que leer el buffer
// periodicamente para no bloquearse.

#pragma once
#include <Arduino.h>

namespace APRSISClient {
    bool connect(const char* server, uint16_t port,
                 const char* callsign, const char* passcode,
                 const char* filter);
    bool send(const String& line);
    bool isConnected();
    void disconnect();
    void flush(); // leer y descartar keepalives del servidor
}
