#include <hardware/gpio.h>  // Para controlar los pines GPIO
#include <hardware/adc.h>
#include <pico/stdlib.h>

// Variables para temperatura, presión y nivel
unsigned long previousMillis = 0;
const long interval = 3000;  // Intervalo de 3 segundos

float contador = 60;
float presion = 0;
float nivel = 0;
float intervalo = 0.1;
constexpr uint8_t ledPin = 25;  // El LED incorporado de la Raspberry Pi Pico está en el pin GPIO 25
float temperatura = 25.0;  // Valor simulado de la temperatura
float checksum = 0;
float temperaturaEstado = 0;
float presionEstado = 0;
float nivelEstado = 0;

// Máquina de estados
enum class TaskStates {
  INIT,
  WAIT_COMMANDS
};
TaskStates taskState = TaskStates::INIT;

// Función para la máquina de estados
void task() {
  switch (taskState) {
    case TaskStates::INIT: {
      if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        
        if (command == "subirIntervalo") {
          if (intervalo < 0.9) {
            intervalo += 0.1;
            Serial.println(intervalo);
          }
        }
        if (command == "bajarIntervalo") {
          if (intervalo > 0.1) {
            intervalo -= 0.1;
            Serial.println(intervalo);
          }
        }
        if (command == "BajarTiempo") {
          if (contador > 0) {
            contador -= intervalo;
            Serial.println(contador);
          }
        }
        if (command == "SubirTiempo") {
          if (contador < 60) {
            contador += intervalo;
            Serial.println(contador);
          }
        }
        if (command == "SubirPresion") {
          if (presion < 25) {
            presion += intervalo;
            Serial.print("Presion: ");
            Serial.println(presion);
          }
        }
        if (command == "BajarPresion") {
          if (presion > 0) {
            presion -= intervalo;
            Serial.print("Presion: ");
            Serial.println(presion);
          }
        }
        if (command == "SubirNivel") {
          if (nivel < 25) {
            nivel += intervalo;
            Serial.print("Nivel: ");
            Serial.println(nivel);
          }
        }
        if (command == "BajarNivel") {
          if (nivel > 0) {
            nivel -= intervalo;
            Serial.print("Nivel: ");
            Serial.println(nivel);
          }
        }
        if (command == "Iniciar") {
          gpio_put(ledPin, 0);  // Apagar LED
          taskState = TaskStates::WAIT_COMMANDS;
        }
      }
      break;
    }
    
    case TaskStates::WAIT_COMMANDS: {
      unsigned long currentMillis = millis();
      if (Serial.available() > 0 && Serial.read() == 's') {
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;
          checksum = 0;

          // Sumar 1 a cada variable cada 3 segundos
          presion++;
          nivel++;
          contador--;

          checksum = temperatura + temperaturaEstado + intervalo + presion + presionEstado + nivel + nivelEstado;

          // Mostrar valores actualizados
          Serial.print("Temperatura: ");
          Serial.print(temperatura);
          Serial.print(" Presion: ");
          Serial.print(presion);
          Serial.print(" Nivel de agua: ");
          Serial.println(nivel);
        }

        if (Serial.available() > 0) {
          String command = Serial.readStringUntil('\n');
          if (command == "C1234") {
            temperatura = 0;
            presion = 0;
            nivel = 0;
            taskState = TaskStates::INIT;
          }
          if (command == "LEDOn") {
            gpio_put(ledPin, 1);  // Encender LED
          }
          if (command == "LEDOff") {
            gpio_put(ledPin, 0);  // Apagar LED
          }
        }
      }
      break;
    }
    
    default:
      break;
  }
}

// Configuración inicial de la Raspberry Pi Pico
void setup() {
  Serial.begin(115200);  // Configura la velocidad del puerto serial
  gpio_init(ledPin);  // Inicializar pin del LED
  gpio_set_dir(ledPin, GPIO_OUT);  // Configurar pin del LED como salida
}

// Bucle principal
void loop() {
  task();  // Ejecuta la máquina de estados
}
