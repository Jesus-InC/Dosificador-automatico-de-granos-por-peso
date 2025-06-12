#include "Dispensador.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "HX711.h"
#include <Servo.h>

// Pines
#define DOUT 2
#define CLK 3
#define BTN_UP 4
#define BTN_DOWN 5
#define BTN_START 6
#define SERVO_PIN 9
#define BUZZER_LED_PIN 10

// Configuración
const float FACTOR_CALIBRACION = 410.459991;
const int PASO_UMBRAL = 50;
const int UMBRAL_MIN = 100;
const int UMBRAL_MAX = 5000;
const int SERVO_CERRADO = 180;
const int SERVO_ABIERTO = 150;

// Objetos
HX711 scale;
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;

// Variables de estado
int peso_umbral = 100;
bool sistema_activo = false;
unsigned long ultimo_envio = 0;
const unsigned long INTERVALO_ENVIO = 500; // ms

// Declaración funciones privadas (prototipos)
void mostrarMensaje(String linea1, String linea2);
void actualizarPantalla(float peso);
void iniciarServido();
void detectarBotonStart();
void manejarBotones();

// Funciones públicas
void dispensadorSetup() {
  // Inicializar componentes
  lcd.init();
  lcd.backlight();

  scale.begin(DOUT, CLK);
  scale.set_scale(FACTOR_CALIBRACION);
  scale.tare();

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_START, INPUT_PULLUP);

  servo.attach(SERVO_PIN);
  servo.write(SERVO_CERRADO);

  pinMode(BUZZER_LED_PIN, OUTPUT);
  digitalWrite(BUZZER_LED_PIN, LOW);

  mostrarMensaje("Sistema Listo", "Peso: 0.0g");
}

void dispensadorLoop() {
  float peso_actual = scale.get_units(3);

  // Enviar datos periódicamente
  if (millis() - ultimo_envio >= INTERVALO_ENVIO) {
    Serial.print("PESO:");
    Serial.println(peso_actual, 1);
    ultimo_envio = millis();
  }

  // Actualizar pantalla
  actualizarPantalla(peso_actual);

  // Manejar comandos seriales
  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();

    if (comando.startsWith("SET:")) {
      int nuevo_umbral = comando.substring(4).toInt();
      if (nuevo_umbral >= UMBRAL_MIN && nuevo_umbral <= UMBRAL_MAX) {
        peso_umbral = nuevo_umbral;
        lcd.setCursor(0, 1);
        lcd.print("Meta: ");
        lcd.print(peso_umbral);
        lcd.print("g  ");
      }
    }
    else if (comando == "START") {
      iniciarServido();
    }
    else if (comando == "TARE") {
      scale.tare();
      Serial.println("TARA:OK");
    }
  }

  // Manejar botones físicos
  manejarBotones();
  delay(100);
}

// Funciones privadas

void iniciarServido() {
  if (sistema_activo) return;
  sistema_activo = true;

  Serial.println("INICIANDO");
  float peso_inicial = scale.get_units(5);

  // Señal de inicio
  tone(BUZZER_LED_PIN, 1000, 200);
  mostrarMensaje("Sirviendo...", "");

  // Abrir servo
  for (int ang = SERVO_CERRADO; ang >= SERVO_ABIERTO; ang--) {
    servo.write(ang);
    delay(50);
  }

  // Proceso de servido
  while (scale.get_units(3) < peso_umbral - 10) {
    float peso = scale.get_units(3);
    Serial.print("PROGRESO:");
    Serial.println(peso, 1);
    delay(200);
  }

  // Cerrar servo
  servo.write(SERVO_CERRADO);

  // Finalización
  float peso_final = scale.get_units(3);
  Serial.print("FINAL:");
  Serial.println(peso_final, 1);

  // Señal de fin
  for (int i = 0; i < 3; i++) {
    tone(BUZZER_LED_PIN, 1200, 150);
    delay(300);
  }

  mostrarMensaje("Servido completo", "Peso final: " + String(peso_final, 1) + "g");
  delay(3000);
  scale.tare();  // Reiniciar la tara después del servido
  sistema_activo = false;

  // Enviar peso dispensado
  float peso_dispensado = peso_final - peso_inicial;
  Serial.print("DISPENSADO:");
  Serial.println(peso_dispensado, 1);
}

void mostrarMensaje(String linea1, String linea2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(linea1);
  lcd.setCursor(0, 1);
  lcd.print(linea2);
}

void actualizarPantalla(float peso) {
  lcd.setCursor(0, 0);
  lcd.print("Peso: ");
  lcd.print(peso, 1);
  lcd.print("g  Ult.");

  lcd.setCursor(0, 1);
  lcd.print("Meta: ");
  lcd.print(peso_umbral);
  lcd.print("g  ");
}

void detectarBotonStart() {
  static unsigned long tiempoInicio = 0;
  static bool esperandoLiberacion = false;

  if (digitalRead(BTN_START) == LOW) {
    if (!esperandoLiberacion) {
      tiempoInicio = millis();
      esperandoLiberacion = true;
    } else if (millis() - tiempoInicio >= 2000) {
      // Botón presionado más de 2 segundos: hacer tara
      scale.tare();
      mostrarMensaje("Tara realizada", "Peso: 0.0g");
      Serial.println("TARA MANUAL EJECUTADA");
      delay(1000);  // Esperar un momento para evitar repeticiones
      esperandoLiberacion = false;
    }
  } else {
    if (esperandoLiberacion && millis() - tiempoInicio < 2000) {
      // Pulsación corta: iniciar servido
      iniciarServido();
    }
    esperandoLiberacion = false;
  }
}

void manejarBotones() {
  // Botón UP
  if (digitalRead(BTN_UP) == LOW) {
    peso_umbral = min(peso_umbral + PASO_UMBRAL, UMBRAL_MAX);
    Serial.print("UMBRAL:");
    Serial.println(peso_umbral);
    actualizarPantalla(scale.get_units(3));
    delay(300);
  }

  // Botón DOWN
  if (digitalRead(BTN_DOWN) == LOW) {
    peso_umbral = max(peso_umbral - PASO_UMBRAL, UMBRAL_MIN);
    Serial.print("UMBRAL:");
    Serial.println(peso_umbral);
    actualizarPantalla(scale.get_units(3));
    delay(300);
  }

  // Botón START
  detectarBotonStart();
}
