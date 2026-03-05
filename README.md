# Sistema de Comunicaciones LoRa/APRS  
## Definición de Alcance, Fundamentación Teórica y Marco Regulatorio en Costa Rica

**Autores:**  
Randy Fernández Aguilar  
Luis Diego García Rojas  

**Afiliación:**  
Escuela de Ingeniería Electrónica,  
Instituto Tecnológico de Costa Rica (ITCR), 30101 Cartago, Costa Rica

---

## Resumen

Este documento presenta el alcance y la investigación inicial para el desarrollo de un sistema de comunicaciones basado en tecnologías LoRa y APRS. Se establece la fundamentación teórica, arquitectura del sistema, parámetros físicos de operación y el análisis de la legislación costarricense aplicable.

**Palabras clave:** LoRa, APRS, LPWAN, Comunicaciones Inalámbricas, Regulación del Espectro, Costa Rica.

---

## Objetivos

### Objetivo General

Desarrollar el firmware de un iGate APRS basado en tecnología LoRa capaz de recibir paquetes de radio provenientes de nodos remotos y retransmitirlos hacia la red APRS-IS mediante conexión a Internet, cumpliendo con las condiciones técnicas y regulatorias establecidas para el uso del espectro en Costa Rica.

### Objetivos Específicos

- Investigar los fundamentos teóricos de los sistemas de comunicación APRS y de la tecnología LoRa, incluyendo su arquitectura de red y parámetros físicos de operación.
- Analizar la normativa costarricense relacionada con el uso del espectro radioeléctrico, con el fin de identificar las bandas de operación permitidas y sus restricciones técnicas.
- Implementar un firmware capaz de recibir tramas LoRa provenientes de nodos remotos, procesarlas y reenviarlas hacia un servidor APRS mediante conexión a Internet.

---

## Introducción

El crecimiento de aplicaciones de telemetría y monitoreo remoto ha impulsado el desarrollo de tecnologías de comunicación inalámbrica capaces de operar a largas distancias con bajo consumo energético. Entre estas tecnologías destaca LoRa (Long Range), una modulación basada en Chirp Spread Spectrum diseñada para redes de área amplia de baja potencia (LPWAN). Por otra parte, el sistema Automatic Packet Reporting System (APRS) es ampliamente utilizado en el servicio de radioaficionados para transmitir información digital como posición geográfica, mensajes cortos y datos de sensores en tiempo real.

La integración de tecnologías LoRa con la infraestructura APRS permite extender las capacidades de monitoreo y telemetría a largas distancias utilizando dispositivos embebidos de bajo consumo. El *iGate* cumple un rol fundamental dentro de la red APRS, ya que permite recibir paquetes transmitidos por radio y reenviarlos hacia la red global APRS-IS mediante conexión a Internet.

El informe se centrará en el desarrollo e investigación de un firmware para un *iGate* APRS basado en tecnología LoRa, capaz de recibir tramas provenientes de nodos remotos y reenviarlas a la red APRS-IS. Este firmware permitirá validar la integración entre comunicaciones de radio de largo alcance y plataformas de visualización en línea.

---

## Fundamentación Teórica

### APRS (Automatic Packet Reporting System)

El Automatic Packet Reporting System (APRS) es un sistema digital de comunicación utilizado en el servicio de radioaficionados que permite el intercambio en tiempo real de información como posición geográfica, mensajes cortos, telemetría, datos meteorológicos y estado de estaciones [1]. Fue desarrollado por Bob Bruninga, WB4APR, y opera principalmente sobre redes VHF utilizando el protocolo AX.25 para la transmisión de paquetes digitales.

Entre las principales aplicaciones del sistema APRS se encuentran:

- Rastreo de vehículos y estaciones móviles mediante GPS.
- Monitoreo meteorológico en tiempo real.
- Comunicación de mensajes cortos entre radioaficionados.
- Coordinación en situaciones de emergencia.
- Telemetría de sensores remotos.

APRS utiliza principalmente el protocolo AX.25 para la transmisión de tramas digitales sobre radio. En VHF se emplea modulación AFSK a 1200 bps, donde los tonos típicos son 1200 Hz y 2200 Hz. La estructura de trama incluye:

- Indicativo de origen
- Indicativo de destino
- Ruta de digipeaters
- Campo de información
- FCS (Frame Check Sequence)

