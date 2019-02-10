#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

void setup() {
  Serial.begin(115200);

  dht.begin();
  connectInternet();
}

void requestViaCep() {
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

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("root@log# connecting wifi");
  }
  Serial.println("root@log# wifi connected");
}

void sendPush(String title, String msg) {
  String json = "{\"app_id\":\"4dd3313c-a425-4b89-b59a-d8a0a060d70d\",\"headings\":{\"en\":\"" + title + "\"},\"contents\":{\"en\":\"" + msg + "\"},\"include_player_ids\":[\"a86eaabd-e425-428e-b00b-85329e0afb97\"]}";
  HTTPClient http;

  http.begin("https://onesignal.com/api/v1/notifications");
  http.addHeader("Content-Type","application/json");
  int httpCode = http.POST(json);

  Serial.println("## --------------------------------------------------------- ##");
  Serial.println(httpCode);
  Serial.println("## --------------------------------------------------------- ##");
}

void loop() {
  sendPush("Title","aee PORRA!");
  delay(100000000);
}
