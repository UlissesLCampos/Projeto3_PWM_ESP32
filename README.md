# Projeto 3 - PWM e Comunicação com ESP32
Lucas Manoel Freitas da Silva - 15471884 /
Gabriel Suerdieck Nardelli - 15453960 /
Ulisses Lombardi Campos - 14781443 /

## 1. Introdução

Este projeto foi desenvolvido usando a placa ESP32 DevKit no simulador Wokwi. O objetivo foi praticar o uso de PWM, comunicação serial, leitura analógica, controle de servomotor, display OLED via I2C e o periférico MCPWM da ESP32.

O projeto foi dividido em três partes:

- Parte 1: controle PWM de um LED RGB;
- Parte 2 - Exercício 1: controle de um servomotor com potenciômetro;
- Parte 2 - Exercício 2: aplicação própria usando MCPWM.

Os códigos foram feitos no ambiente Arduino/Wokwi e os circuitos foram montados no simulador.

---

## 2. Organização do repositório

```text
Projeto3_PWM_ESP32/
│
├── README.md
│
├── parte1_led_rgb_pwm/
│   ├── sketch.ino
│   ├── codigo_fonte.c
│   └── diagram.json
│
├── parte2_ex1_servo_potenciometro/
│   ├── sketch.ino
│   ├── codigo_fonte.c
│   └── diagram.json
│
├── parte2_ex2_cancela_mcpwm/
│   ├── sketch.ino
│   ├── codigo_fonte.c
│   └── diagram.json

```

Os arquivos `sketch.ino` foram usados para rodar as simulações no Wokwi. Também foi colocada uma cópia dos códigos como `codigo_fonte.c`.

---

# Parte 1 - Controle PWM de LED RGB

## 3. Objetivo

Na Parte 1 foi feito o controle de um LED RGB de catodo comum usando três saídas PWM da ESP32.

Cada cor do LED foi ligada em um pino diferente:

- vermelho no GPIO 25;
- verde no GPIO 26;
- azul no GPIO 27.

A frequência usada no PWM foi de **5 kHz** e a resolução foi de **8 bits**, fazendo o duty cycle variar de 0 até 255.

---

## 4. Componentes utilizados

- ESP32 DevKit
- LED RGB de catodo comum
- 3 resistores de 220 ohms
- Monitor serial do Wokwi

---

## 5. Ligações da Parte 1

```text
R   -> resistor de 220 ohms -> GPIO 25
G   -> resistor de 220 ohms -> GPIO 26
B   -> resistor de 220 ohms -> GPIO 27
COM -> GND
```

---

## 6. Funcionamento do programa

No código, foram definidos os pinos do LED RGB:

```c
#define PINO_R 25
#define PINO_G 26
#define PINO_B 27
```

Também foram definidos os parâmetros do PWM:

```c
const int frequenciaPWM = 5000;
const int resolucaoPWM = 8;
const int dutyMaximo = 255;
```

Cada cor possui uma variável de duty cycle:

```c
int dutyR = 0;
int dutyG = 0;
int dutyB = 0;
```

E cada uma possui um incremento diferente:

```c
int incrementoR = 15;
int incrementoG = 5;
int incrementoB = 10;
```

No `setup()`, a comunicação serial é iniciada em 115200 baud e os pinos são configurados como saídas PWM:

```c
Serial.begin(115200);

ledcAttach(PINO_R, frequenciaPWM, resolucaoPWM);
ledcAttach(PINO_G, frequenciaPWM, resolucaoPWM);
ledcAttach(PINO_B, frequenciaPWM, resolucaoPWM);
```

No `loop()`, os duty cycles são atualizados:

```c
dutyR = dutyR + incrementoR;
dutyG = dutyG + incrementoG;
dutyB = dutyB + incrementoB;
```

Quando algum valor passa de 255, ele volta para zero:

```c
if (dutyR > dutyMaximo) {
  dutyR = 0;
}
```

Depois, o PWM é aplicado em cada cor do LED:

```c
ledcWrite(PINO_R, dutyR);
ledcWrite(PINO_G, dutyG);
ledcWrite(PINO_B, dutyB);
```

Também são calculadas as porcentagens para facilitar a leitura no monitor serial:

```c
int porcentagemR = map(dutyR, 0, 255, 0, 100);
int porcentagemG = map(dutyG, 0, 255, 0, 100);
int porcentagemB = map(dutyB, 0, 255, 0, 100);
```

O monitor serial mostra os incrementos e os duty cycles aplicados.

---

# Parte 2 - Exercício 1: Servo controlado por potenciômetro

## 8. Objetivo

Nesta parte foi feito o controle de um servomotor usando um potenciômetro. A ESP32 lê o valor analógico do potenciômetro pelo ADC e converte esse valor para um ângulo entre 0° e 180°.

---

## 9. Componentes utilizados

- ESP32 DevKit
- Servomotor
- Potenciômetro
- Biblioteca ESP32Servo
- Monitor serial do Wokwi

---

