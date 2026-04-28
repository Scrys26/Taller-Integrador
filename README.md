# iGate APRS-IS — Firmware Propio
**Proyecto TEC | Grupos 1-2 | Firmware ESP32 LoRa → APRS-IS**

---

## Descripción

Firmware propio para un iGate APRS basado en ESP32 + LoRa SX1276/SX1278.
Implementa una **máquina de estados finita (FSM)** con los siguientes estados:

```
START → LISTEN → DECODE → UPLINK → LISTEN
              ↓         ↓
          BEACON_TX   ERROR → LISTEN
```

| Estado      | Función                                              |
|-------------|------------------------------------------------------|
| START       | Inicializa LoRa, WiFi y conexión APRS-IS             |
| LISTEN      | Escucha el canal LoRa y detecta paquetes             |
| DECODE      | Valida y parsea la trama LoRa-APRS                   |
| UPLINK      | Reenvía el paquete al servidor APRS-IS por WiFi/TCP  |
| BEACON_TX   | Transmite posición del iGate cada 15 min             |
| ERROR       | Recuperación automática de WiFi y servidor           |

---

## Hardware Soportado

- **TTGO LoRa32 v2.1** (recomendado)
- Heltec WiFi LoRa 32 v2
- Cualquier ESP32 + módulo LoRa SX1276/SX1278 cableado manualmente

---

## Requisitos de Software

| Herramienta | Versión mínima |
|-------------|----------------|
| Visual Studio Code | cualquiera |
| Extensión PlatformIO IDE | 3.x |
| Python | 3.8+ (usa PlatformIO) |

---

## Instalación y Compilación

### 1. Clonar el repositorio

```bash
git clone https://github.com/TU_USUARIO/igate-aprs-firmware.git
cd igate-aprs-firmware
```

### 2. Configurar parámetros

Editar `src/config.h` **antes** de compilar:

```cpp
// Tu indicativo + SSID numérico
#define CALLSIGN     "TI2ABC-10"

// Passcode: generar en https://apps.magicbug.co.uk/passcode/
#define APRS_PASSCODE "12345"

// Red WiFi
#define WIFI_SSID     "MiRed"
#define WIFI_PASSWORD "MiClave"

// Servidor APRS-IS (usar IP del servidor de la clase si corresponde)
#define APRS_SERVER   "rotate.aprs2.net"   // o "192.168.1.50"
#define APRS_PORT     14580

// Posición del iGate (decimal)
#define LATITUDE    9.9328f
#define LONGITUDE  -84.0879f
```

> **Pines LoRa** — Los pines predeterminados son para TTGO LoRa32 v2.1.
> Si usas otra placa, ajusta `LORA_SCK`, `LORA_MISO`, `LORA_MOSI`,
> `LORA_SS`, `LORA_RST` y `LORA_DIO0`.

### 3. Compilar y cargar

**Con PlatformIO IDE (VS Code):**
1. Abrir la carpeta del proyecto en VS Code.
2. PlatformIO descargará las dependencias automáticamente.
3. Clic en el ícono **Build** (✓) para compilar.
4. Conectar el ESP32 por USB.
5. Clic en **Upload** (→) para cargar.

**Con CLI de PlatformIO:**
```bash
pio run --target upload
```

### 4. Monitor serial

```bash
pio device monitor --baud 115200
```

Salida esperada al arrancar:
```
=== iGate APRS-IS — Firmware Propio ===
[FSM] START → LISTEN
[WiFi] Conectando a 'MiRed'....
[WiFi] Conectado — IP: 192.168.1.105
[LoRa] Init OK — 433.775 MHz  SF12  BW125k
[APRS-IS] Conectado a rotate.aprs2.net:14580
[APRS-IS] Login OK
[FSM] START → LISTEN
```

---

## Dependencias (gestionadas por PlatformIO)

```ini
lib_deps =
    sandeepmistry/LoRa@^0.8.0
    adafruit/Adafruit SSD1306@^2.5.7
    adafruit/Adafruit GFX Library@^1.11.9
```

---

## Estructura del Proyecto

```
igate-aprs-firmware/
├── platformio.ini          # Configuración PlatformIO
├── README.md               # Este archivo
└── src/
    ├── main.cpp            # Motor de la FSM principal
    ├── config.h            # ← EDITAR AQUÍ
    ├── state_machine.h     # Enum de estados + helper
    ├── wifi_manager.h/.cpp # Conexión WiFi con reconexión
    ├── lora_manager.h/.cpp # Driver LoRa (SX1276/78)
    ├── aprs_parser.h/.cpp  # Decodificador de tramas APRS
    ├── aprs_is_client.h/.cpp # Cliente TCP APRS-IS
    ├── beacon.h/.cpp       # Generador de beacons
    └── display.h/.cpp      # OLED SSD1306 (opcional)
```

---

## Flujo de la Máquina de Estados

```
         ┌─────────────────────────────────────────────────────────────┐
         │                         START                                │
         │  init LoRa + WiFi + APRS-IS                                 │
         └──────────────────────────┬──────────────────────────────────┘
                                    │ init
                                    ▼
         ┌─────────────────────────────────────────────────────────────┐
  ┌─────▶│                        LISTEN                               │◀──────────────┐
  │      │  Escucha LoRa │ Verifica WiFi │ Chequea timer beacon        │               │
  │      └──────┬───────────────────────────────────┬─────────────────┘               │
  │             │ lora_int                           │ timerbeacon                     │
  │             ▼                                   ▼                                  │
  │      ┌──────────────┐                  ┌─────────────────┐                        │
  │      │    DECODE    │                  │   BEACON_TX     │──── siempre ───────────▶│
  │      │ Parsea trama │                  └─────────────────┘                        │
  │      └──────┬───────┴─────────────────────────────┐                               │
  │             │ valid_lora                           │ invalid_lora                  │
  │             ▼                                     └──────────────────────────────▶│
  │      ┌──────────────┐                                                              │
  │      │    UPLINK    │                                                              │
  │      │  TCP → APRS  │─────── valid_link ───────────────────────────────────────▶  │
  │      └──────┬───────┘                                                              │
  │             │ error                                                                │
  │             ▼                                                                      │
  │      ┌──────────────┐                                                              │
  └──────│    ERROR     │─────── recovery ─────────────────────────────────────────▶  │
         │  Reintenta   │                                                              │
         └──────────────┘
```

---

## Notas de Desarrollo

- La frecuencia por defecto es **433.775 MHz** (LoRa-APRS UHF).
  Para 144.800 MHz (VHF), cambiar `LORA_FREQUENCY 144.800E6`.
- El sync word `0x12` es el estándar de la comunidad LoRa-APRS.
- El beacon se envía cada 15 minutos (configurable en `BEACON_INTERVAL_MS`).
- Tras `MAX_ERROR_RETRIES` (5) fallos consecutivos, el ESP32 hace `ESP.restart()`.

---

## Créditos

Desarrollado como proyecto académico en el TEC.
Basado en la arquitectura del proyecto de referencia
[richonguzman/LoRa_APRS_iGate](https://github.com/richonguzman/LoRa_APRS_iGate).
