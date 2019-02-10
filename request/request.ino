#include <Wire.h>  
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "SSD1306Wire.h"

SSD1306Wire display(0x3c, 5, 4);

void setup() {
  Serial.begin(115200);

  display.init();
  display.flipScreenVertically();

  printDavi();
  //connectInternet();
  //requestViaCep();
}

void requestViaCep() {
  StaticJsonBuffer<500> jsonBuffer;
  HTTPClient http;

  http.begin("http://viacep.com.br/ws/06844020/json/");
  int httpCode = http.GET();

  Serial.print("root@log# http code: ");
  Serial.print(httpCode);
  if (httpCode > 0) {
    String payload = http.getString();
    JsonObject& root = jsonBuffer.parseObject(payload);
 
    Serial.println("root@log# inicio payload body");
    Serial.println("## --------------------------------------------------------- ##");
    Serial.println(payload);
    Serial.println("## --------------------------------------------------------- ##");

    printCEP(root);
  }

  http.end(); //Free the resources
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

void printDisplay(const char* text) {
    display.clear();
    
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_24);
    display.drawString(0, 0, String(text));

    display.display();
}

void printCEP(JsonObject& json) {
  
  const char * cep = json["cep"].as<char*>();
  const char * logradouro = json["logradouro"].as<char*>();
  const char * complemento = json["complemento"].as<char*>();
  const char * bairro = json["bairro"].as<char*>();
  const char * localidade = json["localidade"].as<char*>();
  const char * uf = json["uf"].as<char*>();
  const char * unidade = json["unidade"].as<char*>();
  const char * ibge = json["ibge"].as<char*>();
  const char * gia = json["gia"].as<char*>();
  
  const char * valores[] = {cep, logradouro, complemento, bairro, localidade, uf, ibge, gia};

  for (int i=0; i <= sizeof(valores); i++){
    printDisplay(valores[i]);
    delay(1500);

    if(i == sizeof(valores)){
      i = -1;
    }
  }
}

void printDavi(){  
  const char * valores[] = {"jayce", "rocky", "rider", "zuma", "sky", "marshal", "rumble", "capitao \n rodovalho"};

  for (int i=0; i <= sizeof(valores); i++){
    printDisplay(valores[i]);
    delay(1500);

    if(i == sizeof(valores)){
      i = -1;
    }
  }
}

void loop(){
}

