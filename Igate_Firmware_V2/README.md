# iGate APRS-IS — Firmware Propio
**Proyecto TEC | Grupos 1-2 | Firmware ESP32 LoRa → APRS-IS**

---

## Descripción

Firmware propio para un iGate APRS basado en **ESP32 + LoRa SX1276/SX1278**. El sistema recibe paquetes APRS por radio LoRa, los valida, los convierte al formato APRS-IS y los reenvía por WiFi/TCP a un servidor APRS-IS.

El firmware implementa una **máquina de estados finita (FSM)** con el siguiente flujo:

```text
START → LISTEN → DECODE → UPLINK → LISTEN
              ↓         ↓
          BEACON_TX   ERROR → LISTEN
```

| Estado | Función |
|---|---|
| `START` | Inicializa OLED, LoRa, WiFi y APRS-IS |
| `LISTEN` | Escucha el canal LoRa, verifica WiFi y atiende temporizador de beacon |
| `DECODE` | Valida y parsea la trama LoRa-APRS |
| `UPLINK` | Reenvía el paquete al servidor APRS-IS por TCP |
| `BEACON_TX` | Publica la posición del iGate periódicamente |
| `ERROR` | Ejecuta recuperación automática de WiFi, APRS-IS y LoRa |

---

## Estado actual del proyecto frente a la guía

Este repositorio cubre completamente lo solicitado para:

- **Semanas 6–8:** rutinas de conexión WiFi y configuración del chip LoRa.
- **Semana 9:** demostración del firmware propio recibiendo paquetes LoRa y mostrándolos por serial o reenviándolos a un servidor.

Además, el proyecto ya adelanta parte de:

- **Semanas 10–12:** conversión LoRa → APRS-IS y envío por TCP.
- **Semanas 13–15:** reconexión automática y documentación de compilación/carga.

---

## Hardware validado

Configuración validada en pruebas del proyecto:

- **TTGO LoRa32 v2.1 / LILYGO equivalente**
- ESP32 integrado
- Radio LoRa SX1276/SX1278
- Pantalla OLED SSD1306 I2C (128x64)

Otras placas ESP32 + LoRa pueden funcionar, pero requieren ajuste manual de pines en `src/config.h`.

---

## Requisitos de software

| Herramienta | Versión mínima |
|---|---|
| Visual Studio Code | cualquiera |
| PlatformIO IDE | 3.x |
| Python | 3.8+ |

---

## Configuración previa

Editar `src/config.h` antes de compilar.

### Parámetros obligatorios

```cpp
#define CALLSIGN        "TI2ABC-10"
#define APRS_PASSCODE   "12345"
#define APRS_FILTER     "r/9.99/-84.09/150"

#define WIFI_SSID       "MiRed"
#define WIFI_PASSWORD   "MiClave"
#define WIFI_TIMEOUT_MS 15000

#define APRS_SERVER     "rotate.aprs2.net"   // o IP del servidor local de la clase
#define APRS_PORT       14580
```

### Posición del iGate para beacon

```cpp
#define LATITUDE        9.9328f
#define LONGITUDE      -84.0879f
#define SYMBOL_TABLE    '/'
#define SYMBOL_CODE     '&'
#define BEACON_COMMENT  "iGate LoRa-APRS TEC"
```

### Parámetros LoRa

```cpp
#define LORA_FREQUENCY  433.775E6
#define LORA_BANDWIDTH  125E3
#define LORA_SF         12
#define LORA_CR         5
#define LORA_SYNC_WORD  0x12
```

### OLED

```cpp
#define OLED_ENABLED    true
#define OLED_SDA        21
#define OLED_SCL        22
#define OLED_RST        -1
#define OLED_ADDR       0x3C
```

> Si la clase asigna un **servidor local**, reemplazar `APRS_SERVER` por la IP suministrada, por ejemplo:
>
> ```cpp
> #define APRS_SERVER "192.168.1.50"
> ```

---

## Compilación y carga

### Opción 1 — VS Code + PlatformIO

1. Abrir la carpeta del proyecto en **VS Code**.
2. Verificar que `platformio.ini` use el entorno correcto.
3. Editar `src/config.h` con callsign, WiFi, servidor y posición.
4. Presionar **Build** (✓).
5. Conectar el ESP32 por USB.
6. Presionar **Upload** (→).
7. Abrir el **Serial Monitor** a `115200` baudios.

### Opción 2 — CLI

```bash
pio run
pio run --target upload
pio device monitor --baud 115200
```

---

## Salida serial esperada al arrancar

```text
=== iGate APRS-IS — Firmware ===
[Display] OLED OK
[START] Iniciando subsistemas...
[LoRa] Init OK — 433.775 MHz  SF12  BW125k
[START] LoRa OK
[WiFi] Conectando a 'MiRed'...
[WiFi] Conectado — IP: 192.168.1.105  RSSI: -45 dBm
[START] WiFi OK — IP: 192.168.1.105
[APRS-IS] Conectado a rotate.aprs2.net:14580
[APRS-IS] Login OK
[FSM] START → LISTEN
```

Si llega un paquete válido desde un tracker:

```text
[LISTEN] Paquete recibido (60 bytes) RSSI=-100 SNR=-14.8
[FSM] LISTEN → DECODE
[DECODE] Trama válida: TI2ABC-9>APLT00:...
[FSM] DECODE → UPLINK
[UPLINK] Enviado: TI2ABC-9>APRS,qAR,TI2TEC-10:...
[FSM] UPLINK → LISTEN
```