#### Bandas de Operación

APRS opera en la banda VHF asignada al servicio de radioaficionados. En América, la frecuencia estándar es **144.390 MHz**. También puede operar en UHF o en otras bandas asignadas a radioaficionados según la regulación nacional vigente.

#### Componentes de una Red APRS

Una red APRS está compuesta por:

- **Nodo:** Estación transmisora que genera información APRS.
- **Digipeater:** Repetidor digital que retransmite paquetes para ampliar cobertura.
- **iGate:** Puente entre red de radio y red APRS-IS en Internet.
- **Servidor APRS-IS:** Infraestructura global de distribución de paquetes.
- **Cliente final:** Software o plataforma de visualización.

---

### LoRa (Long Range)

#### ¿Qué es LoRa?

LoRa (Long Range) es una tecnología de modulación inalámbrica de capa física (PHY) patentada por la empresa Semtech, diseñada específicamente para redes de área amplia y baja potencia (LPWAN) [2], [3]. A diferencia de modulaciones tradicionales como FSK o PSK, LoRa utiliza la modulación de espectro ensanchado por chirrido (CSS - Chirp Spread Spectrum) [2]. En esta técnica, la señal varía linealmente su frecuencia a lo largo del tiempo (*chirps*), lo que le otorga una alta resistencia al ruido, mitigación del efecto Doppler y la capacidad de decodificar señales incluso por debajo del nivel de ruido base [3], [4]. Es ideal para enviar tramas pequeñas a largas distancias, característica clave para sistemas de posicionamiento o de telemetría intermitente.

#### Frecuencias de Operación

LoRa opera de manera predominante en bandas ISM (Industrial, Científica y Médica), las cuales típicamente se utilizan bajo regímenes de baja potencia y/o uso libre según la normativa del país. Las frecuencias de operación dependen de la ubicación geográfica:

- **Europa:** 868 MHz y 433 MHz [5].
- **Norteamérica y parte de América Latina:** 915 MHz (operando típicamente dentro de 902–928 MHz) [5].

En Costa Rica, el PNAF (Decreto Ejecutivo N.º 44010-MICITT) incluye un apéndice de *bandas de uso libre* y una tabla de potencias máximas permitidas que habilita el rango **902 a 940 MHz** para tecnologías digitales de baja potencia, con potencia máxima de salida de **24 dBm** y PIRE máxima de **30 dBm** [6].

#### Aplicaciones

Por su arquitectura orientada a la eficiencia energética y la robustez del enlace a largo alcance, LoRa es una de las tecnologías más empleadas para sistemas embebidos orientados al Internet de las Cosas (IoT) [7]. Sus aplicaciones principales incluyen:

- **Telemetría y rastreo:** balizas de ubicación, sensores remotos e instrumentación.
- **Medición y Monitoreo Inteligente:** monitoreo urbano y medición distribuida [7], [8].
- **Agricultura de Precisión e Industria:** telemetría en grandes extensiones [8].
- **Ciudades Inteligentes:** estacionamientos, iluminación y detección de eventos [9].

#### Disponibilidad de Módulos Comerciales

A nivel comercial e investigativo, existe una amplia disponibilidad de placas de desarrollo que combinan un microcontrolador ESP32 (con conectividad Wi-Fi) con un transceptor LoRa (como el SX1276/SX1278) [7], [9]. Dos familias comunes son **Heltec (WiFi LoRa 32)** y **LilyGO/TTGO (LoRa32)**, ampliamente usadas en prototipado y proyectos académicos.

---

### Arquitectura de Red LoRa/APRS

El sistema de comunicaciones propuesto se fundamenta en una topología que permite recolectar datos de telemetría/posicionamiento desde dispositivos remotos para su posterior enrutamiento hacia plataformas de visualización en internet [10]. A continuación, se describen los componentes estructurales:

#### Nodo (Tracker)

El nodo o *tracker* es el dispositivo terminal encargado de adquirir parámetros físicos (como coordenadas) y transmitirlos por radio mediante LoRa [10], [11].

#### Digipeater (Repetidor Digital)

El *digipeater* recibe paquetes válidos y los retransmite para ampliar cobertura [12].

#### iGate (Internet Gateway)

El iGate actúa como puente entre el tráfico físico por radio (LoRa) y la infraestructura global APRS-IS [10], [11].

