#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <LiquidCrystal_I2C.h>
#include<Wire.h>

int pir = D6; //pir input
int value;
int pin1 = A0;
int sensorPower = D8;
String cardid = "Amarnath"; //Only required for the current Google Sheet

//Creating LCD instance
LiquidCrystal_I2C lcd(0x27, 16, 2); // or 0x3F

const char* ssid = "ReapBenefit";
const char* password = "solvesmalldentbig";

//For pushing data into google sheet
const char* host = "script.google.com";                 //Do Not Change
const int httpsPort = 443;                //Do Not Change

WiFiClientSecure client;

// SHA1 fingerprint of the certificate, do not change
const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";

String GAS_ID = "AKfycbzYAanq99TNhKtw91ZmW2xO1VGF0w4dI5PA9z5v-36dNrnrt5i3"; //GAS ID is obtained after deploying your script as web app


void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pir, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hydration Meter");

}

void wificonnect()
{
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void sendData(int val, String cardid)
{
  Serial.print("connecting to ");
  Serial.println(host);
  if (client.connect(host, httpsPort)) {
    if (client.verify(fingerprint, host)) {
      Serial.println("certificate matches");
    } else {
      Serial.println("certificate doesn't match");
    }

    String string_value =  String(val, DEC);
    String string_cardid = String(cardid);
    String url = "/macros/s/" + GAS_ID + "/exec?value=" + string_value + "&cardid=" + string_cardid ;
    Serial.print("requesting URL: ");
    Serial.println(url);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "User-Agent: BuildFailureDetectorESP8266\r\n" +
                 "Connection: close\r\n\r\n");

    Serial.println("request sent");
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }
    String line = client.readStringUntil('\n');
    if (line.startsWith("{\"state\":\"success\"")) {
      Serial.println("esp8266/Arduino CI successfull!");
    } else {
      Serial.println("esp8266/Arduino CI has failed");
    }
    //Serial.println("reply was:");
    //Serial.println("==========");
    //Serial.println(line);
    //Serial.println("==========");
    Serial.println("closing connection");
  }
  else {
    Serial.println("connection failed");
    return;
  }
}

void loop() {
  delay(500);
  int state = digitalRead(pir);
  delay(1500);
  Serial.println("pir");
  if (state == 1)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Motion Detected");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Welcome");

    delay(15000);

    digitalWrite(sensorPower, HIGH);
    delay(2000);
    value =  analogRead(pin1);
    digitalWrite(sensorPower, LOW);
    delay(500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Your value is:");
    lcd.setCursor(0, 1);
    lcd.print(value);
    Serial.println("Your hydration value is:");
    Serial.println(value);

    wificonnect();
    delay(500);

    sendData(value, cardid);
    delay(5000);

    setup();
  }
  else
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("No motion");

  }

}