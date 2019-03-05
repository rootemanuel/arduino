#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

String cryptos[] = {
  "ETHBTC", "LTCBTC", "BNBBTC", "NEOBTC", "BCCBTC", "GASBTC", "HSRBTC", "MCOBTC", "WTCBTC", "LRCBTC", "QTUMBTC", "YOYOBTC", "OMGBTC", "ZRXBTC", "STRATBTC", "SNGLSBTC",
  "BQXBTC", "KNCBTC", "FUNBTC", "SNMBTC", "IOTABTC", "LINKBTC", "XVGBTC", "SALTBTC", "MDABTC", "MTLBTC", "SUBBTC", "EOSBTC", "SNTBTC", "ETCBTC", "MTHBTC", "ENGBTC",
  "DNTBTC", "ZECBTC", "BNTBTC", "ASTBTC", "DASHBTC", "OAXBTC", "ICNBTC", "BTGBTC", "EVXBTC", "REQBTC", "VIBBTC", "TRXBTC", "POWRBTC", "ARKBTC", "XRPBTC", "MODBTC",
  "ENJBTC", "STORJBTC", "VENBTC", "KMDBTC", "RCNBTC", "NULSBTC", "RDNBTC", "XMRBTC", "DLTBTC", "AMBBTC", "BATBTC", "BCPTBTC", "ARNBTC", "GVTBTC", "CDTBTC", "GXSBTC",
  "POEBTC", "QSPBTC", "BTSBTC", "XZCBTC", "LSKBTC", "TNTBTC", "FUELBTC", "MANABTC", "BCDBTC", "DGDBTC", "ADXBTC", "ADABTC", "PPTBTC", "CMTBTC", "XLMBTC", "CNDBTC",
  "LENDBTC", "WABIBTC", "TNBBTC", "WAVESBTC", "GTOBTC", "ICXBTC", "OSTBTC", "ELFBTC", "AIONBTC", "NEBLBTC", "BRDBTC", "EDOBTC", "WINGSBTC", "NAVBTC", "LUNBTC", "TRIGBTC",
  "APPCBTC", "VIBEBTC", "RLCBTC", "INSBTC", "PIVXBTC", "IOSTBTC", "CHATBTC", "STEEMBTC", "NANOBTC", "VIABTC", "BLZBTC", "AEBTC", "RPXBTC", "NCASHBTC", "POABTC",
  "ZILBTC", "ONTBTC", "STORMBTC", "XEMBTC", "WANBTC", "WPRBTC", "QLCBTC", "SYSBTC", "GRSBTC", "CLOAKBTC", "GNTBTC", "LOOMBTC", "BCNBTC", "REPBTC", "TUSDBTC", "ZENBTC",
  "SKYBTC", "CVCBTC", "THETABTC", "IOTXBTC", "QKCBTC", "AGIBTC", "NXSBTC", "DATABTC", "SCBTC", "NPXSBTC", "KEYBTC", "NASBTC", "MFTBTC", "DENTBTC", "ARDRBTC", "HOTBTC",
  "VETBTC", "DOCKBTC", "POLYBTC", "PHXBTC", "HCBTC", "GOBTC", "PAXBTC", "RVNBTC", "DCRBTC", "USDCBTC", "MITHBTC", "BCHABCBTC", "BCHSVBTC", "RENBTC", "BTTBTC", "ONGBTC", "FETBTC"
};

int ledPin = 2;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

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

  piscaLed(1);
  while (WiFi.status() != WL_CONNECTED) {
    piscaLed(1);
    delay(2000);
    Serial.println("root@log# connecting wifi");
  }
  Serial.println("root@log# wifi connected");
  piscaLed(2);
}

void sendPush(String title, String msg) {
  String json = "{\"app_id\":\"4dd3313c-a425-4b89-b59a-d8a0a060d70d\",\"headings\":{\"en\":\"" + title + "\"},\"contents\":{\"en\":\"" + msg + "\"},\"include_player_ids\":[\"a86eaabd-e425-428e-b00b-85329e0afb97\"]}";
  HTTPClient http;

  piscaLed(4);
  http.begin("https://onesignal.com/api/v1/notifications");
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(json);

  Serial.println("## POST PUSH - httpCode##");
  Serial.println(httpCode);
}

void verificaIPO() {
  HTTPClient http;
  bool resb = true;

  while (resb) {
    http.begin("https://api.binance.com/api/v3/ticker/price");
    int httpCode = http.GET();

    Serial.println("## GET BINANCE - httpCode##");
    Serial.println(httpCode);

    if (httpCode == 200) {
      piscaLed(3);
      resb = false;
      
      DynamicJsonBuffer  jsonBuffer;
      String payload = http.getString();
      JsonArray& cryptor = jsonBuffer.parseArray(payload);

      for (JsonObject& json : cryptor) {
        bool existe = false;
        String cryptoName = json["symbol"].as<char*>();
        String cryptoPrice = json["price"].as<char*>();

        if (cryptoName.indexOf("BTC") > 0) {
          for (String crypto : cryptos) {
            if (crypto == cryptoName) {
              existe = true;
              break;
            }
          }

          if (!existe) {
            sendPush("## R00T IPO", cryptoName + " => " + cryptoPrice);
          }
        }
      }
    }
  }

  http.end();
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
  verificaIPO();
  delay(3600000);
}
