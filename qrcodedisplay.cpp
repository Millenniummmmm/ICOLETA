#include <qrcode.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // Não utilizamos o pino de reset

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
    Serial.begin(115200);

    // Inicializa o display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("Falha ao iniciar OLED"));
        while (true);
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    
    // Start time
    uint32_t dt = millis();
  
    // URL do site a ser direcionado
    const char* url = "https://www.usa.gov/";
    
    // Crie o QR code com a URL
    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(3)];
    qrcode_initText(&qrcode, qrcodeData, 3, 0, url);
  
    // Delta time
    dt = millis() - dt;
    Serial.print("QR Code Generation Time: ");
    Serial.print(dt);
    Serial.print("\n");

    // Limpar display e mostrar QR code
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("QR Code:");

    // Exibir QR code no display
    for (uint8_t y = 0; y < qrcode.size; y++) {
        for (uint8_t x = 0; x < qrcode.size; x++) {
            if (qrcode_getModule(&qrcode, x, y)) {
                display.fillRect(x * 2, y * 2 + 10, 2, 2, WHITE); // Cada módulo é 2x2 pixels
            }
        }
    }

    display.display(); // Atualiza o display para mostrar o QR code
}

void loop() {
    // Nothing to do here
}
