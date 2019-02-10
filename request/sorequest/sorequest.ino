#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const int DHTPin = 5;

void setup() {
  Serial.begin(9600);
  connectInternet();
}

void requestViaCep() {
  HTTPClient http;

  http.begin("http://viacep.com.br/ws/06844020/json/");
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

  http.end(); //Free the resources
}

void connectInternet() {

  const char* ssid = "root@op6#";
  const char* password =  "r00t@f3rr0r#";

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("root@log# connecting wifi");
  }
  Serial.println("root@log# wifi connected");
}

void loop() {
  requestViaCep();
  delay(10000);
}
