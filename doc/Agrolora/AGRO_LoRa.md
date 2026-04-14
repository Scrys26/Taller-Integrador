# AGRO-LoRa
### Diseño preliminar de un nodo de monitoreo agrícola de bajo consumo

**Taller Integrador**  
Randy Fernández Aguilar · Luis Diego García Rojas  
Escuela de Ingeniería Electrónica — Instituto Tecnológico de Costa Rica · 2026

---

## Tabla de Contenidos

1. [Diagrama de Contexto](#1-diagrama-de-contexto)
2. [Descomposición Funcional](#2-descomposición-funcional)
3. [Arquitectura de Interconexión](#3-arquitectura-de-interconexión)
4. [Secuencia de Operación (Firmware)](#4-secuencia-de-operación-firmware)
5. [Máquina de Estados (FSM)](#5-máquina-de-estados-fsm)
6. [Diseño de Hardware](#6-diseño-de-hardware)
7. [Modelo de Consumo Energético](#7-modelo-de-consumo-energético)
8. [Resultados y Autonomía](#8-resultados-y-autonomía)

---

## 1. Diagrama de Contexto

![Diagrama de contexto del sistema Agro-LoRa](/doc/Agrolora/1.png)

**Entradas**
- Humedad del suelo
- Temperatura / humedad ambiente
- Nivel de batería

**Nodo**
- Adquisición de datos
- Procesamiento y deep sleep (95 % del tiempo)

**Salida**
- Enlace RF / LoRa hacia gateway central a 2 km

> **Clave:** El nodo sensa, empaqueta y transmite, permaneciendo la mayor parte del tiempo en bajo consumo.

---

## 2. Descomposición Funcional

![Diagrama de bloques funcionales del nodo sensor Agro-LoRa](/doc/Agrolora/2.png)

| Bloque | Descripción |
|--------|-------------|
| **Gestión de energía** | Regulación, temporización, deep sleep y habilitación de sensores |
| **Adquisición** | Lectura analógica del suelo, lectura digital ambiental y monitoreo de batería |
| **Procesamiento y control** | Empaquetado, secuencia FSM y reintentos de transmisión |
| **Etapa RF** | Transceptor LoRa y antena como interfaz con el gateway |

> Separación funcional entre energía, adquisición, control y radiofrecuencia.

---

## 3. Arquitectura de Interconexión

![Esquemático funcional del nodo Agro-LoRa](/doc/Agrolora/3.png)

### Bus SPI

| Señal | GPIO |
|-------|------|
| SCK   | GPIO5  |
| MOSI  | GPIO18 |
| MISO  | GPIO19 |
| CS    | GPIO21 |

### Bus I²C

| Señal | GPIO |
|-------|------|
| SDA   | GPIO23 |
| SCL   | GPIO22 |

> Pull-up externos de **4.7 kΩ**

### Señales ADC

| Señal | GPIO |
|-------|------|
| Humedad del suelo | GPIO34 / A2 |
| Divisor de batería | GPIO39 / A3 |

### Señales de Control

| Señal | GPIO |
|-------|------|
| RST_LORA | GPIO14 |
| DIO0 | GPIO27 |
| EN_SENS | GPIO32 |
| GPIO_LED | GPIO33 |

---

## 4. Secuencia de Operación (Firmware)

![Diagrama de flujo del firmware Agro-LoRa](/doc/Agrolora/4.png)

| Parámetro | Valor |
|-----------|-------|
| Periodo de ciclo | ~10 minutos |
| Tiempo de estabilización sensores | 50 ms |
| Timeout de sensor | 200 ms |
| Timeout de ACK | 3 s |
| Reintentos de transmisión | hasta 3 |

---

## 5. Máquina de Estados (FSM)

![Máquina de estados del nodo Agro-LoRa](/doc/Agrolora/5.png)

### Estados

| Estado | Descripción |
|--------|-------------|
| `SLEEP` | Deep sleep, mínimo consumo |
| `SENSING` | Lectura de sensores con EN_SENS activo |
| `TX_DATA` | Transmisión del paquete LoRa |
| `WAIT_ACK` | Espera confirmación del gateway |
| `ERROR_RETRY` | Reintento ante fallo de transmisión |

### Transiciones principales

| Evento | Origen → Destino |
|--------|-----------------|
| Timer_RTC expira [Bat > 3.3V] | `SLEEP` → `SENSING` |
| Timeout_Sensor / MOSFET OFF | `SENSING` → `ERROR_RETRY` |
| Tx_complete | `TX_DATA` → `WAIT_ACK` |
| ACK_receive | `WAIT_ACK` → `SLEEP` |
| timeout_ACK ≥ 3 ms | `WAIT_ACK` → `ERROR_RETRY` |
| Tx_fail | `TX_DATA` → `ERROR_RETRY` |
| cnt_retry < 3 | `ERROR_RETRY` → `TX_DATA` |
| cnt_retry = 3 | `ERROR_RETRY` → `SLEEP` |
| [Bat ≤ 3.3V] | cualquier estado → `SLEEP` |

---

## 6. Diseño de Hardware

### Componentes seleccionados

| Bloque | Número de parte |
|--------|----------------|
| MCU / control | Adafruit Feather HUZZAH32 ESP32 |
| Transceptor LoRa | HopeRF RFM95W-868S2 |
| Sensor ambiente | Sensirion SHT31-DIS |
| MOSFET de control | BSS138 |
| Medición batería | R3 100 kΩ + R4 100 kΩ |
| Indicador | LED + R5 330 Ω |

### Criterios de implementación

- Todo el sistema opera a **3.3 V** → no se requieren level shifters.
- El bloque **EN_SENS + BSS138** aísla la carga del sensado durante el sleep.
- Los pull-up de I²C se agregan externamente con **R1 y R2 de 4.7 kΩ**.

---

## 7. Modelo de Consumo Energético

### Componentes y corrientes adoptadas

| Bloque | Número de parte | Estado | Corriente |
|--------|----------------|--------|-----------|
| MCU | Adafruit Feather HUZZAH32 ESP32 | Deep sleep | **0.10 mA** |
| MCU | Adafruit Feather HUZZAH32 ESP32 | Activo sin Wi-Fi | **25 mA** |
| LoRa | HopeRF RFM95W-868S2 | Sleep / retención | **0.0002 mA** |
| LoRa | HopeRF RFM95W-868S2 | RX | **10.3 mA** |
| LoRa | HopeRF RFM95W-868S2 | TX +13 dBm | **28 mA** |
| LoRa | HopeRF RFM95W-868S2 | TX +17 dBm | **90 mA** |
| Sensor ambiente | Sensirion SHT31-DIS | Single-shot | **1.5 mA máx** |
| Monitoreo batería | R3 100 kΩ + R4 100 kΩ | Divisor activo | **0.021 mA** |
| Indicador | LED + R5 330 Ω | Durante TX | **~4 mA** |

### Modelo de cálculo

Ciclo periódico de **600 s** (envío cada 10 minutos):

```
Iavg = (Isleep·tsleep + Isense·tsense + Itx·ttx + Irx·tack) / T
```

| Parámetro | Valor |
|-----------|-------|
| T | 600 s |
| t_sleep | 598.3 s |
| t_sense | 1.0 s |
| t_tx | 0.2 s |
| t_ack | 0.5 s |

### Corrientes por fase

| Fase | Corriente total |
|------|----------------|
| Sleep (ESP32 + divisor + radio dormido) | ≈ 0.121 mA |
| Sensado (ESP32 activo + SHT31) | ≈ 26.5 mA |
| RX / ACK | ≈ 35.3 mA |
| TX +13 dBm | ≈ 57 mA |
| TX +17 dBm | ≈ 119 mA |

---

## 8. Resultados y Autonomía

### Resultados

| Caso | I_avg | Autonomía (batería 1000 mAh) |
|------|-------|------------------------------|
| TX +13 dBm | **0.213 mA** | **≈ 195 días** |
| TX +17 dBm | **0.234 mA** | **≈ 178 días** |

### Conclusiones

- El diseño cumple el objetivo de operar la mayor parte del tiempo en ultrabajo consumo.
- El pico instantáneo está en la transmisión LoRa; la potencia TX debe justificarse según el alcance real requerido.
- El promedio sigue siendo bajo porque el sistema se encuentra en sleep más del **99 %** del ciclo.
- No se requieren level shifters: el ESP32 Feather y el RFM95W trabajan a **3.3 V**.
- El MOSFET **BSS138** actúa como mecanismo de aislamiento del bloque de sensado durante el sueño.
- Pendiente: agregar el consumo real del sensor de humedad de suelo cuando se defina el modelo final.

---

## Referencias

- [Adafruit HUZZAH32 ESP32 Feather](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-huzzah32-esp32-feather.pdf)
- [HopeRF RFM95W Datasheet](https://www.mouser.com/datasheet/2/975/1463993415RFM95_96_97_98W-1858106.pdf)
- [Sensirion SHT3x-DIS Datasheet](https://sensirion.com/media/documents/213E6A3B/63A5A569/Datasheet_SHT3x_DIS.pdf)
- [ESP32 Series Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)

---

*AGRO-LoRa · Nivel 5 · Taller Integrador I — Escuela de Ingeniería Electrónica, ITCR · 2026*
