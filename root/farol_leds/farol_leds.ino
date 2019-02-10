int ledRed = 15;
int ledYellow = 2;
int ledGreen = 4;

void setup()
{
    pinMode(ledRed, OUTPUT);
    pinMode(ledYellow, OUTPUT);
    pinMode(ledGreen, OUTPUT);
    Serial.begin(115200);
}

void loop()
{
    Serial.println("Hello ESP32!"); 
    digitalWrite(ledRed, HIGH);
    delay(50);
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, HIGH);
    delay(50);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, HIGH);
    delay(50);
    digitalWrite(ledGreen, LOW);
}
