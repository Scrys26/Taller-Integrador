// display.h — Manejo de pantalla OLED SSD1306
// Si OLED_ENABLED = false en config.h, todas las funciones son no-op.

#pragma once
#include <Arduino.h>
#include "state_machine.h"

namespace Display {
    void init();
    void showBoot();
    void showState(SystemState s);
    void showRx(const String& packet, bool valid);
    void showUplink(const String& pkt);
    void showBeacon();
    void showError(uint8_t retries);
}