## 10. Ligações da Parte 2 - Exercício 1

```text
Potenciômetro:
VCC -> 3V3
GND -> GND
SIG -> GPIO 34

Servo:
V+  -> 5V
GND -> GND
PWM -> GPIO 18
```

---

## 11. Funcionamento do programa

No código, foi usada a biblioteca `ESP32Servo.h`:

```c
#include <ESP32Servo.h>
```

Foram definidos os pinos do potenciômetro e do servo:

```c
#define PINO_POT 34
#define PINO_SERVO 18
```

O objeto do servo foi criado com o nome `meuServo`:

```c
Servo meuServo;
```

No `setup()`, a comunicação serial é iniciada e o servo é configurado no GPIO 18:

```c
Serial.begin(115200);
meuServo.attach(PINO_SERVO);
```

No `loop()`, a ESP32 lê o valor analógico do potenciômetro:

```c
int valorADC = analogRead(PINO_POT);
```

Esse valor é convertido para um ângulo de 0 a 180 graus:

```c
int angulo = map(valorADC, 0, 4095, 0, 180);
```

Depois, o ângulo é enviado para o servo:

```c
meuServo.write(angulo);
```

Também é calculada uma porcentagem para aparecer no monitor serial:

```c
int porcentagem = map(valorADC, 0, 4095, 0, 100);
```

O monitor serial mostra o valor do ADC, a porcentagem de controle e o ângulo aplicado no servo.

---

## 12. Registros da simulação da Parte 2 - Exercício 1
As fotos da simulação estão dentro do projeto

# Parte 2 - Exercício 2: Aplicação própria com MCPWM

## 13. Objetivo

No segundo exercício da Parte 2 foi desenvolvida uma aplicação própria usando o periférico MCPWM da ESP32.

A aplicação escolhida foi uma cancela automática. O servomotor representa a cancela, o botão é usado para abrir e fechar, o buzzer faz um aviso sonoro e o display OLED mostra o estado atual do sistema.

---

## 14. Componentes utilizados

- ESP32 DevKit
- Servomotor
- Pushbutton
- Buzzer
- Display OLED SSD1306
- Comunicação I2C
- Comunicação serial UART
- Periférico MCPWM da ESP32

---

## 15. Ligações da Parte 2 - Exercício 2

```text
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
```

---

## 16. Funcionamento do sistema

A cancela começa fechada, com o servo em 0°. Quando o botão é pressionado, a cancela abre até 90°. Quando o botão é pressionado novamente, ela fecha e retorna para 0°.

Durante o movimento:

- o servo é controlado pelo MCPWM;
- o buzzer toca rapidamente;
- o OLED mostra o estado atual;
- o monitor serial mostra o movimento e o ângulo da cancela.

Os estados usados no OLED foram:

```text
Fechada
Abrindo
Aberta
Fechando
```

---

## 17. Funcionamento do código

As bibliotecas usadas nessa parte foram:

```c
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "driver/mcpwm.h"
```

Os pinos foram definidos assim:

```c
#define PINO_SERVO 18
#define PINO_BOTAO 19
#define PINO_BUZZER 23

#define PINO_SDA 21
#define PINO_SCL 22
```

O display OLED foi configurado com largura de 128 pixels, altura de 64 pixels e endereço I2C `0x3C`:

```c
#define LARGURA_TELA 128
#define ALTURA_TELA 64
#define ENDERECO_OLED 0x3C

Adafruit_SSD1306 display(LARGURA_TELA, ALTURA_TELA, &Wire, -1);
```

O servo foi configurado para trabalhar com frequência de 50 Hz:

```c
#define FREQ_SERVO 50

#define PULSO_MIN 500
#define PULSO_MAX 2400
```

As variáveis principais de controle foram:

```c
int anguloAtual = 0;
bool cancelaAberta = false;
unsigned long ultimoAperto = 0;
```

A função `atualizarDisplay()` mostra as informações no OLED:

```c
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

  display.display();
}
```

A função `tocarBuzzer()` liga o buzzer por um curto tempo:

```c
void tocarBuzzer() {
  digitalWrite(PINO_BUZZER, HIGH);
  delay(120);
  digitalWrite(PINO_BUZZER, LOW);
}
```

A função `anguloParaPulso()` converte o ângulo do servo para largura de pulso em microssegundos:

```c
int anguloParaPulso(int angulo) {
  angulo = constrain(angulo, 0, 180);
  return map(angulo, 0, 180, PULSO_MIN, PULSO_MAX);
}
```

A função `aplicarAnguloServo()` calcula o duty cycle e aplica o sinal no MCPWM:

```c
void aplicarAnguloServo(int angulo) {
  int pulso = anguloParaPulso(angulo);

  float duty = (pulso / 20000.0) * 100.0;

  mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty);
  mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
}
```

A função `moverCancela()` faz o movimento da cancela aos poucos, de 0° até 90° ou de 90° até 0°:

