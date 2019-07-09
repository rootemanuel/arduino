#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

#define DHTPIN 23 // pino de dados do DHT11
#define DHTTYPE DHT11 // define o tipo de sensor, no caso DHT11

DHT dht(DHTPIN, DHTTYPE);

int bugSeila = 2147483647;
int ledPin = 2;
int buttonPin = 27;

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

  connectInternet();
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

void sendTemp(int temp, int hum) {
  String json = "{\"temp\":\"" + String(temp) + "\",\"hum\":\"" + String(hum) + "\"}";
  HTTPClient http;

  piscaLed(4);
  http.begin("http://192.168.2.13:8080/temp");
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
  int hum;
  int temp;

  hum = dht.readHumidity();
  temp = dht.readTemperature();

  if (!isnan(temp) && !isnan(hum))
  {
    if ((temp != bugSeila) && (hum != bugSeila)) {
      sendTemp(temp, hum);
      delay(5000);
    }
  }
}
