/*
  SEL0433 - Projeto 3
  Parte 2 - Exercício 2

  Aplicação escolhida:
  Cancela automática controlada por servomotor.

  Funcionamento da simulação:
  - O botão é usado para abrir ou fechar a cancela.
  - O servo representa a cancela.
  - O sinal PWM do servo é gerado usando o periférico MCPWM da ESP32.
  - O buzzer emite um aviso curto quando a cancela começa a se mover.
  - O display OLED mostra o estado atual do sistema.
  - O monitor serial mostra as informações principais da operação.

  Ligações usadas:

  Servo:
  V+  -> 5V
  GND -> GND
  PWM -> GPIO 18

  Botão:
  Um lado -> GPIO 19
  Outro lado -> GND

  Buzzer:
  + -> GPIO 23
  - -> GND

  OLED I2C:
  VCC -> 3V3
  GND -> GND
  SDA -> GPIO 21
  SCL -> GPIO 22
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "driver/mcpwm.h"


// Definição dos pinos usados

#define PINO_SERVO 18
#define PINO_BOTAO 19
#define PINO_BUZZER 23

#define PINO_SDA 21
#define PINO_SCL 22

// Configuração do display OLED
#define LARGURA_TELA 128
#define ALTURA_TELA 64
#define ENDERECO_OLED 0x3C

Adafruit_SSD1306 display(LARGURA_TELA, ALTURA_TELA, &Wire, -1);

// Parâmetros do servo
// O servo trabalha normalmente com frequência de 50 Hz.
// Isso significa um período de 20 ms.
// Para controlar a posição, mudamos a largura do pulso.

#define FREQ_SERVO 50

#define PULSO_MIN 500    // pulso aproximado para 0 graus
#define PULSO_MAX 2400   // pulso aproximado para 180 graus


// Variáveis de controle
int anguloAtual = 0;             // começa com a cancela fechada
bool cancelaAberta = false;      // estado inicial
unsigned long ultimoAperto = 0;  // usado para evitar leitura dupla do botão



// Função que mostra informações no OLED
void atualizarDisplay(const char *estado, int angulo) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("Projeto 3 - Parte 2");

  display.setCursor(0, 18);
  display.print("Estado: ");
  display.println(estado);

  display.setCursor(0, 34);
  display.print("Angulo: ");
  display.print(angulo);
  display.println(" graus");

  display.setCursor(0, 50);
  display.println("MCPWM no GPIO 18");

  display.display();
}




// Função para acionar o buzzer

void tocarBuzzer() {
  digitalWrite(PINO_BUZZER, HIGH);
  delay(120);
  digitalWrite(PINO_BUZZER, LOW);
}


// Converte ângulo em largura de pulso
// O servo não recebe diretamente o ângulo.
// Primeiro o ângulo é convertido em largura de pulso em microssegundos.

int anguloParaPulso(int angulo) {
  angulo = constrain(angulo, 0, 180);
  return map(angulo, 0, 180, PULSO_MIN, PULSO_MAX);
}

// Aplica o ângulo no servo usando MCPWM

void aplicarAnguloServo(int angulo) {
  int pulso = anguloParaPulso(angulo);

  // Período de 50 Hz = 20.000 us.
  // Duty cycle em porcentagem = tempo em nível alto / período total.
  float duty = (pulso / 20000.0) * 100.0;

  // Envia o duty cycle para o canal MCPWM.
  mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty);
  mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
}

// Movimento suave da cancela
// Em vez de mudar direto de 0 para 90 graus,
// o servo vai mudando aos poucos. Isso deixa a simulação mais clara.

void moverCancela(int destino, const char *estadoMovendo, const char *estadoFinal) {
  int passo;

  if (destino > anguloAtual) {
    passo = 2;
  } else {
    passo = -2;
  }

  Serial.print("Movimento: ");
  Serial.println(estadoMovendo);

  atualizarDisplay(estadoMovendo, anguloAtual);
  tocarBuzzer();

  while (anguloAtual != destino) {
    anguloAtual = anguloAtual + passo;

    if (passo > 0 && anguloAtual > destino) {
      anguloAtual = destino;
    }

    if (passo < 0 && anguloAtual < destino) {
      anguloAtual = destino;
    }

    aplicarAnguloServo(anguloAtual);

    Serial.print("Angulo da cancela: ");
    Serial.print(anguloAtual);
    Serial.println(" graus");

    atualizarDisplay(estadoMovendo, anguloAtual);

    delay(40);
  }

  Serial.print("Estado final: ");
  Serial.println(estadoFinal);
  Serial.println("--------------------------------");

  atualizarDisplay(estadoFinal, anguloAtual);
}

void setup() {
  // Inicializa a comunicação serial.
  // Ela é usada para acompanhar os estados da simulação.
  Serial.begin(115200);

  // Configura o botão com resistor interno de pull-up.
  // Assim, quando o botão não está pressionado, a leitura fica em HIGH.
  // Quando o botão é pressionado, a leitura vai para LOW.
  pinMode(PINO_BOTAO, INPUT_PULLUP);

  // Configura o buzzer como saída.
  pinMode(PINO_BUZZER, OUTPUT);
  digitalWrite(PINO_BUZZER, LOW);

  // Inicializa a comunicação I2C do display OLED.
  Wire.begin(PINO_SDA, PINO_SCL);

  // Inicializa o OLED.
  if (!display.begin(SSD1306_SWITCHCAPVCC, ENDERECO_OLED)) {
    Serial.println("Falha ao iniciar o display OLED.");
    while (true) {
      delay(100);
    }
  }

  // Configura o pino GPIO 18 como saída MCPWM.
  // O sinal MCPWM0A será usado para controlar o servo.
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, PINO_SERVO);

  // Configuração básica do MCPWM.
  mcpwm_config_t pwm_config;
  pwm_config.frequency = FREQ_SERVO;
  pwm_config.cmpr_a = 0;
  pwm_config.cmpr_b = 0;
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

  // Inicializa o MCPWM com as configurações acima.
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);

  // Começa com a cancela fechada.
  anguloAtual = 0;
  aplicarAnguloServo(anguloAtual);

  Serial.println("SEL0433 - Projeto 3");
  Serial.println("Parte 2 - Aplicacao com MCPWM");
  Serial.println("Sistema: cancela automatica com servo");
  Serial.println("Botao no GPIO 19");
  Serial.println("Servo MCPWM no GPIO 18");
  Serial.println("--------------------------------");

  atualizarDisplay("Fechada", anguloAtual);
}

void loop() {
  // Lê o estado do botão.
  int leituraBotao = digitalRead(PINO_BOTAO);

  // Como foi usado INPUT_PULLUP, o botão pressionado gera LOW.
  // O millis evita que um único aperto seja lido várias vezes.
  if (leituraBotao == LOW && millis() - ultimoAperto > 600) {
    ultimoAperto = millis();

    if (cancelaAberta == false) {
      // Se a cancela está fechada, abre até 90 graus.
      moverCancela(90, "Abrindo", "Aberta");
      cancelaAberta = true;
    } else {
      // Se a cancela está aberta, fecha voltando para 0 grau.
      moverCancela(0, "Fechando", "Fechada");
      cancelaAberta = false;
    }
  }
}
