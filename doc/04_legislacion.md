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

El sistema LoRa/APRS puede operar bajo tres escenarios regulatorios en Costa Rica.

### Escenario A: Banda de Radioaficionados (VHF 144–148 MHz)

El PNAF atribuye los segmentos **144–146 MHz** y **146–148 MHz** al servicio de **Radioaficionados**.

El APRS tradicional utilizado en América opera típicamente en **144.390 MHz**, dentro de esta banda.

**Condiciones regulatorias:**

- El operador debe poseer una **licencia de radioaficionado** emitida por el MICITT.
- El uso del espectro se limita a actividades permitidas dentro del servicio de aficionados.

---

### Escenario B: Banda de Radioaficionados (UHF 430–440 MHz) — Frecuencia utilizada en este proyecto

El PNAF atribuye el segmento **430–440 MHz** al servicio de **Radioaficionados (secundario)**, en coexistencia con el servicio fijo y el servicio móvil.

El firmware desarrollado opera en **433.775 MHz**, que es la frecuencia estándar de la comunidad **LoRa-APRS** para la banda de 70 cm. Esta frecuencia fue configurada en el parámetro `LORA_FREQUENCY` del archivo `config.h` y corresponde al sync word `0x12`, identificador de la red LoRa-APRS.

**Condiciones regulatorias:**

- El operador debe poseer una **licencia de radioaficionado** emitida por el MICITT.
- La potencia de transmisión y la P.I.R.E. deben respetar los límites del servicio de aficionados.
- Para uso académico experimental sin transmisión al aire (solo recepción en el iGate), el requisito de licencia aplica principalmente a los trackers transmisores.

---

### Escenario C: Banda de Uso Libre (902–940 MHz)

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
| **430–440 MHz** | **LoRa-APRS 433.775 MHz (este proyecto)** | Servicio de radioaficionados (secundario) | Licencia MICITT |
| **902–940 MHz** | LoRa / LPWAN alternativo | Banda de uso libre | Homologación SUTEL |

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

Clasificación comúnmente asociada a modulaciones digitales complejas que combinan técnicas de modulación y codificación de espectro ensanchado, como es el caso de **LoRa (Chirp Spread Spectrum)** en 433.775 MHz. Esta es la clase de emisión que corresponde al sistema implementado en este proyecto.

Dependiendo de la configuración específica del transceptor, también puede clasificarse dentro de la familia **F1D**, utilizada para transmisiones digitales de datos en canal único.

---

## P.I.R.E. Permitida (EIRP)

### Banda 430–440 MHz (utilizada en este proyecto)

Para el servicio de radioaficionados en la banda de 70 cm, los límites de potencia dependen de la licencia habilitante. En Costa Rica, el MICITT establece las potencias máximas permitidas por categoría de licencia. El transceptor SX1276 utilizado en el prototipo opera con una potencia de salida configurable de hasta **20 dBm (100 mW)**, valor que se encuentra dentro de los rangos típicamente autorizados para radioaficionados.

### Banda 902–940 MHz (uso libre)

Para operar dentro del escenario de **uso libre en la banda 902–940 MHz**, el PNAF establece límites técnicos para los dispositivos de baja potencia:

- **Potencia máxima de salida:** 24 dBm  
- **P.I.R.E. máxima permitida:** 30 dBm

El diseño del sistema debe garantizar que la combinación de potencia del transmisor, pérdidas en el cable y ganancia de la antena no supere estos valores.

---

## Permisos y Tiempos de Trámite

Desde el punto de vista administrativo, la banda **902–940 MHz** representa la opción más viable para proyectos que busquen evitar el proceso de habilitación de radioaficionados. Sin embargo, dado que el firmware desarrollado opera en **433.775 MHz** dentro de la banda de aficionados, el marco regulatorio aplicable es el de la licencia MICITT.

| Escenario | Institución | Trámite | Tiempo estimado | Resultado |
|---|---|---|---|---|
| **Radioaficionados (433/144 MHz)** | MICITT / SUTEL | Licencia de radioaficionado | 3 a 6 meses aprox. | Licencia de operación |
| **Uso libre (900 MHz)** | SUTEL | Homologación del equipo | ≤ 10 días hábiles | Resolución de homologación |

*Nota:* El proceso de **homologación ante SUTEL** requiere presentar documentación técnica del equipo, como la hoja de datos (*datasheet*) del transceptor utilizado (por ejemplo **SX1276**), demostrando que cumple con los límites de potencia establecidos por la normativa.

---

➡️ **Sección anterior:** [Arquitectura del Sistema](/doc/03_arquitectura.md)  

➡️ **Siguiente sección:** [Avances del Proyecto](/doc/05_Avances.md)
