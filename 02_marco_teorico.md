# 2. Marco Teórico

Este documento describe los fundamentos tecnológicos que sustentan el desarrollo del **iGate LoRa/APRS**, incluyendo las modulaciones utilizadas, los protocolos de comunicación y los parámetros de configuración necesarios para el funcionamiento del sistema.

---

## LoRa

**LoRa (Long Range)** es una tecnología de modulación inalámbrica desarrollada por la empresa Semtech, diseñada para redes de área amplia y baja potencia (**LPWAN — Low Power Wide Area Network**).

A diferencia de modulaciones tradicionales como **FSK** o **PSK**, LoRa utiliza una técnica llamada **Chirp Spread Spectrum (CSS)**.

En esta técnica la frecuencia de la señal varía linealmente a lo largo del tiempo, generando lo que se conoce como **chirps**. Este tipo de modulación proporciona varias ventajas importantes para aplicaciones de Internet de las Cosas (**IoT**):

- **Alta resistencia al ruido:** permite decodificar señales incluso cuando su potencia está por debajo del nivel de ruido.
- **Mitigación del efecto Doppler**, lo que mejora la estabilidad del enlace en condiciones dinámicas.
- **Bajo consumo energético**, ideal para transmitir pequeñas cantidades de datos como telemetría o posicionamiento a largas distancias.

### Frecuencias de operación

LoRa opera principalmente en **bandas ISM (Industrial, Científica y Médica)**, las cuales varían según la región del mundo.

En **Costa Rica**, la normativa permite el uso del rango aproximado de **902 a 940 MHz** para sistemas digitales de baja potencia que operan bajo condiciones de uso libre del espectro.

---

## APRS

El **Automatic Packet Reporting System (APRS)** es un sistema digital de comunicación utilizado principalmente dentro del servicio de radioaficionados.

Su objetivo es permitir el intercambio en tiempo real de información como:

- Posición geográfica
- Telemetría
- Datos meteorológicos
- Mensajes cortos

Tradicionalmente APRS opera en la banda **VHF**, específicamente en **144.390 MHz en América**, utilizando:

- Protocolo **AX.25**
- Modulación **AFSK a 1200 bps**

### Componentes de una red APRS

Una red APRS típica está compuesta por los siguientes elementos:

**Nodo (Tracker)**  
Estación transmisora que genera la información, como coordenadas GPS o datos de sensores.

**Digipeater**  
Repetidor digital que retransmite paquetes recibidos para ampliar la cobertura de la red.

**iGate (Internet Gateway)**  
Dispositivo que actúa como puente entre la red de radiofrecuencia y la red de servidores de Internet.

**Servidor APRS-IS**  
Infraestructura distribuida de servidores que recopila y distribuye paquetes APRS a nivel global.

**Cliente final**  
Aplicaciones o plataformas web utilizadas para visualizar la información transmitida, como `aprs.fi`.

---

## Capa física

El diseño del sistema se enfoca principalmente en la **Capa 1 (Capa Física)** y parcialmente en la **Capa 2 (Enlace de Datos)** del **modelo OSI**.

En el sistema APRS tradicional, la capa física se implementa mediante **modulación AFSK transmitida sobre RF en VHF**.

En nuestro sistema, esta capa se reemplaza por **modulación LoRa basada en Chirp Spread Spectrum**.

Esto implica que el **iGate LoRa** debe ser capaz de:

1. Escuchar el medio físico mediante el receptor LoRa.
2. Decodificar los chirps recibidos.
3. Extraer la carga útil (*payload*) del paquete.
4. Encapsular dicha información para enviarla mediante **TCP/IP hacia la red APRS-IS**.

---

## Parámetros de transmisión

Para establecer y optimizar el enlace LoRa entre los nodos remotos y el iGate, es necesario configurar correctamente varios parámetros del transceptor.

### Spreading Factor (SF)

El **Spreading Factor** determina cuántos chips representan cada símbolo transmitido.

- Un **SF alto (ej. SF12)** aumenta la sensibilidad del receptor y el alcance.
- Reduce la tasa de datos.
- Incrementa el **Time-On-Air** del paquete.

### Bandwidth (BW)

La **Bandwidth** define el ancho de banda ocupado por el chirp.

Valores típicos:

- 125 kHz
- 250 kHz
- 500 kHz

Un mayor ancho de banda permite **mayor velocidad de transmisión**, pero reduce la sensibilidad del receptor.

### Coding Rate (CR)

El **Coding Rate** determina el nivel de **Forward Error Correction (FEC)** aplicado a la transmisión.

- Mayor CR → mayor robustez frente a interferencias
- Aumenta la redundancia del paquete
- Incrementa el tiempo de transmisión

### Sync Word

El **Sync Word** funciona como un identificador de red a nivel de hardware.

Permite que los dispositivos LoRa ignoren paquetes provenientes de otras redes.

Valores comunes:

- `0x34` → redes LoRaWAN públicas
- `0x12` → redes LoRa privadas

### Adaptive Data Rate (ADR)

El **Adaptive Data Rate (ADR)** es un mecanismo que permite ajustar automáticamente parámetros como:

- Spreading Factor
- Potencia de transmisión

Esto se realiza en función de la calidad del enlace, optimizando tanto la **eficiencia espectral** como el **consumo energético** de los nodos.

---

➡️ **Siguiente sección:** [Arquitectura del Sistema](03_arquitectura.md)
