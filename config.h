/**
 * config.h — Configuración del iGate APRS-IS
 * ============================================================
 * EDITA ESTE ARCHIVO antes de compilar.
 * ============================================================
 */

#pragma once

// ──────────────────────────────────────────────────────────────
//  Identificación del iGate
// ──────────────────────────────────────────────────────────────
#define CALLSIGN        "TI2TEC-10"    // Indicativo + SSID
#define APRS_PASSCODE   "20668"       
#define APRS_FILTER     "r/9.99/-84.09/150"  // Filtro de rango (radio 150 km)

// ──────────────────────────────────────────────────────────────
//  Posición del iGate (grados decimales)
// ──────────────────────────────────────────────────────────────
#define LATITUDE        9.8594459f         // + Norte / - Sur
#define LONGITUDE      -83.912458f        // + Este  / - Oeste
#define SYMBOL_TABLE   '/'
#define SYMBOL_CODE    '&'              // '&' = iGate en el mapa APRS
#define BEACON_COMMENT  "iGate LoRa-APRS TEC / Proyecto"

// ──────────────────────────────────────────────────────────────
//  WiFi
// ──────────────────────────────────────────────────────────────
#define WIFI_SSID           "S24 de Randy Steve"
#define WIFI_PASSWORD       "tifanny2027"
#define WIFI_TIMEOUT_MS     15000        // 15 s máximo de espera

// ──────────────────────────────────────────────────────────────
//  Servidor APRS-IS
// ──────────────────────────────────────────────────────────────
// Para el servidor de la clase, cambiar por la IP asignada:
// Ej: #define APRS_SERVER "192.168.1.50"
#define APRS_SERVER     "rotate.aprs2.net"
#define APRS_PORT       14580

// ──────────────────────────────────────────────────────────────
//  LoRa — Pines ESP32 (TTGO LoRa32 / Heltec v2)
// ──────────────────────────────────────────────────────────────
#define LORA_SCK        5
#define LORA_MISO       19
#define LORA_MOSI       27
#define LORA_SS         18
#define LORA_RST        14
#define LORA_DIO0       26

// Parámetros de radio (LoRa-APRS estándar)
#define LORA_FREQUENCY      433.775E6   // Hz — cambiar a 144.800 si VHF
#define LORA_BANDWIDTH      125E3
#define LORA_SF             12          // Spreading Factor
#define LORA_CR             5           // Coding Rate 4/5
#define LORA_SYNC_WORD      0x12        // Sync word LoRa-APRS

// ──────────────────────────────────────────────────────────────
//  Display OLED (I2C) — opcional
// ──────────────────────────────────────────────────────────────
#define OLED_ENABLED    true
#define OLED_SDA        4
#define OLED_SCL        15
#define OLED_RST        16
#define OLED_ADDR       0x3C

// ──────────────────────────────────────────────────────────────
//  Beacon
// ──────────────────────────────────────────────────────────────
#define BEACON_INTERVAL_MS  (15UL * 60UL * 1000UL)  // 15 minutos

// ──────────────────────────────────────────────────────────────
//  Control de errores
// ──────────────────────────────────────────────────────────────
#define MAX_ERROR_RETRIES    5           // Reinicio de CPU tras N fallos
#define ERROR_RETRY_DELAY_MS 5000        // 5 s entre reintentos
