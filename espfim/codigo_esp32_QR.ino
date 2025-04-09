#include <SPI.h>
#include <TFT_eSPI.h>
#include <qrcode_espi.h>
#include <ArduinoJson.h>
#include "Base64.h"
#include "mbedtls/md.h"

// === Constantes e configurações ===
#define PIN_POWER_ON 15
#define PIN_LCD_BL 38
#define LIXEIRA_ID "LIX123456"

const char* secret = ">E82bw:6wrOg9'VyS^;$£@NkaBZ6z}Pm5uYKBKrD^h8>,{s[uh";

TFT_eSPI tft = TFT_eSPI();
QRcode_eSPI qrcode(&tft);

bool qrCodeGerado = false;

// === Inicialização da tela ===
void inicializarTela() {
  pinMode(PIN_POWER_ON, OUTPUT);
  pinMode(PIN_LCD_BL, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);
  digitalWrite(PIN_LCD_BL, HIGH);

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  qrcode.init();
}

// === Utilitários de JWT ===
String base64UrlEncode(String input) {
  String encoded = base64::encode(input);
  encoded.replace("+", "-");
  encoded.replace("/", "_");
  encoded.replace("=", "");
  return encoded;
}

String signHMACSHA256(String data, const char* key) {
  unsigned char hmacResult[32];
  mbedtls_md_context_t ctx;
  const mbedtls_md_info_t* info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);

  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, info, 1);
  mbedtls_md_hmac_starts(&ctx, (const unsigned char*)key, strlen(key));
  mbedtls_md_hmac_update(&ctx, (const unsigned char*)data.c_str(), data.length());
  mbedtls_md_hmac_finish(&ctx, hmacResult);
  mbedtls_md_free(&ctx);

  String signature;
  for (int i = 0; i < 32; i++) {
    signature += (char)hmacResult[i];
  }
  return base64UrlEncode(signature);
}

String gerarJWT(float weight, int point_id) {
  StaticJsonDocument<64> header;
  header["alg"] = "HS256";
  header["typ"] = "JWT";
  String headerStr;
  serializeJson(header, headerStr);
  String headerB64 = base64UrlEncode(headerStr);

  StaticJsonDocument<128> payload;
  payload["weight"] = weight;
  payload["point_id"] = point_id;
  String payloadStr;
  serializeJson(payload, payloadStr);
  String payloadB64 = base64UrlEncode(payloadStr);

  String dataToSign = headerB64 + "." + payloadB64;
  String signatureB64 = signHMACSHA256(dataToSign, secret);

  return dataToSign + "." + signatureB64;
}

// === Geração de URL ===
String gerarURL(String jwt) {
  return "http://localhost:3000/descarte/" + jwt;
  // ou apenas: return jwt; // caso só queira o token
}

// === Exibir QR Code ===
void exibirQRCode(String url) {
  tft.fillScreen(TFT_BLACK);
  qrcode.create(url.c_str());
  Serial.println("QR Code exibido na tela.");
}

// === Função principal unificada ===
void mostrarQRCode(float weight, int point_id) {
  Serial.println("Gerando JWT...");
  String jwt = gerarJWT(weight, point_id);
  Serial.println("JWT Gerado:");
  Serial.println(jwt);

  String url = gerarURL(jwt);
  Serial.println("URL Gerada:");
  Serial.println(url);

  exibirQRCode(url);
}

// === Setup e loop ===
void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando sistema...");
  inicializarTela();

  if (!qrCodeGerado) {
    mostrarQRCode(5.3, 42); // você pode trocar por valores reais futuramente
    qrCodeGerado = true;
  }
}

void loop() {
  // Nada aqui — gerado uma vez no setup
}
