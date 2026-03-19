# 5. Avances del Proyecto

En este apartado se resumen los principales avances alcanzados hasta el momento en el desarrollo del proyecto **Firmware de iGate APRS basado en tecnología LoRa**, correspondiente al **Grupo 1** del curso **Taller Integrador**.

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

- Estado de espera del módulo
![Estado de espera](/doc/Espera_de_recepcion.jpeg)

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

Este diagrama corresponde a una versión refinada basada en la comprensión adquirida durante las pruebas y el análisis del sistema.

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

Este diagrama constituye una base sólida para la implementación posterior de la máquina de estados y del firmware propio.
 ![Diagrama de Bloques](/doc/Diagrama_de_bloques.jpeg)


---

## Avance 4: Definición del flujo principal del sistema

A partir del diagrama de bloques desarrollado, se estableció el flujo principal de operación del firmware, el cual se describe de la siguiente manera:

**Configuración → Inicialización → Recepción LoRa → Procesamiento → Formateo APRS → Buffer → Envío por WiFi/TCP → APRS-IS**

Este flujo representa la secuencia de procesamiento de la información dentro del sistema y permite visualizar claramente la transformación de los datos desde su recepción hasta su publicación en la red APRS.

---

➡️ **Siguiente sección:** [Arquitectura del Sistema](/doc/03_arquitectura.md)
