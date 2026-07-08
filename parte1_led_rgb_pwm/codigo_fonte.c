/*
  SEL0433 - Projeto 3
  Parte 1: Controle PWM de LED RGB com ESP32
  Ligações usadas na simulação:
  R   -> resistor de 220 ohms -> GPIO 25
  G   -> resistor de 220 ohms -> GPIO 26
  B   -> resistor de 220 ohms -> GPIO 27
  COM -> GND
*/

#include <Arduino.h>

// Pinos da ESP32 ligados em cada cor do LED RGB
#define PINO_R 25
#define PINO_G 26
#define PINO_B 27

// Configuração do PWM
// A frequência usada foi 5 kHz.
// A resolução de 8 bits faz o duty cycle variar de 0 até 255.
const int frequenciaPWM = 5000;
const int resolucaoPWM = 8;
const int dutyMaximo = 255;

// Variáveis que guardam o brilho atual de cada cor
int dutyR = 0;
int dutyG = 0;
int dutyB = 0;

// Incrementos diferentes para cada cor
// Assim cada componente do LED muda em uma velocidade diferente.
int incrementoR = 15;
int incrementoG = 5;
int incrementoB = 10;

void setup() {
  // Inicializa a comunicação serial.
  // Ela será usada para mostrar os valores de duty cycle durante a simulação.
  Serial.begin(115200);

  // Configura os três pinos como saídas PWM.
  // A função ledcAttach liga o pino escolhido a um canal PWM interno da ESP32.
  ledcAttach(PINO_R, frequenciaPWM, resolucaoPWM);
  ledcAttach(PINO_G, frequenciaPWM, resolucaoPWM);
  ledcAttach(PINO_B, frequenciaPWM, resolucaoPWM);

  // Mensagens iniciais no monitor serial
  Serial.println("SEL0433 - Projeto 3");
  Serial.println("Parte 1 - Controle PWM de LED RGB");
  Serial.println("Frequencia: 5 kHz");
  Serial.println("Resolucao: 8 bits");
  Serial.println("Serial: 115200 baud");
  Serial.println("--------------------------------------");
}

void loop() {
  // Aumenta o duty cycle de cada cor.
  // Cada cor recebe um incremento diferente.
  dutyR = dutyR + incrementoR;
  dutyG = dutyG + incrementoG;
  dutyB = dutyB + incrementoB;

  // Quando o valor passa de 255, ele volta para zero.
  // Isso faz o brilho variar continuamente durante a simulação.
  if (dutyR > dutyMaximo) {
    dutyR = 0;
  }

  if (dutyG > dutyMaximo) {
    dutyG = 0;
  }

  if (dutyB > dutyMaximo) {
    dutyB = 0;
  }

  // Aplica o PWM em cada pino.
  // Quanto maior o duty cycle, maior o brilho daquela cor.
  ledcWrite(PINO_R, dutyR);
  ledcWrite(PINO_G, dutyG);
  ledcWrite(PINO_B, dutyB);

  // Converte os valores de 0 a 255 para porcentagem.
  // Isso deixa a leitura no monitor serial mais fácil.
  int porcentagemR = map(dutyR, 0, 255, 0, 100);
  int porcentagemG = map(dutyG, 0, 255, 0, 100);
  int porcentagemB = map(dutyB, 0, 255, 0, 100);

  // Mostra no monitor serial os incrementos e os duty cycles aplicados.
  Serial.print("Incrementos -> ");
  Serial.print("R: ");
  Serial.print(incrementoR);
  Serial.print(" | G: ");
  Serial.print(incrementoG);
  Serial.print(" | B: ");
  Serial.print(incrementoB);

  Serial.print(" || Duty cycle -> ");

  Serial.print("R: ");
  Serial.print(dutyR);
  Serial.print(" (");
  Serial.print(porcentagemR);
  Serial.print("%)");

  Serial.print(" | G: ");
  Serial.print(dutyG);
  Serial.print(" (");
  Serial.print(porcentagemG);
  Serial.print("%)");

  Serial.print(" | B: ");
  Serial.print(dutyB);
  Serial.print(" (");
  Serial.print(porcentagemB);
  Serial.println("%)");

  // Pequeno atraso para conseguir observar a mudança de cor no LED
  // e também para não deixar o monitor serial rápido demais.
  delay(200);
}