#### Gateway LoRa

En LoRaWAN un *gateway* es multicanal y atiende muchos nodos; en redes APRS simplificadas la recepción física y el enrutamiento lógico suelen converger en un solo equipo [13], [12].

#### Servidor

Entidad de *backend* para centralizar, procesar y almacenar tráfico proveniente de iGates; puede implementarse con software especializado como APRS Trackdirect [10], [13].

#### Cliente Final

Interfaces (web/móvil) para visualizar datos en plataformas como `aprs.fi` o `aprsdirect.de` [10], [12].

#### Diagrama de Red

![Diagrama de red LoRa/APRS](Diagrama.png)

---

### Capa Física y de Enlace (Modelo OSI)

La modulación LoRa opera en la **Capa 1 del modelo OSI**. Para el establecimiento del enlace entre nodos e iGate, se consideran:

#### Spreading Factor (SF)

El *Spreading Factor* determina la duración temporal de cada símbolo. A mayor SF, mayor sensibilidad y alcance, pero menor tasa de datos [14].

#### Bandwidth (BW)

El ancho de banda define el rango espectral del *chirp* (125/250/500 kHz típicos). BW mayor aumenta tasa, pero reduce sensibilidad [14].

#### Coding Rate (CR)

La tasa de codificación implementa FEC, aumentando robustez a costa de mayor tiempo en aire [14].

#### Sync Word

Identificador de red; valores comunes `0x34` (público) y `0x12` (privado) [15], [16].

#### Adaptive Data Rate (ADR)

El ADR ajusta SF y potencia según calidad del enlace para mejorar eficiencia espectral y energética [17].

---

## Marco Regulatorio en Costa Rica

### Cuadro Nacional de Atribución de Frecuencias (PNAF)

El despliegue de sistemas de radiocomunicación en Costa Rica está regulado por el Estado para asegurar el uso eficiente del espectro y prevenir interferencias. El instrumento normativo central es el Plan Nacional de Atribución de Frecuencias (PNAF), actualizado mediante el Decreto Ejecutivo N.º 44010-MICITT, publicado en el Alcance N.º 99 a La Gaceta N.º 95 del 30 de mayo de 2023 [6]. El PNAF adopta el marco del Reglamento de Radiocomunicaciones de la UIT y establece, para cada segmento del espectro, los servicios atribuidos (por ejemplo: fijo, móvil, aficionados) y notas nacionales aplicables.

### Bandas de Frecuencia Aplicables al Sistema LoRa/APRS

El sistema LoRa/APRS puede operar, en Costa Rica, bajo dos escenarios regulatorios principales:

#### Escenario A: Banda de radioaficionados (VHF 144–148 MHz)

El PNAF atribuye los segmentos **144–146 MHz** y **146–148 MHz** al servicio de **Aficionados** (y, donde aplica, aficionados por satélite), por lo que el uso de APRS clásico en **144.390 MHz** se enmarca dentro del servicio de radioaficionados y requiere que la estación operadora cuente con habilitación/licencia correspondiente [6], [18].

#### Escenario B: Bandas de uso libre para tecnologías digitales (902–940 MHz)

El PNAF incorpora un *Apéndice de bandas de uso libre*, definiendo condiciones técnicas y la obligatoriedad de homologación previa de equipos. En particular, el rango **902–940 MHz** se incluye como banda de uso libre para tecnologías digitales, con límites máximos de potencia de salida y PIRE (EIRP) [6].

### Resumen normativo del PNAF para el proyecto

La siguiente tabla resume los segmentos más relevantes para el sistema propuesto, separando la atribución de servicios y las condiciones de uso de libre.

| Banda | Uso | Servicio en PNAF | Condición |
|---|---|---|---|
| 144–148 MHz | APRS VHF | Aficionados | Licencia |
| 902–940 MHz | LoRa/LPWAN | Uso libre | Límites |

### Clases de Emisión

El PNAF utiliza la designación de la UIT para describir emisiones radioeléctricas. Una clase de emisión caracteriza:

1. Tipo de modulación de la portadora  
2. Naturaleza de la señal modulante  
3. Tipo de información transmitida  

