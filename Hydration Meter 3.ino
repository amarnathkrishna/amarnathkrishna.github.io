/* This code explains the working of hydration meter, that uses the principle of conductivity of urine, to measure dehydration.*/
//This uses GScript to push data on to a Google sheet.

//Libraries
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <LiquidCrystal_I2C.h>
#include<Wire.h>
#include <SPI.h>
#include <MFRC522.h>

//Creating LCD instance
LiquidCrystal_I2C lcd(0x27, 16, 2); // or 0x3F

//Creating RFID instance
#define SS_PIN 2
#define RST_PIN 0
MFRC522 mfrc522(SS_PIN, RST_PIN);

//Wifi Credentials
const char* ssid = "ReapBenefit_Act_Upstairs";                 //Your Wifi SSID
const char* password = "solvesmalldentbig";               //Wifi password

//For pushing data into google sheet
const char* host = "script.google.com";                 //Do Not Change
const int httpsPort = 443;                //Do Not Change

WiFiClientSecure client;

// SHA1 fingerprint of the certificate, do not change
const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";

String GAS_ID = "AKfycbwxosqLamJ50n63AK43D4Lm57PyeuZxmf4HDvCinIbPaQkGFIZi"; //GAS ID is obtained after deploying your script as web app

//Initializing sensor pins
int pin1 = A0;
int sensorPower = D8;
int value;
String user1;

void setup()
{
  Serial.begin(115200);

  pinMode(pin1, INPUT);
  pinMode(sensorPower, OUTPUT);

  //Initializing LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hydration Meter");

  wificonnect();

  //Initializing RFID
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Approximate your card to the reader:");
  Serial.println();

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
  lcd.setCursor(0, 1);
  lcd.print("Wifi Connected");
}

void loop()
{
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    //Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();

  if (content.substring(1) == "8E 46 47 C2")
  {

    user1 = "Amarnath";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(user1);
    delay(15000);

    digitalWrite(sensorPower, HIGH);
    delay(2000);
    value =  analogRead(pin1);
    digitalWrite(sensorPower, LOW);

    lcd.setCursor(0, 1);
    lcd.print(value);

    sendData(value, user1);

    delay(5000);

    ESP.restart();
  }

  if (content.substring(1) == "66 25 78 89")
  {

    user1 = "Rutuja";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(user1);
    delay(15000);

    digitalWrite(sensorPower, HIGH);
    delay(2000);
    value =  analogRead(pin1);
    digitalWrite(sensorPower, LOW);

    lcd.setCursor(0, 1);
    lcd.print(value);

    sendData(value, user1);

    delay(5000);

    ESP.restart();

  }

  if (content.substring(1) == "76 06 8E 79")
  {

    user1 = "Pradnya";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(user1);
    delay(15000);

    digitalWrite(sensorPower, HIGH);
    delay(2000);
    value =  analogRead(pin1);
    digitalWrite(sensorPower, LOW);

    lcd.setCursor(0, 1);
    lcd.print(value);

    sendData(value, user1);

    delay(5000);

    ESP.restart();
  }

  if (content.substring(1) == "0C 64 9A FE")
  {

    user1 = "Maria";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(user1);
    delay(15000);

    digitalWrite(sensorPower, HIGH);
    delay(2000);
    value =  analogRead(pin1);
    digitalWrite(sensorPower, LOW);

    lcd.setCursor(0, 1);
    lcd.print(value);

    sendData(value, user1);

    delay(5000);

    ESP.restart();

  }
  if (content.substring(1) == "3C 74 AA 29") {
    user1 = "Gautam";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(user1);
    delay(15000);

    digitalWrite(sensorPower, HIGH);
    delay(2000);
    value =  analogRead(pin1);
    digitalWrite(sensorPower, LOW);

    lcd.setCursor(0, 1);
    lcd.print(value);

    sendData(value, user1);

    delay(5000);

    ESP.restart();

  }
  if (content.substring(1) == "BE 84 46 C2") {
    user1 = "Kuldeep";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(user1);
    delay(15000);

    digitalWrite(sensorPower, HIGH);
    delay(2000);
    value =  analogRead(pin1);
    digitalWrite(sensorPower, LOW);

    lcd.setCursor(0, 1);
    lcd.print(value);

    sendData(value, user1);

    delay(5000);

    ESP.restart();

  }
  if (content.substring(1) == "CA 3E B8 B4") {
    user1 = "Sairam";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(user1);
    delay(15000);

    digitalWrite(sensorPower, HIGH);
    delay(2000);
    value =  analogRead(pin1);
    digitalWrite(sensorPower, LOW);

    lcd.setCursor(0, 1);
    lcd.print(value);

    sendData(value, user1);

    delay(5000);

    ESP.restart();

  }
  if (content.substring(1) == "AE 39 38 62") {
    user1 = "Kumar";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(user1);
    delay(15000);

    digitalWrite(sensorPower, HIGH);
    delay(2000);
    value =  analogRead(pin1);
    digitalWrite(sensorPower, LOW);

    lcd.setCursor(0, 1);
    lcd.print(value);

    sendData(value, user1);

    delay(5000);

    ESP.restart();

  }
  if (content.substring(1) == "4E B7 47 62") {
    user1 = "Shashi";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(user1);
    delay(15000);

    digitalWrite(sensorPower, HIGH);
    delay(2000);
    value =  analogRead(pin1);
    digitalWrite(sensorPower, LOW);

    lcd.setCursor(0, 1);
    lcd.print(value);

    sendData(value, user1);

    delay(5000);

    ESP.restart();

  }

  if (content.substring(1) == "C2 9E CF E9")
  {

    user1 = "Sankarshan";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(user1);
    delay(15000);

    digitalWrite(sensorPower, HIGH);
    delay(2000);
    value =  analogRead(pin1);
    digitalWrite(sensorPower, LOW);

    lcd.setCursor(0, 1);
    lcd.print(value);

    sendData(value, user1);

    delay(5000);

    ESP.restart();

  }

  if (content.substring(1) == "F2 5C 60 6D")
  {

    user1 = "Rizwan";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(user1);
    delay(15000);

    digitalWrite(sensorPower, HIGH);
    delay(2000);
    value =  analogRead(pin1);
    digitalWrite(sensorPower, LOW);

    lcd.setCursor(0, 1);
    lcd.print(value);

    sendData(value, user1);

    delay(5000);

    ESP.restart();

  }

}

// Function for Send data into Google Spreadsheet
void sendData(int val, String user)
{
  Serial.print("connecting to ");
  Serial.println(host);
  if (client.connect(host, httpsPort)) 
  {

    if (client.verify(fingerprint, host)) {
      Serial.println("certificate matches");
    } else {
      Serial.println("certificate doesn't match");
    }

    String string_value =  String(val, DEC);
    String string_user = String(user);

    String url = "/macros/s/" + GAS_ID + "/exec?value=" + string_value + "&user=" + string_user ;
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
    Serial.println("reply was:");
    Serial.println("==========");
    Serial.println(line);
    Serial.println("==========");
    Serial.println("closing connection");
  }
  else {
    Serial.println("connection failed");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Not recorded");
    delay(3000);
    return;
  }
}