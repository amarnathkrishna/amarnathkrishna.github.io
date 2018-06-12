//GScript Test Code, pushing data every 1 minute into Google Sheets
//Uses only NodeMCU
//12 June 2018

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
String readString;
const char* ssid = "ReapBenefit";
const char* password = "solvesmalldentbig";

//const int buttonPin = 2;    			// the number of the pushbutton pin
//int buttonState;             			// the current reading from the input pin
//int lastButtonState = LOW;   			// the previous reading from the input pin
//unsigned long lastDebounceTime = 0;  	// the last time the output pin was toggled
//unsigned long debounceDelay = 50;    	// the debounce time; increase if the output flickers

const char* host = "script.google.com";
const int httpsPort = 443;
// Use WiFiClientSecure class to create TLS connection
WiFiClientSecure client;
// SHA1 fingerprint of the certificate, don't care with your GAS service
const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";
String GAS_ID = "AKfycbwxosqLamJ50n63AK43D4Lm57PyeuZxmf4HDvCinIbPaQkGFIZi"; 	// Replace by your GAS service id
void setup()
{
  //pinMode(buttonPin, INPUT);
  Serial.begin(115200);
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

void loop()
{
  sendData(113, "Amarnath");
  delay(60000);

}



// Function for Send data into Google Spreadsheet
void sendData(int val, String user)
{
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

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