/**
 * wifi_manager.h — Gestión de conexión WiFi con reconexión automática
 */

#pragma once
#include <Arduino.h>

namespace WiFiManager {

    /**
     * Conectar al AP indicado.
     * @param ssid       Nombre de la red
     * @param password   Contraseña
     * @param timeoutMs  Tiempo máximo de espera en ms
     * @return true si se conectó exitosamente
     */
    bool connect(const char* ssid, const char* password, uint32_t timeoutMs);

    /** Devuelve true si hay enlace activo */
    bool isConnected();

    /** IP local como String */
    String localIP();

    /** Desconectar y limpiar */
    void disconnect();

} 
