#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <LiquidCrystal_I2C.h>
#include<Wire.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 2
#define RST_PIN 0
MFRC522 mfrc522(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;


LiquidCrystal_I2C lcd(0x3F, 16, 2); // or 0x3F

const char* ssid = "ReapBenefit";                 //Your Wifi SSID
const char* password = "solvesmalldentbig";               //Wifi password

//For pushing data into google sheet
const char* host = "script.google.com";                 //Do Not Change
const int httpsPort = 443;                //Do Not Change

WiFiClientSecure client;

// SHA1 fingerprint of the certificate, do not change
const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";

String GAS_ID = "AKfycbzYAanq99TNhKtw91ZmW2xO1VGF0w4dI5PA9z5v-36dNrnrt5i3"; //GAS ID is obtained after deploying your script as web app

int pin1 = A0;
int sensorPower = D8;
int value;
String cardid = "";

void setup() {
  Serial.begin(115200);

  pinMode(pin1, INPUT);
  pinMode(sensorPower, OUTPUT);

  //Initializing LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hydration Meter");

  SPI.begin();
  mfrc522.PCD_Init();
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
  //lcd.setCursor(0, 1);
  //lcd.print("Wifi Connected");
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


void printid(byte *buffer, byte bufferSize) {
  cardid = "";
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");

    Serial.print(buffer[i], HEX);

    cardid += (buffer[i]);

  }
  Serial.println();
  Serial.println("Card ID");
  Serial.println(cardid);
}


void loop() {

  if (mfrc522.PICC_IsNewCardPresent())
  {

    if (mfrc522.PICC_ReadCardSerial())
    {
      printid(mfrc522.uid.uidByte, mfrc522.uid.size);
      
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
      delay(500);

      wificonnect();
      delay(500);

      sendData(value, cardid);
      delay(5000);


    }
    ESP.restart();
  }


  mfrc522.PICC_HaltA();


  mfrc522.PCD_StopCrypto1();


}