Para APRS clásico en VHF (AFSK 1200 bps) se utilizan designaciones del tipo **F2D**. Para LoRa (CSS) la clasificación depende del ancho de banda ocupado y del esquema de modulación digital, por lo que se reporta como una emisión digital en canal único (telemetría/datos), típicamente en familias **F1D/F7W** según el ancho de banda y parámetros de modulación [19].

### P.I.R.E Permitida (EIRP) para bandas de uso libre

El Apéndice V del PNAF define límites máximos por banda para equipos de baja potencia. En particular, para **902 a 940 MHz** se especifica **potencia máxima de salida de 24 dBm** y **PIRE máxima de 30 dBm**, lo cual condiciona el diseño de hardware (PA, antena y ganancia) y la configuración del enlace LoRa [6].

### Permisos y Trámites Requeridos

La operación depende de la banda seleccionada:

#### Bandas de uso libre (por ejemplo, 902–940 MHz)

No requieren concesión individual; sin embargo, el PNAF establece que previo a la utilización de bandas de uso libre debe realizarse el procedimiento de **homologación** del equipo ante SUTEL [6], [20]. Este trámite verifica cumplimiento de límites técnicos (potencia, PIRE, máscaras espectrales y requisitos aplicables).

#### Bandas de radioaficionados (por ejemplo, 144–148 MHz)

Requieren **licencia/permiso de operación** del servicio de radioaficionados y cumplimiento de condiciones asociadas (indicativo, responsabilidades operativas y compatibilidad electromagnética) [6], [18].

### Mapa de Tiempos de Trámites Regulatorios

Con el objetivo de evaluar la viabilidad administrativa del despliegue del sistema LoRa/APRS, se presenta un análisis comparativo de los trámites requeridos según la banda de operación seleccionada, conforme al PNAF y la normativa vigente [6], [20], [18].

#### Escenario A: Banda de Uso Libre (902–940 MHz)

En este escenario no se requiere concesión individual del espectro; sin embargo, el PNAF establece la obligatoriedad de homologar previamente el equipo ante la Superintendencia de Telecomunicaciones (SUTEL).

| Instancia | Trámite | Tiempo | Resultado |
|---|---|---|---|
| SUTEL | Homologación de equipo (uso libre) | ≤ 10 días hábiles | Resolución |

El proceso consiste en la presentación de documentación técnica del dispositivo (especificaciones, potencia de salida, certificaciones internacionales) a través de la plataforma oficial de homologación. Una vez aprobado, el equipo puede operar dentro de los límites técnicos establecidos (24 dBm potencia de salida y 30 dBm PIRE).

#### Escenario B: Banda de Radioaficionados (144–148 MHz)

En este caso se requiere licencia de radioaficionado y autorización de uso del espectro.

| Instancia | Trámite | Tiempo | Resultado |
|---|---|---|---|
| SUTEL | Evaluación / criterio técnico | Variable | Dictamen |
| MICITT | Emisión de licencia / autorización | 3–6 meses (aprox.) | Licencia |

El proceso incluye la acreditación como radioaficionado (según categoría), evaluación técnica y resolución ejecutiva. Debido a la intervención de múltiples instancias administrativas, el tiempo total del trámite es significativamente mayor que en el régimen de uso libre.

#### Análisis Comparativo

Desde el punto de vista administrativo y temporal, la operación en la banda **902–940 MHz** bajo régimen de uso libre representa la alternativa más viable para un proyecto académico de duración semestral, debido a la simplicidad del trámite y su corto plazo de resolución.

---

## Conclusiones

En el informe se realizó una investigación inicial sobre los fundamentos de los sistemas de comunicación APRS y la tecnología LoRa, analizando su funcionamiento, arquitectura de red y principales parámetros de operación. A partir de esta investigación, se identificó que la combinación de ambas tecnologías permite implementar sistemas de telemetría y monitoreo remoto con bajo consumo energético y gran alcance de comunicación.

Asimismo, se estudió la legislación costarricense relacionada con el uso del espectro radioeléctrico, particularmente el Plan Nacional de Atribución de Frecuencias (PNAF). Esto permitió identificar las bandas disponibles para la implementación del sistema propuesto, así como las restricciones técnicas asociadas a potencia de transmisión y P.I.R.E. permitida.

A partir de la arquitectura de red analizada, se determinó que el componente *iGate* desempeña un papel fundamental dentro del sistema APRS, al actuar como puente entre la red de radio y la infraestructura de servidores en Internet.

---
