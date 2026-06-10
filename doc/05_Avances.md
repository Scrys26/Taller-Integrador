# 5. Avances del Proyecto

En este apartado se resumen los principales avances alcanzados durante el semestre en el desarrollo del proyecto **Firmware de iGate APRS basado en tecnología LoRa**, correspondiente al **Grupo 1** del curso **Taller Integrador**.

---

## Avance 1: Validación del firmware de referencia

Como primera etapa práctica del proyecto, se utilizó un firmware de referencia desarrollado por Richard Guzmán, con el objetivo de validar el funcionamiento del módulo iGate y familiarizarse con su operación.

Inicialmente, se realizó el proceso de carga (*flashing*) del firmware en el módulo basado en **ESP32**, seguido de la configuración de los parámetros necesarios para su funcionamiento dentro de la red.

El dispositivo fue configurado en modo **receptor y transmisor**, permitiendo su operación continua dentro del sistema. Una vez configurado, el iGate comenzó a recibir paquetes provenientes de otros dispositivos activos en la red LoRa/APRS.

Durante esta etapa se logró:

- Verificar la correcta recepción de paquetes LoRa.
- Observar el comportamiento del sistema en estado de espera y recepción.
- Confirmar la transmisión de datos hacia la red.
- Validar la conectividad del dispositivo con la infraestructura APRS.

Adicionalmente, se comprobó el funcionamiento del sistema mediante la plataforma **APRS**, donde fue posible visualizar el dispositivo en línea y monitorear la actividad de la red.

A continuación, se presentan evidencias del funcionamiento del sistema:

- Recepción de paquetes LoRa
![Estado de recepción](/doc/Recibiendo.jpeg)

---

- Estado de espera del módulo
![Estado de espera](/doc/Espera_de_recepcion.jpeg)

---

- Visualización del dispositivo en la red APRS
![Red APRS](/doc/APRS.jpeg)

---

## Avance 2: Análisis del firmware de referencia

A partir de la experiencia obtenida durante las pruebas prácticas, se realizó un análisis del firmware de referencia con el fin de identificar los módulos funcionales más relevantes para el diseño del sistema propio.

Se identificaron los siguientes bloques fundamentales:

- Recepción de paquetes **LoRa**
- Configuración del sistema
- Procesamiento y validación de paquetes
- Formateo de tramas para **APRS**
- Gestión de conexión **WiFi/TCP**
- Manejo de errores y reconexión
- Envío de información hacia servidores **APRS-IS**

Este análisis permitió comprender la arquitectura general de un iGate y sirvió como base para el diseño del firmware del proyecto.

---

## Avance 3: Diseño del diagrama de bloques del firmware

Posterior a la validación y análisis del sistema de referencia, se procedió con el diseño del **diagrama de bloques del firmware**, el cual representa la arquitectura propuesta para el desarrollo del iGate del grupo.

El diseño contempla tres capas principales:

### Capa de entrada
Encargada de la recepción de paquetes por medio del módulo **LoRa**.

### Núcleo del firmware
Incluye los bloques de:

- Configuración del sistema
- Inicialización del ESP32
- Procesamiento de paquetes
- Formateador APRS
- Buffer o cola de transmisión
- Manejo de errores y monitoreo

### Capa de salida
Responsable de la conexión **WiFi/TCP** y del envío de paquetes hacia los servidores **APRS-IS**.

![Diagrama de Bloques](/doc/Diagrama_de_bloques.jpeg)

---

## Avance 4: Definición del flujo principal del sistema

A partir del diagrama de bloques desarrollado, se estableció el flujo principal de operación del firmware:

**Configuración → Inicialización → Recepción LoRa → Procesamiento → Formateo APRS → Buffer → Envío por WiFi/TCP → APRS-IS**

Este flujo representa la secuencia de procesamiento de la información dentro del sistema y permite visualizar claramente la transformación de los datos desde su recepción hasta su publicación en la red APRS.

---

## Avance 5: Diseño de la máquina de estados del firmware

Con base en el diagrama de bloques, se desarrolló una máquina de estados que describe el comportamiento dinámico del firmware del iGate.

Los estados principales del sistema son:

- **START:** Estado inicial del sistema.
- **LISTEN:** Estado de espera y recepción de paquetes LoRa.
- **DECODE:** Procesamiento y validación de las tramas recibidas.
- **UPLINK:** Envío de paquetes hacia la red APRS-IS mediante conexión WiFi.
- **BEACON_TX:** Transmisión periódica de beacons del sistema.
- **ERROR:** Manejo de fallos y recuperación del sistema.

El flujo principal sigue la secuencia: **START → LISTEN → DECODE → UPLINK → LISTEN**

![Máquina de Estados](/doc/Maquina_de_Estados.png)

---

## Diagrama de Gantt

![Diagrama de Gantt](/doc/dgantt.png)

---

---

## Avance 6: Implementación del firmware propio

