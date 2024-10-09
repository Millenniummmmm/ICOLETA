#include <Arduino.h>
#include "qrcode.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// CÓDIGO EM TESTE
// Defina as dimensões do display OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // Definir como -1 se não estiver utilizando o pino de reset

// Inicialize o display OLED usando a biblioteca Adafruit
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
    // Inicie a comunicação serial
    Serial.begin(115200);

    // Inicie o display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("Falha ao inicializar o display OLED"));
        for (;;);  // Trava se não conseguir iniciar o display
    }
    
    // Limpe o buffer do display
    display.clearDisplay();
    
    // Exiba uma mensagem inicial
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Gerando QR Code"));
    display.display();
    
    // Criação do QR code
    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(3)];
    qrcode_initText(&qrcode, qrcodeData, 3, 0, "HELLO WORLD");
  
    // Delta time
    uint32_t dt = millis();
    dt = millis() - dt;
    Serial.print("Tempo de Geracao do QR Code: ");
    Serial.println(dt);

    // Tamanho de cada módulo (quadrado) no display OLED
    int moduleSize = 2;  // Ajuste o tamanho conforme necessário
    
    // Desenhe o QR code no display OLED
    for (uint8_t y = 0; y < qrcode.size; y++) {
        for (uint8_t x = 0; x < qrcode.size; x++) {
            // Desenhe um pixel preto ou branco para cada módulo do QR code
            if (qrcode_getModule(&qrcode, x, y)) {
                display.fillRect(x * moduleSize, y * moduleSize, moduleSize, moduleSize, SSD1306_WHITE); // Módulo preto
            } else {
                display.fillRect(x * moduleSize, y * moduleSize, moduleSize, moduleSize, SSD1306_BLACK); // Módulo branco
            }
        }
    }

    // Atualize o display para mostrar o QR code
    display.display();
}

void loop() {
  // O loop está vazio, pois o QR code é gerado apenas uma vez no setup
}
