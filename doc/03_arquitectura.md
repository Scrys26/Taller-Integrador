# 3. Arquitectura del Sistema

El sistema de comunicaciones propuesto utiliza una arquitectura que permite recolectar datos de telemetría y posicionamiento desde dispositivos remotos, para posteriormente enviarlos hacia plataformas de visualización globales a través de Internet.

La red combina enlaces de radio basados en **LoRa** con la infraestructura distribuida de **APRS-IS**, permitiendo transportar información desde nodos en campo hasta aplicaciones de visualización en línea.

---

## Diagrama de Red

El siguiente diagrama muestra el flujo general de los datos dentro del sistema, desde los nodos terminales hasta el cliente final.

![Diagrama de red LoRa/APRS](doc/Diagrama.png)

*(Asegúrate de que la ruta de la imagen coincida con la estructura de carpetas del repositorio.)*

---

## Componentes de la Red

### Nodo (Tracker)

Es el dispositivo terminal desplegado en campo.  

Se encarga de adquirir información del entorno mediante sensores o módulos externos (por ejemplo GPS, temperatura o nivel de batería) y transmitirla utilizando modulación **LoRa**.

Este nodo representa la **fuente original de los datos** dentro del sistema.

---

### Digipeater (Repetidor Digital)

El **digipeater** actúa como un repetidor digital dentro de la red de radio.

Su función consiste en:

1. Escuchar el medio de radio.
2. Recibir paquetes válidos provenientes de nodos u otros digipeaters.
3. Retransmitir dichos paquetes para ampliar la cobertura de la red.

Esto permite que nodos ubicados fuera del alcance directo del iGate puedan seguir enviando información.

---

### iGate (Internet Gateway)

El **iGate** es el componente central de este proyecto.

Actúa como un puente entre:

- la **red de radio LoRa**
- la **infraestructura de servidores APRS-IS en Internet**

El iGate recibe paquetes LoRa, los decodifica y los reenvía hacia la red APRS-IS utilizando una conexión **TCP/IP**, normalmente mediante **Wi-Fi o Ethernet**.

---

### Gateway LoRa

En redes comerciales como **LoRaWAN**, un *gateway* es un dispositivo multicanal capaz de recibir transmisiones simultáneas de múltiples nodos.

Sin embargo, en implementaciones experimentales o de bajo costo como la utilizada en este proyecto, las funciones de recepción de RF y enrutamiento de paquetes pueden integrarse en un único dispositivo.

En este caso, el **iGate actúa también como un gateway LoRa monocanal**, simplificando la arquitectura del sistema.

---

### Servidor

Los **servidores APRS-IS** conforman la infraestructura global encargada de recibir, enrutar y distribuir los paquetes provenientes de múltiples iGates alrededor del mundo.

Estos servidores permiten centralizar el tráfico APRS y ponerlo a disposición de diferentes aplicaciones de visualización y análisis.

---

### Cliente Final

El cliente final corresponde a las aplicaciones utilizadas por el usuario para visualizar la información transmitida por los nodos.

Estas plataformas permiten mostrar en tiempo real:

- la posición geográfica de los nodos
- telemetría asociada
- rutas históricas

Ejemplos de clientes ampliamente utilizados son:

- `aprs.fi`
- `aprsdirect.de`

---

➡️ **Sección anterior:** [Marco Teórico](/02_marco_teorico.md)  
➡️ **Siguiente sección:** [Legislación y Regulación del Espectro](/04_legislacion.md)