Con base en el diseño de la máquina de estados, se desarrolló el firmware desde cero en **PlatformIO** con el framework Arduino para ESP32. La implementación se organizó en módulos independientes:

| Módulo | Responsabilidad |
|---|---|
| `main.cpp` | Motor de la FSM y coordinación de transiciones |
| `wifi_manager.cpp` | Conexión WiFi con reconexión automática |
| `lora_manager.cpp` | Inicialización SPI y recepción de paquetes |
| `aprs_parser.cpp` | Validación y reformateo de tramas APRS |
| `aprs_is_client.cpp` | Socket TCP, login APRS-IS y lectura de keepalives |
| `beacon.cpp` | Generación del beacon de posición del iGate |
| `display.cpp` | Dashboard en OLED SSD1306 |

El código fuente completo está disponible en el [repositorio del grupo](https://github.com/Scrys26/Taller-Integrador).

---

## Avance 7: Integración LoRa → APRS-IS

Se implementó el flujo completo de procesamiento de tramas: recepción por radio LoRa, limpieza del byte de encabezado `0x3C`, validación del formato `SOURCE>DEST,PATH:PAYLOAD`, y reformateo al estándar APRS-IS con el código `qAR`.

La trama de salida generada por el firmware sigue el formato:

```
SOURCE>APRS,qAR,TI2TEC-10:PAYLOAD\r\n
```

El `qAR` indica que el paquete fue recibido directamente por radio. El payload del tracker se conserva sin modificar.

Adicionalmente, se implementó el beacon periódico del iGate, que publica la posición estática del nodo cada 15 minutos:

```
TI2TEC-10>APRS,TCPIP*:!0951.57N/08354.75W&iGate LoRa-APRS TEC / Proyecto\r\n
```

---

## Avance 8: Manejo de errores y reconexión automática

Una de las mejoras más importantes respecto a la versión inicial fue la implementación de recuperación automática ante fallos de red. El estado `ERROR` ejecuta la siguiente secuencia sin intervención manual:

1. Reintentar conexión WiFi con timeout configurable.
2. Cerrar el socket TCP y reconectar al servidor APRS-IS con login completo.
3. Verificar el estado del chip LoRa y reinicializarlo si es necesario.
4. Si supera el número máximo de intentos (`MAX_ERROR_RETRIES = 5`), ejecutar `ESP.restart()`.

La siguiente figura muestra la evidencia en el monitor serial de un ciclo completo de pérdida de WiFi y recuperación automática:

![Reconexión automática en estado ERROR](/doc/serial_error_reconexion.png)

---

## Avance 9: OLED como dashboard operativo

La pantalla OLED pasó de mostrar únicamente una pantalla de arranque a funcionar como un panel de estado en tiempo real. En cada ciclo de renderizado muestra:

- Nombre de la red WiFi y señal
- Estado de APRS-IS (ON/OFF)
- IP local del ESP32
- Estado del radio LoRa
- Contadores de paquetes recibidos (RX) y enviados (TX)
- Callsign de la última estación recibida
- Barra inferior con el estado actual de la FSM (`LISTENING`, `RX VALID`, `UPLINK`, `ERROR`)

![Hardware del iGate con OLED en operación](/doc/oled_hardware.jpg)

---

## Avance 10: Pruebas con múltiples trackers

Se realizaron pruebas de integración con trackers reales del curso, verificando que el sistema procesara correctamente paquetes de múltiples indicativos en simultáneo.

| Indicativo | RSSI (dBm) | SNR (dB) | Resultado |
|---|---|---|---|
| TI2FRN-10 | -100 | -14.8 | DECODE → UPLINK exitoso |
| TI2JAB-10 | -99 | -16.2 | DECODE → UPLINK exitoso |
| TI2JR-10 | -97 | -12.5 | DECODE → UPLINK exitoso |
| TI3WTI-10 | -100 | -18.0 | DECODE → UPLINK exitoso |

La siguiente figura muestra la evidencia en el monitor serial de la recepción y uplink del paquete de `TI3WTI-10`:

![Monitor serial: recepción y uplink de tracker real](/doc/serial_uplink_real.png)

---

## Avance 11: Validación en la red APRS-IS global

Se verificó la publicación del iGate en la red global **APRS-IS** mediante la plataforma **aprs.fi**. El callsign `TI2TEC-10` aparece visible en el mapa con los metadatos del nodo, confirmando que el beacon está siendo recibido y distribuido correctamente por la red.

![Evidencia de publicación en aprs.fi](/doc/aprsfi_beacon.png)

La siguiente figura muestra el monitor serial durante el estado `BEACON_TX`, con la trama de posición generada y confirmada como enviada:

![Monitor serial: transmisión de beacon](/doc/serial_beacon.jpg)

---

➡️ **Sección anterior:** [Legislación](/doc/04_legislacion.md)  
➡️ **Inicio:** [Introducción](/README.md)