```c
void moverCancela(int destino, const char *estadoMovendo, const char *estadoFinal) {
  int passo;

  if (destino > anguloAtual) {
    passo = 2;
  } else {
    passo = -2;
  }

  atualizarDisplay(estadoMovendo, anguloAtual);
  tocarBuzzer();

  while (anguloAtual != destino) {
    anguloAtual = anguloAtual + passo;

    aplicarAnguloServo(anguloAtual);
    atualizarDisplay(estadoMovendo, anguloAtual);

    delay(40);
  }

  atualizarDisplay(estadoFinal, anguloAtual);
}
```

No `setup()`, o botão foi configurado com `INPUT_PULLUP`, o buzzer como saída, o OLED foi iniciado e o MCPWM foi configurado:

```c
pinMode(PINO_BOTAO, INPUT_PULLUP);
pinMode(PINO_BUZZER, OUTPUT);

Wire.begin(PINO_SDA, PINO_SCL);

mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, PINO_SERVO);
```

A configuração básica do MCPWM foi feita com:

```c
mcpwm_config_t pwm_config;
pwm_config.frequency = FREQ_SERVO;
pwm_config.cmpr_a = 0;
pwm_config.cmpr_b = 0;
pwm_config.counter_mode = MCPWM_UP_COUNTER;
pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
```

No `loop()`, o botão é lido. Como foi usado `INPUT_PULLUP`, quando o botão é pressionado a leitura fica em `LOW`:

```c
int leituraBotao = digitalRead(PINO_BOTAO);

if (leituraBotao == LOW && millis() - ultimoAperto > 600) {
  ultimoAperto = millis();

  if (cancelaAberta == false) {
    moverCancela(90, "Abrindo", "Aberta");
    cancelaAberta = true;
  } else {
    moverCancela(0, "Fechando", "Fechada");
    cancelaAberta = false;
  }
}
```

---

## 18. Registros da simulação da Parte 2 - Exercício 2
As fotos da simulação estão dentro do projeto

# 19. Bibliotecas utilizadas

## Arduino.h

Foi usada como base do ambiente Arduino para a ESP32. Ela permite usar funções como `pinMode`, `digitalWrite`, `analogRead`, `delay`, `millis` e comunicação serial.

## ESP32Servo.h

Foi usada na Parte 2 - Exercício 1 para facilitar o controle do servomotor com a ESP32.

## Wire.h

Foi usada para iniciar a comunicação I2C com o display OLED.

## Adafruit_GFX.h

Foi usada para permitir a escrita de textos no display OLED.

## Adafruit_SSD1306.h

Foi usada para controlar o display OLED SSD1306.

## driver/mcpwm.h

Foi usada para acessar o periférico MCPWM da ESP32, utilizado no controle do servo da cancela automática.

---

# 20. Conceitos envolvidos

## PWM

PWM é uma técnica que usa um sinal digital alternando entre nível alto e nível baixo. Variando o tempo em nível alto, é possível controlar brilho, potência ou posição de atuadores.

Neste projeto, o PWM foi usado para controlar o LED RGB e os servomotores.

## Duty cycle

Duty cycle é a porcentagem do período em que o sinal PWM fica em nível alto. Na Parte 1, ele foi usado para controlar o brilho de cada cor do LED RGB.

## ADC

O ADC converte uma tensão analógica em um valor digital. Ele foi usado para ler o potenciômetro no exercício do servo.

## UART

A comunicação serial UART foi usada no monitor serial do Wokwi para mostrar valores como duty cycle, leitura ADC, ângulo do servo e estado da cancela.

## I2C

O I2C foi usado para comunicação com o display OLED. Os pinos usados foram GPIO 21 para SDA e GPIO 22 para SCL.

## MCPWM

O MCPWM é um periférico da ESP32 usado para gerar sinais PWM em aplicações com motores e atuadores. No projeto, ele foi usado para controlar o servo da cancela automática.

---

# 21. Discussão dos resultados

Na Parte 1, o LED RGB mudou de cor continuamente. Isso mostrou que os três sinais PWM estavam funcionando de forma independente. O monitor serial mostrou os valores de duty cycle e os incrementos de cada cor.

Na Parte 2 - Exercício 1, o potenciômetro controlou corretamente a posição do servo. Quando o valor do ADC aumentava, o ângulo do servo também aumentava.

Na Parte 2 - Exercício 2, a cancela automática funcionou conforme esperado. O botão alternou entre abrir e fechar, o servo realizou o movimento, o buzzer emitiu o aviso e o OLED mostrou o estado atual.

Os testes no Wokwi permitiram verificar o funcionamento dos códigos e das ligações dos circuitos.

---

# 22. Conclusão

O projeto permitiu aplicar vários recursos da ESP32 em simulações práticas. Foram usados PWM, ADC, UART, I2C, display OLED e MCPWM.

A ESP32 foi adequada para esse projeto porque possui vários periféricos integrados e permite controlar diferentes dispositivos ao mesmo tempo. Mesmo assim, para aplicações muito simples, um microcontrolador mais básico também poderia ser suficiente.

---
