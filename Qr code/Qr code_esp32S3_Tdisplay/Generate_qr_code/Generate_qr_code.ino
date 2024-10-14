#include <SPI.h>
#include <TFT_eSPI.h>
#include <qrcode_espi.h>
#include "qrcode.h"

TFT_eSPI tft = TFT_eSPI();  
QRcode_eSPI qrcode(&tft);   

#define PIN_POWER_ON 15
#define PIN_LCD_BL 38

void setup() {
  Serial.begin(115200);  
  Serial.println("Iniciando...");

  pinMode(PIN_POWER_ON, OUTPUT);
  pinMode(PIN_LCD_BL, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);
  digitalWrite(PIN_LCD_BL, HIGH);

  Serial.println("Configurando Display...");
  tft.init();
  Serial.println("Display Inicializado.");

  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);  
  Serial.println("Tela preenchida com preto.");

  Serial.println("Inicializando QR Code...");
  qrcode.init();  
  Serial.println("QR Code Inicializado.");


  Serial.println("Criando QR Code...");
  qrcode.create("https://icoleta.netlify.app/login");  
  Serial.println("QR Code Criado.");
  
}

void loop() {
  // Código do loop aqui
}

