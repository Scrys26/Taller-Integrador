# 4. Legislación y Marco Regulatorio en Costa Rica

El despliegue de sistemas de radiocomunicación en Costa Rica está regulado por el Estado con el objetivo de garantizar el uso eficiente del espectro radioeléctrico y prevenir interferencias entre servicios.

Esta sección resume los lineamientos legales y técnicos aplicables al desarrollo del sistema **iGate LoRa/APRS**.

---

## Referencia al Decreto MICITT (PNAF)

El instrumento normativo principal que rige el uso del espectro en Costa Rica es el **Plan Nacional de Atribución de Frecuencias (PNAF)**.

La versión vigente utilizada como referencia en este proyecto corresponde al **Decreto Ejecutivo N.º 44010-MICITT**, publicado en el Alcance N.º 99 a La Gaceta N.º 95 del 30 de mayo de 2023.

Este decreto adopta el marco regulatorio establecido por el **Reglamento de Radiocomunicaciones de la Unión Internacional de Telecomunicaciones (UIT)** y define los servicios atribuidos y las notas nacionales aplicables a cada segmento del espectro radioeléctrico.

---

## Bandas de Frecuencia Aplicables

El sistema LoRa/APRS puede operar bajo dos escenarios regulatorios principales en Costa Rica.

### Escenario A: Banda de Radioaficionados (VHF 144–148 MHz)

El PNAF atribuye los segmentos **144–146 MHz** y **146–148 MHz** al servicio de **Radioaficionados**.

El APRS tradicional utilizado en América opera típicamente en **144.390 MHz**, dentro de esta banda.

**Condiciones regulatorias:**

- El operador debe poseer una **licencia de radioaficionado** emitida por el MICITT.
- El uso del espectro se limita a actividades permitidas dentro del servicio de aficionados.

---

### Escenario B: Banda de Uso Libre (902–940 MHz) — Recomendado

El PNAF incluye un **apéndice de bandas de uso libre** destinadas a tecnologías digitales de baja potencia, comúnmente utilizadas en sistemas **LPWAN**.

Dentro de este marco se habilita el rango aproximado de **902 a 940 MHz**, donde operan diversas tecnologías de comunicación de corto alcance, incluyendo **LoRa**.

**Condiciones regulatorias:**

- No requiere concesión individual del espectro.
- El equipo debe estar **homologado ante la Superintendencia de Telecomunicaciones (SUTEL)**.
- Deben respetarse los límites de potencia y P.I.R.E. establecidos.

---

### Resumen Normativo

| Banda | Uso del Proyecto | Servicio según PNAF | Requisito Legal |
|---|---|---|---|
| **144–148 MHz** | APRS clásico (VHF) | Servicio de radioaficionados | Licencia MICITT |
| **902–940 MHz** | LoRa / LPWAN | Banda de uso libre | Homologación SUTEL |

---

## Clases de Emisión

El **PNAF** utiliza las designaciones de la **UIT** para clasificar las emisiones radioeléctricas según:

- tipo de modulación
- naturaleza de la señal
- tipo de información transmitida

Para el sistema analizado se contemplan las siguientes clases de emisión:

**F2D**

Utilizada en el **APRS tradicional en VHF**, basado en modulación **AFSK a 1200 bps** sobre portadora FM.

**F7W**

Clasificación comúnmente asociada a modulaciones digitales complejas que combinan técnicas de modulación y codificación de espectro ensanchado, como es el caso de **LoRa (Chirp Spread Spectrum)**.

Dependiendo de la configuración específica del transceptor, también puede clasificarse dentro de la familia **F1D**, utilizada para transmisiones digitales de datos en canal único.

---

## P.I.R.E. Permitida (EIRP)

Para operar dentro del escenario de **uso libre en la banda 902–940 MHz**, el PNAF establece límites técnicos para los dispositivos de baja potencia.

Estos límites se expresan en términos de **P.I.R.E. (Potencia Isotrópica Radiada Equivalente)**, conocida internacionalmente como **EIRP (Equivalent Isotropically Radiated Power)**.

El diseño del sistema debe garantizar que la combinación de:

- potencia del transmisor
- pérdidas en el cable
- ganancia de la antena

no supere los siguientes valores:

- **Potencia máxima de salida:** 24 dBm  
- **P.I.R.E. máxima permitida:** 30 dBm

---

## Permisos y Tiempos de Trámite

Desde el punto de vista administrativo y operativo, la banda **902–940 MHz** representa la opción más viable para proyectos académicos o experimentales.

El siguiente cuadro resume los procesos regulatorios asociados a cada escenario.

| Escenario | Institución | Trámite | Tiempo estimado | Resultado |
|---|---|---|---|---|
| **Uso libre (900 MHz)** | SUTEL | Homologación del equipo | ≤ 10 días hábiles | Resolución de homologación |
| **Radioaficionados (VHF)** | MICITT / SUTEL | Licencia de radioaficionado | 3 a 6 meses aprox. | Licencia de operación |

*Nota:* El proceso de **homologación ante SUTEL** requiere presentar documentación técnica del equipo, como la hoja de datos (*datasheet*) del transceptor utilizado (por ejemplo **SX1276**), demostrando que cumple con los límites de potencia establecidos por la normativa.

---

➡️ **Sección anterior:** [Arquitectura del Sistema](/doc/03_arquitectura.md)  
➡️ **Inicio:** [Introducción](/README.md)  
