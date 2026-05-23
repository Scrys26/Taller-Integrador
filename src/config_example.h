// config_example.h — Copiar como config.h y completar con los datos reales

#pragma once

#define CALLSIGN        "TI2XXX-10"
#define APRS_PASSCODE   "00000"
#define APRS_FILTER     "r/9.99/-84.09/150"

#define LATITUDE        9.9328f
#define LONGITUDE      -84.0879f
#define SYMBOL_TABLE   '/'
#define SYMBOL_CODE    '&'
#define BEACON_COMMENT  "iGate LoRa-APRS TEC / Proyecto"

#define WIFI_SSID           "NOMBRE_RED"
#define WIFI_PASSWORD       "CONTRASENA"
#define WIFI_TIMEOUT_MS     15000

#define APRS_SERVER     "rotate.aprs2.net"
#define APRS_PORT       14580

// Pines TTGO LoRa32 v2.1
#define LORA_SCK        5
#define LORA_MISO       19
#define LORA_MOSI       27
#define LORA_SS         18
#define LORA_RST        14
#define LORA_DIO0       26

#define LORA_FREQUENCY      433.775E6
#define LORA_BANDWIDTH      125E3
#define LORA_SF             12
#define LORA_CR             5
#define LORA_SYNC_WORD      0x12

#define OLED_ENABLED    true
#define OLED_SDA        21
#define OLED_SCL        22
#define OLED_RST        -1
#define OLED_ADDR       0x3C

#define BEACON_INTERVAL_MS  (15UL * 60UL * 1000UL)

#define MAX_ERROR_RETRIES    5
#define ERROR_RETRY_DELAY_MS 5000