---

## Estructura del proyecto

```text
igate-aprs-firmware/
├── platformio.ini
├── README.md
└── src/
    ├── main.cpp              # FSM principal
    ├── config.h              # Parámetros de compilación
    ├── state_machine.h       # Estados y helper de nombres
    ├── wifi_manager.h/.cpp   # Conexión WiFi y utilidades
    ├── lora_manager.h/.cpp   # Inicialización y recepción LoRa
    ├── aprs_parser.h/.cpp    # Decodificación APRS + conversión APRS-IS
    ├── aprs_is_client.h/.cpp # Cliente TCP APRS-IS
    ├── beacon.h/.cpp         # Generación de beacon
    └── display.h/.cpp        # Dashboard OLED
```

---

## Flujo funcional del firmware

```text
         ┌──────────────────────────────────────────┐
         │                  START                   │
         │ OLED + LoRa + WiFi + APRS-IS            │
         └───────────────────┬──────────────────────┘
                             ▼
         ┌──────────────────────────────────────────┐
         │                  LISTEN                  │
         │ Escucha LoRa / revisa WiFi / timer       │
         └───────┬───────────────────────┬──────────┘
                 │                       │
                 ▼                       ▼
         ┌──────────────┐        ┌─────────────────┐
         │    DECODE    │        │   BEACON_TX     │
         └──────┬───────┘        └────────┬────────┘
                │                         │
                ▼                         │
         ┌──────────────┐                 │
         │    UPLINK    │─────────────────┘
         └──────┬───────┘
                │ error
                ▼
         ┌──────────────┐
         │    ERROR     │
         │ Reconexión   │
         └──────┬───────┘
                ▼
              LISTEN
```

---

## Dependencias

Las dependencias son gestionadas por PlatformIO:

```ini
lib_deps =
    sandeepmistry/LoRa@^0.8.0
    adafruit/Adafruit SSD1306@^2.5.7
    adafruit/Adafruit GFX Library@^1.11.9
```

---

## Validación funcional ya alcanzada

El firmware ya permite demostrar:

- inicialización correcta de LoRa,
- conexión WiFi,
- login APRS-IS,
- recepción de paquetes LoRa,
- decodificación APRS,
- reenvío a APRS-IS,
- visualización del estado en OLED,
- beacon periódico del iGate.

---

## Validación para semanas 13–15

### 1. Reconexión automática

La guía solicita reconexión automática ante caídas de WiFi y servidor.

El firmware lo implementa mediante el estado `ERROR`, que:

- intenta reconectar WiFi,
- intenta reconectar APRS-IS,
- reinicializa LoRa si es necesario,
- y vuelve a `LISTEN` cuando la recuperación es exitosa.

### Prueba sugerida

1. Arrancar el iGate hasta llegar a `LISTEN`.
2. Apagar temporalmente el AP WiFi o alejar el equipo hasta perder conexión.
3. Verificar que aparezca el paso a `ERROR`.
4. Restaurar la red WiFi.
5. Confirmar retorno a `LISTEN` sin reinicio manual.

Resultado esperado:

```text
[LISTEN] WiFi perdido → ERROR
[ERROR] Intento de recuperación #1
[ERROR] Reconectando WiFi...
[ERROR] Reconectando APRS-IS...
[FSM] ERROR → LISTEN
```

### 2. Recepción de múltiples nodos

La guía pide probar recepción simultánea de múltiples trackers.

### Prueba sugerida

1. Encender dos o más trackers LoRa.
2. Transmitir paquetes con distinto `SOURCE`.
3. Verificar en serial y OLED que el iGate procese múltiples estaciones.

Resultado esperado:

```text
[DECODE] Trama válida: TI2AAA-9>...
[UPLINK] Enviado: TI2AAA-9>APRS,qAR,TI2TEC-10:...

[DECODE] Trama válida: TI2BBB-9>...
[UPLINK] Enviado: TI2BBB-9>APRS,qAR,TI2TEC-10:...
```


## Tramas principales manejadas por el firmware

### 1. Trama LoRa de entrada

```text
SOURCE>DEST,PATH:PAYLOAD
```

Ejemplo:

```text
TI2ABC-9>APLT00,WIDE1-1:!0933.55N/08402.74W>Tracker TEC
```

### 2. Login APRS-IS

```text
user CALLSIGN pass PASSCODE vers SOFTWARE filter FILTRO\r\n
```

### 3. Trama APRS-IS de salida

```text
SOURCE>APRS,qAR,IGATE:PAYLOAD\r\n
```

Ejemplo:

```text
TI2ABC-9>APRS,qAR,TI2TEC-10:!0933.55N/08402.74W>Tracker TEC\r\n
```

### 4. Beacon del iGate

```text
CALLSIGN>APRS,TCPIP*:!LAT/LON&COMENTARIO\r\n
```

---

## Notas de implementación

- Frecuencia por defecto: **433.775 MHz**.
- Para bandas distintas, ajustar `LORA_FREQUENCY`.
- `LORA_SYNC_WORD = 0x12` corresponde al estándar LoRa-APRS.
- El beacon se transmite cada **15 minutos**.
- Tras `MAX_ERROR_RETRIES`, el ESP32 ejecuta `ESP.restart()`.
- La posición del iGate es **estática**, tomada de `config.h`; no se usa GPS dinámico en esta versión.

---


## Créditos

Desarrollado como proyecto académico en el TEC.
Basado en la arquitectura de referencia de `richonguzman/LoRa_APRS_iGate`.
