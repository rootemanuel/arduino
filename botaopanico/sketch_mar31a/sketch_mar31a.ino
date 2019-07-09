#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

int ledPin = 2;
int buttonPin = 27;

void setup() {
  Serial.begin(115200);

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  connectInternet();
}

void requestBotao() {
  HTTPClient http;

  http.begin("https://onesignal.com/api/v1/notifications");
  int httpCode = http.GET();

  Serial.println("## --------------------------------------------------------- ##");
  Serial.println(httpCode);
  Serial.println("## --------------------------------------------------------- ##");

  if (httpCode > 0) {
    StaticJsonBuffer<300> jsonBuffer;
    String payload = http.getString();
    JsonObject& parsed = jsonBuffer.parseObject(payload);

    Serial.println(parsed["logradouro"].as<char*>());

    Serial.println("## --------------------------------------------------------- ##");
    Serial.println("root@log# inicio payload body");
    Serial.println(payload);
    Serial.println("## --------------------------------------------------------- ##");
  }

  http.end();
}

void connectInternet() {
  const char* ssid = "FRANCISCO";
  const char* password =  "francisco2018";

  WiFi.begin(ssid, password);

  piscaLed(1);
  while (WiFi.status() != WL_CONNECTED) {
    piscaLed(1);
    delay(2000);
    Serial.println("root@log# connecting wifi");
  }
  Serial.println("root@log# wifi connected");
  piscaLed(2);
}

void sendPedido() {
  String json = "{\"id\":\"ESP32\"}";
  HTTPClient http;  

  piscaLed(4);
  http.begin("http://192.168.2.5:8080/pedido");
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(json);

  Serial.println("## POST PUSH - httpCode##");
  Serial.println(httpCode);
}

void piscaLed(int count) {
  for (int i = 0; i < count; i++) {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
}

void loop() {
  if (digitalRead(buttonPin) == HIGH) {
    sendPedido();
  }
}
