SISTEMA DE CONTROL DE BALANZA AUTOMATIZADO
===========================================

1. HARDWARE NECESARIO
---------------------
- Arduino Nano
- Módulo HX711
- Celda de carga (5kg recomendado)
- Servomotor SG90
- Display LCD I2C 16x2
- 3 botones pulsadores
- Buzzer pasivo
- LED indicador
- Protoboard
- Cables dupont (M-M, H-H)
- Fuente de alimentación 5V/2A

2. INSTALACIÓN DE SOFTWARE
-------------------------
2.1. Para Arduino:
- Arduino IDE 2.x
- Bibliotecas:
  * HX711 by Bogdan Necula
  * LiquidCrystal I2C by Frank de Brabander
  * Servo (incluida por defecto)

2.2. Para Python:
- Python 3.7 o superior
- Paquetes:
  * fastapi
  * uvicorn
  * pyserial

3. CONEXIÓN DE COMPONENTES
--------------------------
3.1. Conexiones HX711:
- DOUT -> Pin D2 Arduino
- CLK -> Pin D3 Arduino
- VCC -> 5V
- GND -> GND

3.2. Conexiones LCD I2C:
- SDA -> A4
- SCL -> A5
- VCC -> 5V
- GND -> GND

3.3. Conexiones Servomotor:
- Señal -> D9
- VCC -> 5V
- GND -> GND

3.4. Conexiones Botones:
- BTN_UP -> D4
- BTN_DOWN -> D5
- BTN_START -> D6
- Todos con resistencia pull-up interna

4. CONFIGURACIÓN INICIAL
------------------------
4.1. Arduino:
1. Abrir Arduino IDE
2. Pegar código completo del sketch
3. Seleccionar:
   - Placa: Arduino Nano
   - Procesador: ATmega328P (Old Bootloader)
   - Puerto COM correcto
4. Subir sketch

4.2. Python:
1. Crear archivo main.py
2. Pegar código Python completo
3. Editar:
   SERIAL_PORT = "COM3" # Cambiar por tu puerto
   BAUD_RATE = 9600     # Debe coincidir con Arduino

5. CALIBRACIÓN DEL SISTEMA
--------------------------
5.1. Calibración inicial:
1. Vaciar la plataforma
2. Ejecutar comando de tarado:
   scale.tare()
3. Esperar 5 segundos

5.2. Ajuste de factor:
1. Colocar peso conocido (ej. 500g)
2. Calcular:
   factor = lectura_raw / peso_real
3. Ajustar en sketch:
   const float FACTOR_CALIBRACION = nuevo_valor
4. Subir sketch nuevamente

6. OPERACIÓN DEL SISTEMA
------------------------
6.1. Iniciar servidor:
uvicorn main:app --reload

6.2. Interfaz web:
Acceder a http://localhost:8000

6.3. Controles:
- Botones UP/DOWN: ajustar umbral
- Botón START: iniciar servido
- Pantalla muestra peso en tiempo real

7. COMANDOS SERIALES
--------------------
- "SET:500" -> Establece umbral a 500g
- "START" -> Inicia proceso
- "TARE" -> Tarar balanza
- "CAL:123.45" -> Ajusta factor calibración

8. SOLUCIÓN DE PROBLEMAS
------------------------
8.1. No se detecta Arduino:
- Verificar conexión USB
- Revisar puerto COM
- Reiniciar servidor Python

8.2. Lecturas inestables:
- Revisar conexiones HX711
- Asegurar celda de carga fija
- Alejar de fuentes de interferencia

8.3. Servo no responde:
- Verificar alimentación 5V
- Comprobar conexión a D9
- Revisar rango de movimiento

9. ESTRUCTURA DE CÓDIGO
-----------------------
9.1. Sketch Arduino:
- Configuración pines
- Lógica de medición
- Control de servo
- Comunicación serial

9.2. Servidor Python:
- API FastAPI
- Comunicación serial
- Interfaz web
- Manejo de eventos

