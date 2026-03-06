# 1. Introducción al Proyecto

## Introducción

El crecimiento de aplicaciones de telemetría y monitoreo remoto ha impulsado el desarrollo de tecnologías de comunicación inalámbrica capaces de operar a largas distancias con bajo consumo energético.

Entre estas tecnologías destaca **LoRa (Long Range)**, una técnica de modulación basada en *Chirp Spread Spectrum* diseñada para redes de área amplia de baja potencia (*LPWAN*).

Por otra parte, el **Automatic Packet Reporting System (APRS)** es un sistema ampliamente utilizado en el servicio de radioaficionados para transmitir información digital como posición geográfica, mensajes cortos y datos de sensores en tiempo real.

La integración de tecnologías **LoRa** con la infraestructura **APRS** permite extender las capacidades de monitoreo y telemetría a largas distancias utilizando dispositivos embebidos de bajo consumo.

Dentro de esta arquitectura, el **iGate** cumple un rol fundamental en la red APRS, ya que permite recibir paquetes transmitidos por radio y reenviarlos hacia la red global **APRS-IS** mediante una conexión a Internet.

Este proyecto se centra en el desarrollo e investigación de un firmware para un **iGate APRS basado en tecnología LoRa**, capaz de recibir tramas provenientes de nodos remotos y reenviarlas hacia la red APRS-IS.

---

## Justificación

La integración de tecnologías **LoRa** con la infraestructura de red **APRS** permite extender significativamente las capacidades de monitoreo a largas distancias utilizando dispositivos embebidos de muy bajo consumo.

Al utilizar LoRa como capa física, el sistema puede operar en **bandas ISM de uso libre**, como el rango de **902 a 940 MHz en Costa Rica**. Esto permite un despliegue mucho más sencillo desde el punto de vista administrativo, ya que no requiere concesiones individuales del espectro.

Esta característica facilita la experimentación y validación de sistemas de comunicación de radio de largo alcance con plataformas globales de visualización en línea como **aprs.fi**.

---

## Objetivo General

Desarrollar el **firmware de un iGate APRS basado en tecnología LoRa** capaz de recibir paquetes de radio provenientes de nodos remotos y retransmitirlos hacia la red **APRS-IS** mediante conexión a Internet.

---

## Objetivos Específicos

- **Investigar** los fundamentos teóricos de los sistemas de comunicación **APRS** y **LoRa**.
- **Analizar** la normativa costarricense relacionada con el uso del espectro radioeléctrico (PNAF).
- **Implementar** un firmware capaz de recibir tramas LoRa y reenviarlas hacia un servidor APRS mediante Internet.

---

➡️ **Siguiente sección:** [Marco Teórico](02_marco_teorico.md)
