#include <DHT.h>
#define DHTPIN 4 // Analog Pin sensor is connected to

DHT dht(4, DHT11, 20);

float h, t;

#include <SoftwareSerial.h>

SoftwareSerial SMS(9, 10);// rx(9) and tx(10)
// rx(9-arduino) connect to tx(gsm module)
// tx(10-arduino) connect to rx(gsm module)

void setup() {
  SMS.begin(9600);
  Serial.begin(9600);
  delay(500);
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  pinMode(DHTPIN, INPUT);
  dht.begin();
  delay(1000);//Wait before accessing Sensor
}

void sendmessage()
{
  SMS.println("AT+CMGF=1"); //AT command for text mode of gsm module
  delay(1000);
  SMS.println("AT+CMGS=\"+919845811821\"\r"); //  x is your mobile number
  delay(1000);
  SMS.println("Temperature is");
  SMS.println(t);
  SMS.println("Humidity is");
  SMS.println(h);
  delay(100);
  SMS.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void loop() {
  t = dht.readTemperature();
  h = dht.readHumidity();
  Serial.print("Current humidity = ");
  Serial.print(h);
  Serial.print("%  ");
  Serial.print("temperature = ");
  Serial.print(t);
  Serial.println("C  ");

  sendmessage();

  delay(15000);
}