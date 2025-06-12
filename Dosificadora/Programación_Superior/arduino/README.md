#  Dosificadora Automática con Arduino

Este proyecto implementa una **dosificadora automática de granos** utilizando Arduino Nano, una celda de carga con el módulo HX711, un servomotor y una pantalla LCD I2C. El sistema permite configurar un peso objetivo y dispensar con precisión según ese valor, ya sea desde botones físicos o comandos por puerto serial.

---

## Componentes utilizados

- Arduino Nano
- Celda de carga + Módulo HX711
- Servomotor (SG90 o similar)
- Pantalla LCD 16x2 con I2C
- Pulsadores (3): Subir peso, Bajar peso, Iniciar
- Buzzer (zumbador) y LED (opcional)
- Cables y resistencias

---

## Funcionalidades principales

- Configuración del peso objetivo mediante botones físicos.
- Dispensado automático hasta alcanzar el peso deseado.
- Control de la dosificación mediante una **máquina de estados finitos**.
- Comandos por **puerto serial USB**:
  - `SET:<valor>` → cambia el peso objetivo.
  - `START` → inicia la dosificación.
  - `TARE` → realiza la tara de la balanza.
- Indicaciones sonoras mediante buzzer.
- Interfaz LCD para mostrar el peso actual, objetivo y último valor dispensado.

---

##  Máquina de Estados

- `CONFIGURACION`: estado inicial, espera interacción del usuario.
- `ACTIVAR_MOTOR`: abre el servomotor e inicia el dispensado.
- `CONTROL_CANTIDAD`: monitorea el peso y detiene cuando se alcanza el umbral.
- `FINALIZACION`: cierra el servo, emite señal sonora y actualiza valores.

---

##  Estructura del Código

El código está modularizado y puede organizarse de esta forma:
├── Dispensador.h # Declaraciones de funciones y estados
├── Dispensador.cpp # Lógica principal de la máquina de estados
├── main.ino # Archivo principal que llama a setup y loop del módulo
├── README.md # Este archivo


## Cómo usar

1. Conecta todos los componentes según los pines definidos:
   - HX711 → DOUT = pin 2, CLK = pin 3
   - Botones: UP = pin 4, DOWN = pin 5, START = pin 6
   - Servo = pin 9
   - Buzzer/LED = pin 10

2. Carga el programa en el Arduino.

3. Abre el Monitor Serial a 9600 baudios o dependiendo el sensor que vayas a usar.

4. Usa los botones físicos o los comandos seriales para interactuar:
   - `SET:200` → configura 200g como peso meta.
   - `START` → comienza la dosificación.
   - `TARE` o mantener pulsado el botón de inicio por 2s → realiza tara.

---

## ✨ Características destacadas

- Modular y fácil de expandir.
- Precisión ajustable.
- Compatible con interfaz web u otros módulos como Bluetooth o Wi-Fi (opcional).
- Ideal para proyectos de automatización o alimentación inteligente.

---

## 🧠 Autor

Desarrollado por Susann Baldiviezo, Florencia Frigerio, Jesus Ibarra y Giovanna Tarifa como parte de un proyecto académico de Ingeniería Mecatrónica para las materias de Programacion Superior, Fundamentos de Contro y Seniales y Sistemas.

