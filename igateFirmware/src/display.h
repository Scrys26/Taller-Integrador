/**
 * display.h — Gestor de pantalla OLED (opcional)
 * Librería: Adafruit SSD1306 + Adafruit GFX
 * Si OLED_ENABLED = false, todas las funciones son no-op.
 */

#pragma once
#include <Arduino.h>
#include "state_machine.h"

namespace Display {

    /** Inicializar bus I2C y pantalla */
    void init();

    /** Pantalla de arranque con logo */
    void showBoot();

    /** Mostrar estado actual de la FSM */
    void showState(SystemState s);

    /**
     * Mostrar último paquete LoRa recibido.
     * @param packet Trama (se trunca a 21 chars por línea)
     * @param valid  true = verde, false = rojo (en log)
     */
    void showRx(const String& packet, bool valid);

    /** Confirmar envío al servidor */
    void showUplink(const String& pkt);

    /** Indicar transmisión de beacon */
    void showBeacon();

    /**
     * Mostrar error y contador de reintentos.
     * @param retries Número de intento actual
     */
    void showError(uint8_t retries);

} // namespace Display
