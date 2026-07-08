/*
  SEL0433 - Projeto 3
  Parte 2 - Exercício 1
  Controle de servomotor com potenciômetro usando ESP32

  Funcionamento:
  - O potenciômetro fornece uma tensão variável para a entrada analógica da ESP32.
  - A ESP32 lê esse valor pelo ADC.
  - O valor lido é convertido para um ângulo entre 0 e 180 graus.
  - O ângulo é enviado para o servomotor.

  Ligações:
  Potenciômetro:
  VCC -> 3V3
  GND -> GND
  SIG -> ESP 34

  Servo:
  V+  -> 5V
  GND -> GND
  PWM -> ESP 18
*/

#include <Arduino.h>
#include <ESP32Servo.h>

// Pino analógico ligado ao terminal central do potenciômetro
#define PINO_POT 34

// Pino PWM usado para controlar o servo
#define PINO_SERVO 18

// Criação do objeto do servo
Servo meuServo;

void setup() {
  // Inicializa a comunicação serial para acompanhar os valores da simulação
  Serial.begin(115200);

  // Configura o servo no pino escolhido
  meuServo.attach(PINO_SERVO);

  // Mensagens iniciais no monitor serial
  Serial.println("SEL0433 - Projeto 3");
  Serial.println("Parte 2 - Servo controlado por potenciometro");
  Serial.println("--------------------------------------------");
}

void loop() {
  // Lê o valor analógico do potenciômetro.
  // Na ESP32, o ADC normalmente varia de 0 a 4095.
  int valorADC = analogRead(PINO_POT);

  // Converte o valor do ADC para um ângulo de 0 a 180 graus.
  // Quando o potenciômetro está no mínimo, o servo vai para 0 graus.
  // Quando está no máximo, o servo vai para 180 graus.
  int angulo = map(valorADC, 0, 4095, 0, 180);

  // Envia o ângulo calculado para o servomotor
  meuServo.write(angulo);

  // Calcula uma porcentagem apenas para mostrar no monitor serial.
  // Isso ajuda a relacionar o potenciômetro com o duty cycle pedido no projeto.
  int porcentagem = map(valorADC, 0, 4095, 0, 100);

  // Mostra os valores no monitor serial
  Serial.print("ADC: ");
  Serial.print(valorADC);

  Serial.print(" | Controle: ");
  Serial.print(porcentagem);
  Serial.print("%");

  Serial.print(" | Angulo do servo: ");
  Serial.print(angulo);
  Serial.println(" graus");

  // Pequeno atraso para deixar a leitura mais estável no monitor serial
  delay(200);
}
