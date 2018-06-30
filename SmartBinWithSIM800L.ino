#include <SoftwareSerial.h>

SoftwareSerial SMS(9, 10);// GSM Tx(9) and GSM Rx(10)

const int trigPin = 4;
const int echoPin = 5;

long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  SMS.begin(9600);
}

void sendmessage()
{
  SMS.println("AT+CMGF=1"); //AT command for text mode of gsm module
  delay(1000);
  SMS.println("AT+CMGS=\"+919994966105\"\r"); //  x is your mobile number
  delay(1000);
  SMS.println("Garbage can is full! Please clear"); //  x is your mobile number
  delay(1000);
  SMS.println((char)26);// ASCII code of CTRL+Z
  delay(10000);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  delay(5000);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034 / 2;

  Serial.println(distance);
  if (distance <= 50)
  {
    sendmessage();
  }
  else
  {
    return;
  }
}