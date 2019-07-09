#include <Adafruit_NeoPixel.h>
#include <M5Stack.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define BLACK           0x0000
#define RED             0xF800
#define CYAN            0x07FF
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

#define POS_X_GRAFICO       30
#define POS_Y_GRAFICO       3
#define ALTURA_GRAFICO      180
#define COMPRIMENTO_GRAFICO 270

#define POS_X_DADOS 30
#define POS_Y_DADOS 200

#define M5STACK_FIRE_NEO_NUM_LEDS 10
#define M5STACK_FIRE_NEO_DATA_PIN 15

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(M5STACK_FIRE_NEO_NUM_LEDS, M5STACK_FIRE_NEO_DATA_PIN, NEO_GRB + NEO_KHZ800);

int COLOR_RED_RGB [3] = {255, 0, 0};
int COLOR_GREEN_RGB [3] = {51, 204, 51};
int COLOR_WHITE_RGB [3] = {255, 255, 255};

void setup() {
  beginM5();
  connectInternet();
}

void connectInternet() {
  const char* ssid = "FRANCISCO";
  const char* password =  "francisco2018";

  WiFi.begin(ssid, password);

  lcd_clear();
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("root@log#");
  M5.Lcd.setCursor(0, 20);
  M5.Lcd.print("connecting wifi");

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED) {

    M5.Lcd.print(".");
    delay(1000);

    if (tentativas > 8) {
      exibirLed(1000, COLOR_RED_RGB[0], COLOR_RED_RGB[1], COLOR_RED_RGB[2]);
      M5.Lcd.print("root@log# wifi connected");
      break;
    }

    tentativas++;
  }

  lcd_clear();
  M5.Lcd.setCursor(0, 0);
  if (WiFi.status() == WL_CONNECTED) {
    exibirLed(1000, COLOR_GREEN_RGB[0], COLOR_GREEN_RGB[1], COLOR_GREEN_RGB[2]);
    M5.Lcd.print("root@log# wifi connected");
  }
}

void exibirLed(int t, int r, int g, int b ) {
  for (int n = 0; n < 10; n++)pixels.setPixelColor(n, pixels.Color(r, g, b));
  pixels.show();

  delay(t);
  for (int n = 0; n < 10; n++)pixels.setPixelColor(n, pixels.Color(0, 0, 0));
  pixels.show();
}

void beginM5() {

  pixels.begin();

  M5.begin();
  M5.Lcd.setBrightness(100);
  M5.Lcd.setTextSize(5);
  M5.Lcd.setCursor(100, 100);
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.printf("r00t");
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.printf("?");
  M5.Lcd.setTextSize(2);
  delay(2000);
}

void show(int tela = 0) {

  if (tela == 0) {
    lcd_clear();
    M5.Lcd.setCursor(0, 0);
    exibirLed(1000, COLOR_WHITE_RGB[0], COLOR_WHITE_RGB[1], COLOR_WHITE_RGB[2]);
    requestViaCep();
  }

  if (tela == 1) {
    lcd_clear();
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print("Camila Gonsales Parente");
    exibirLed(1000, COLOR_WHITE_RGB[0], COLOR_WHITE_RGB[1], COLOR_WHITE_RGB[2]);
  }

  if (tela == 2) {
    lcd_clear();
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print("Francisco Emanuel Pereira Parente");
    exibirLed(1000, COLOR_WHITE_RGB[0], COLOR_WHITE_RGB[1], COLOR_WHITE_RGB[2]);
  }

}

void requestViaCep() {
  HTTPClient http;

  String url = "http://viacep.com.br/ws/06844020/json/";
  http.begin(url);
  int httpCode = http.GET();

  lcd_clear();
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("root@log# request url => ");
  M5.Lcd.print(url);
  delay(5000);

  if (httpCode == 200) {
    DynamicJsonBuffer jsonBuffer;

    String payload = http.getString();
    JsonObject& parsed = jsonBuffer.parseObject(payload);

    lcd_clear();
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print(payload);
    delay(5000);
  }

  http.end();
}

void lcd_clear() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
}

void loop() {
  if (M5.BtnA.wasPressed()) {
    show(0);
  }

  if (M5.BtnB.wasPressed()) {
    show(1);
  }

  if (M5.BtnC.wasPressed()) {
    show(2);
  }

  M5.update();
}
