/**
 * state_machine.h — Definición de estados y utilidades FSM
 */

#pragma once
#include <Arduino.h>

// ──────────────────────────────────────────────────────────────
//  Enumeración de estados del sistema
// ──────────────────────────────────────────────────────────────
enum SystemState : uint8_t {
    STATE_START      = 0,
    STATE_LISTEN     = 1,
    STATE_DECODE     = 2,
    STATE_UPLINK     = 3,
    STATE_BEACON_TX  = 4,
    STATE_ERROR      = 5
};

// ──────────────────────────────────────────────────────────────
//  Nombre del estado para logging
// ──────────────────────────────────────────────────────────────
inline const char* stateName(SystemState s) {
    switch (s) {
        case STATE_START:     return "START";
        case STATE_LISTEN:    return "LISTEN";
        case STATE_DECODE:    return "DECODE";
        case STATE_UPLINK:    return "UPLINK";
        case STATE_BEACON_TX: return "BEACON_TX";
        case STATE_ERROR:     return "ERROR";
        default:              return "UNKNOWN";
    }
}
