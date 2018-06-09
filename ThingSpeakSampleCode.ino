//Sample code to upload data from NodeMCU into Thingspeak

#include <ESP8266WiFi.h> // ESP8266WiFi.h library

const char* ssid     = "Reap_Benefit_2.4";
const char* password = "solvesmalldentbig";
const char* host = "api.thingspeak.com";
const char* writeAPIKey = "2I59914QLVTMWFDR";


void setup() {
  Serial.begin(9600);

//  Connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
 float humidity = 1100;
 float temperature = 35;

  if (isnan(humidity) || isnan(temperature)) {
    return;
  }

// make TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    return;
  }

  String url = "/update?key=";
  url+=writeAPIKey;
  url+="&field1=";
  url+=String(temperature);
  url+="&field2=";
  url+=String(humidity);
  url+="\r\n";
  
  // Request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
    delay(1000);
}