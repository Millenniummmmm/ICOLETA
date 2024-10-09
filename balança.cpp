#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "HX711.h"
//BALANÇA AINDA ESTÁ BUGADA 
#define SCREEN_WIDTH 128 // Largura do display
#define SCREEN_HEIGHT 64  // Altura do display
#define OLED_RESET    -1  // Reset não utilizado

// Cria objeto do display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
HX711 scale;

// Defina os pinos do HX711
const int DOUT_PIN = 18;   // Substitua por um pino disponível no seu ESP32
const int SCK_PIN = 19;    // Substitua por um pino disponível no seu ESP32

void setup() {
  Serial.begin(9600);
  Serial.println("Inicializando a balança");

  // Inicializa o display OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Endereço I2C padrão 0x3C
  display.clearDisplay(); // Limpa o display
  display.setTextSize(1); // Tamanho do texto
  display.setTextColor(WHITE); // Cor do texto

  // Configuração dos pinos do HX711
  scale.begin(DOUT_PIN, SCK_PIN); // DOUT e PD_SCK conforme definido acima

  scale.set_scale(84.f); // Ajuste o fator de calibração conforme necessário
  scale.tare(); // Zera a balança
  delay(1000); // Aguarda um momento para estabilizar
}

void loop() {
  // Lê o peso
  float peso = scale.get_units(1); // Lê o peso com uma casa decimal

  // Se o peso for negativo, trate como 0
  if (peso < 0) {
    peso = 0;
  }

  // Calcula a pontuação
  int pontos = ((peso * 1000) / 50) * 30; // Cada 50g dá 30 pontos

  // Exibe o peso e a pontuação no display OLED
  display.clearDisplay(); // Limpa o display
  display.setCursor(0, 0); // Define a posição do cursor
  display.print("Peso: ");
  display.print(peso, 1); // Exibe o peso com uma casa decimal
  display.println(" kg");

  display.setCursor(0, 30); // Define a posição do cursor para a pontuação
  display.print("Pontuacao: ");
  display.print(pontos); // Exibe a pontuação
  display.println(" pts");

  display.display(); // Atualiza o display
  
  delay(2000); // Atualiza a cada 5 segundos
}
