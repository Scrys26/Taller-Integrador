# Grupo 1 — Firmware de iGate APRS

**Proyecto:** Taller Integrador — EL-5522 Taller de Comunicaciones Eléctricas  
**Instituto Tecnológico de Costa Rica, Escuela de Ingeniería Electrónica**

**Autores:** Luis Diego García Rojas · Randy Fernández Aguilar

---

## Introducción

El crecimiento de aplicaciones de telemetría y monitoreo remoto ha impulsado el desarrollo de tecnologías de comunicación inalámbrica capaces de operar a largas distancias con bajo consumo energético.

Entre estas tecnologías destaca **LoRa (Long Range)**, una técnica de modulación basada en *Chirp Spread Spectrum* diseñada para redes de área amplia de baja potencia (*LPWAN*). Por otra parte, el **Automatic Packet Reporting System (APRS)** es un sistema ampliamente utilizado en el servicio de radioaficionados para transmitir información digital como posición geográfica, mensajes cortos y datos de sensores en tiempo real.

Dentro de esta arquitectura, el **iGate** recibe paquetes transmitidos por radio y los reenvía hacia la red global **APRS-IS** mediante una conexión a Internet. Este proyecto desarrolla el firmware completo de un iGate basado en ESP32 + LoRa, implementado desde cero como parte del curso.

---

## Justificación

Al utilizar LoRa como capa física, el sistema puede operar en **bandas ISM de uso libre** (902–940 MHz en Costa Rica), lo que simplifica el despliegue administrativo al no requerir concesiones individuales del espectro. Esto facilita la experimentación y validación del sistema con plataformas globales como **aprs.fi** y **aprsdirect.de**.

---

## Objetivos

**General:** Desarrollar el firmware de un iGate APRS basado en LoRa capaz de recibir paquetes de nodos remotos y retransmitirlos hacia la red APRS-IS mediante Internet.

**Específicos:**
- Investigar los fundamentos teóricos de APRS y LoRa.
- Analizar la normativa costarricense de uso del espectro (PNAF).
- Implementar un firmware propio con reconexión automática y transmisión de beacons.

---

## Arquitectura del Firmware

El firmware implementa una **máquina de estados finita (FSM)**:

```
START → LISTEN → DECODE → UPLINK → LISTEN
              ↓         ↓
          BEACON_TX   ERROR → LISTEN
```

| Estado      | Descripción                                          |
|-------------|------------------------------------------------------|
| START       | Inicializa LoRa, WiFi y conexión APRS-IS             |
| LISTEN      | Escucha el canal LoRa, verifica WiFi y timer beacon  |
| DECODE      | Valida y parsea la trama LoRa-APRS recibida          |
| UPLINK      | Reenvía el paquete al servidor APRS-IS por TCP       |
| BEACON_TX   | Transmite la posición del iGate cada 15 minutos      |
| ERROR       | Reconexión automática de WiFi y servidor APRS-IS     |

---

## Hardware

- **TTGO LoRa32 v2.1** (placa utilizada en el proyecto)
- Compatible con Heltec WiFi LoRa 32 v2

---

## Estructura del Repositorio

```
Taller-Integrador/
├── platformio.ini
├── README.md
├── src/
│   ├── main.cpp              # Motor de la FSM
│   ├── config.h              # Parámetros — editar antes de compilar
│   ├── config_example.h      # Plantilla de configuración
│   ├── state_machine.h       # Definición de estados
│   ├── wifi_manager.h/.cpp   # Conexión WiFi
│   ├── lora_manager.h/.cpp   # Driver LoRa SX1276/78
│   ├── aprs_parser.h/.cpp    # Decodificador de tramas APRS
│   ├── aprs_is_client.h/.cpp # Cliente TCP APRS-IS
│   ├── beacon.h/.cpp         # Generador de beacons
│   └── display.h/.cpp        # OLED SSD1306 (opcional)
├── Informe/
│   └── Latex/                # Informe del proyecto en LaTeX
├── Presentación inicial/
└── doc/                      # Diagramas y documentación adicional
```

---

## Compilación y Carga

**Requisitos:** VS Code + PlatformIO IDE, Python 3.8+

### 1. Configurar parámetros

Copiar `src/config_example.h` como `src/config.h` y editar:

```cpp
#define CALLSIGN      "TI2ABC-10"   // indicativo + SSID
#define APRS_PASSCODE "12345"       // generar en apps.magicbug.co.uk/passcode
#define WIFI_SSID     "MiRed"
#define WIFI_PASSWORD "MiClave"
#define APRS_SERVER   "rotate.aprs2.net"  // o IP del servidor local
```

### 2. Compilar y cargar

```bash
pio run --target upload
```

### 3. Monitor serial

```bash
pio device monitor --baud 115200
```

Salida esperada al arrancar:
```
=== iGate APRS-IS — Firmware ===
[LoRa] Init OK — 433.775 MHz  SF12  BW125k
[WiFi] Conectado — IP: 192.168.1.x
[APRS-IS] Login OK
[FSM] START → LISTEN
```

---

## Dependencias

```ini
lib_deps =
    sandeepmistry/LoRa@^0.8.0
    adafruit/Adafruit SSD1306@^2.5.7
    adafruit/Adafruit GFX Library@^1.11.9
```

---

## Diagrama de Gantt

![Diagrama de Gantt](/doc/dgantt.png)

---

➡️ **Documentación adicional:** [Marco Teórico](/doc/02_marco_teorico.md)