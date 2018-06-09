/*This sketch explains the working of Personal Hydration Meter
 * Works on priciple of electric conductivity of urine
 */

//Libraries
#include "ESP8266WiFi.h"
#include <LiquidCrystal_I2C.h>
#include<Wire.h>
#include <SPI.h>
#include <MFRC522.h>

//LCD
LiquidCrystal_I2C lcd(0x3F, 16, 2);

//rfid 
#define RST_PIN 0
#define SS_PIN 2
MFRC522 mfrc522(SS_PIN, RST_PIN);

//sensor pin declaration
int pin1 = A0;
int sensorPower = D8;
int value;
String user;

//Pushing Box Credentials
const char WEBSITE[] = "api.pushingbox.com";
const String devid = "v028FA2AB5025CBC";

//Wifi Credentials
const char* MY_SSID = "Reap_Benefit_2.4";
const char* MY_PWD =  "solvesmalldentbig";

void setup() {
  Serial.begin(115200);

  //Calling function to connect to Wifi
  wificonnect(); 

  pinMode(sensorPower, OUTPUT);

  //Initializing LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  
  //Initializing RFID
  SPI.begin();
  mfrc522.PCD_Init(); 
  Serial.println("Approximate your card to the reader:");
  Serial.println();
}

void loop() {
  
  testdata();


}

void testdata() {
  lcd.setCursor(0, 0);
  lcd.print("Hydration Meter");

  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();

  if (content.substring(1) == "3C 74 AA 29") {
    user = "Gautam";
    displaycontent();
  }

  if (content.substring(1) == "BE 84 46 C2") {
    user = "Kuldeep";
    displaycontent();
  }

  if (content.substring(1) == "CA 3E B8 B4") {
    user = "Sairam";
    displaycontent();
  }

  if (content.substring(1) == "AE 39 38 62") {
    user = "Kumar";
    displaycontent();
  }

  if (content.substring(1) == "8E 46 47 C2") {
    user = "Amarnath";
    displaycontent();
  }

  if (content.substring(1) == "4E B7 47 62") {
    user = "Shashi";
    displaycontent();
  }

  if (content.substring(1) == "0C 64 9A FE") {
    user = "New User";
    displaycontent();
  }

  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Not Recognized");
    delay(3000);
    lcd.clear();
  }
}

void wificonnect() {
  Serial.print("Connecting to " + *MY_SSID);
  WiFi.begin(MY_SSID, MY_PWD);
  Serial.println("going into wl connect");
  while (WiFi.status() != WL_CONNECTED) //not connected,..waiting to connect
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("wl connected");
  Serial.println("");
  Serial.println("Credentials accepted! Connected to wifi\n ");
  Serial.println("");
}

void pushbox() {
  WiFiClient client;

  if (client.connect(WEBSITE, 80))
  {
    client.print("GET /pushingbox?devid=" + devid
                 + "&user=" + String(user)
                 + "&value=" + String(value)

                );

    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(WEBSITE);
    client.println("User-Agent: ESP8266/1.0");
    client.println("Connection: close");
    client.println();
  }
}

void displaycontent() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(user);
  Serial.println(user);
  Serial.println();
  delay(15000);
  digitalWrite(sensorPower, HIGH);
  delay(3000);
  value =  analogRead(pin1);
  digitalWrite(sensorPower, LOW);
  pushbox();
  lcd.setCursor(0, 1);
  lcd.print(value);
  Serial.println(value);
  delay(8000);
  lcd.clear();

}