// state_machine.h — Estados de la FSM del iGate

#pragma once
#include <Arduino.h>

enum SystemState : uint8_t {
    STATE_START      = 0,
    STATE_LISTEN     = 1,
    STATE_DECODE     = 2,
    STATE_UPLINK     = 3,
    STATE_BEACON_TX  = 4,
    STATE_ERROR      = 5
};

// Devuelve el nombre del estado como string (para logs)
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
