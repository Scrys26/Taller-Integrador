
#pragma once
#include <Arduino.h>

namespace APRSISClient {

  
    bool connect(const char* server, uint16_t port,
                 const char* callsign, const char* passcode,
                 const char* filter);


    bool send(const String& line);

    /** Verificar si el socket TCP sigue activo */
    bool isConnected();

    /** Cerrar la conexión */
    void disconnect();

    /**
     * Leer y descartar datos del servidor (keepalive / acks).
     * Llamar desde el estado LISTEN para evitar buffer lleno.
     */
    void flush();

} 
