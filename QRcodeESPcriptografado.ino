#include <SPI.h>
#include <TFT_eSPI.h>
#include <qrcode_espi.h>
#include <ArduinoJson.h>
#include "Base64.h"

TFT_eSPI tft = TFT_eSPI();
QRcode_eSPI qrcode(&tft);

#define PIN_POWER_ON 15
#define PIN_LCD_BL 38


// 🗑️ Defina um ID FIXO para esta lixeira
#define LIXEIRA_ID "LIX123456" 

//Flag para garantir que o QR Code só seja gerado uma vez
bool qrCodeGerado = false;  

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando...");

  pinMode(PIN_POWER_ON, OUTPUT);
  pinMode(PIN_LCD_BL, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);
  digitalWrite(PIN_LCD_BL, HIGH);

  Serial.println("Configurando Display...");
  tft.init();
  tft.setCursor(165, 0);
  Serial.println("Display Inicializado.");

  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  Serial.println("Tela preenchida com preto.");

  Serial.println("Inicializando QR Code...");
  qrcode.init();
  Serial.println("QR Code Inicializado.");

  // Gera o QR Code apenas uma vez
  if (!qrCodeGerado) {
    gerarQRCode();  
    qrCodeGerado = true;
  }
}

void loop() {
  // O QR Code só é gerado uma vez
}

void gerarQRCode() {
  Serial.println("Gerando QR Code...");

  //JSON único e id fixo (lixeira) e timestamp variável
  StaticJsonDocument<256> jsonDoc;
  jsonDoc["url"] = "https://icoleta.netlify.app/login";
  jsonDoc["id_lixeira"] = LIXEIRA_ID;
  jsonDoc["timestamp"] = millis(); 

  String jsonString;
  serializeJson(jsonDoc, jsonString);

  Serial.println("JSON Gerado:");
  Serial.println(jsonString);

  // Criptografando JSON com Base64
  String encodedJson = base64::encode(jsonString);
  Serial.println("JSON Criptografado:");
  Serial.println(encodedJson);

  Serial.println("Criando QR Code...");
  qrcode.create(encodedJson.c_str());
  Serial.println("QR Code Criado.");
}
