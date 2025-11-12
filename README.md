# Control PID y HMI con PLC Controllino Mega
Repositorio académico para la asignatura de **Control Digital**, de la carrera de Ingeniería en Telecomunicaciones de la **Universidad de Cuenca**. Este proyecto documenta la implementación de tres prácticas que escalan en complejidad, desde el manejo básico de salidas digitales hasta un controlador PID en lazo cerrado.

**Autores:**
* Angel Apolo (`angel.apolo@ucuenca.edu.ec`)
* Stalyn Ochoa (`stalyn.ochoa@ucuenca.edu.ec`)

---

## Hardware y Software Utilizado
* **PLC:** Controllino Mega
* **HMI:** Stone STWC050WT-01
* **Planta:** Entrenador de Planta de Control (EPC) con Motor DC y Encoder.
* **Software:** Arduino IDE (C++), Stone Designer GUI.

---

## Resumen del Proyecto
Este repositorio contiene el código fuente y la documentación de tres laboratorios que demuestran los siguientes conceptos:

### 1. Práctica 4.1: Manejo de Salidas Digitales y Punteros
El primer laboratorio se centró en el control secuencial de una matriz de 9 LEDs (3x3) para generar un patrón en espiral.
* **Conceptos Clave:**
    * Uso de las variables predefinidas de la librería `Controllino.h`.
    * Implementación de **temporización no bloqueante** (basada en `millis()`) para evitar el uso de `delay()`.
    * Uso de **aritmética de punteros** para gestionar eficientemente el estado de la secuencia y el reinicio del ciclo.

### 2. Práctica 4.3: Integración de HMI y Control Híbrido
El segundo laboratorio introdujo la HMI Stone para el control de brillo (PWM) de dos LEDs.
* **Conceptos Clave:**
    * **Diseño de GUI** en Stone Designer para crear widgets `SpinBox` que envían valores (0-100) vía serial.
    * **Control Híbrido:** El encendido/apagado general de cada LED se gestiona mediante **botones físicos** (entradas digitales) con detección de flanco de subida.
    * **Lógica Combinada:** El valor PWM de la HMI solo se aplica si el LED ha sido habilitado por su botón físico correspondiente.

### 3. Práctica 4.4: Controlador PID de Velocidad en Lazo Cerrado
El laboratorio final integra todos los conceptos para implementar un controlador PID que regula la velocidad (RPM) de un motor DC.
* **Conceptos Clave:**
    * **HMI como Panel de Operador:** La GUI permite establecer el \textit{Setpoint} (Referencia de RPM) y realizar la **sintonización en tiempo real** de las constantes $K_p$, $K_i$, y $K_d$.
    * **Adquisición de Datos:** Se usa una **interrupción externa** (`attachInterrupt`) para el conteo preciso de pulsos del encoder.
    * **Ejecución Determinística:** El algoritmo **PID incremental** se ejecuta dentro de una **interrupción de Timer** (Timer1) para garantizar un tiempo de muestreo ($T_s$) constante de 50 ms.
    * **Visualización:** La HMI grafica en tiempo real la Referencia, la Velocidad Medida (RPM) y la Señal de Control (Salida PID).

---

## Demostración en Video

* **[Video] Práctica 4.4: Demostración del Controlador PID:** [ENLACE A TU VIDEO DE YOUTUBE AQUÍ]
* **[Video] Práctica 4.3: Control HMI y Botones Físicos:** [ENLACE A TU VIDEO DE YOUTUBE AQUÍ]
* **[Video] Práctica 4.1: Secuencia de LEDs con Punteros:** [ENLACE A TU VIDEO DE YOUTUBE AQUÍ]
