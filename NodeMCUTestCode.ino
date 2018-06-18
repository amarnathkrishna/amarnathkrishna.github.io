//NodeMCU Test Code
//4 Step testing method:
//1)LED Blink 2)Wifi Scan 3)IP Address print 4)Push Dummy Data

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
//Wifi Credentials
const char* MY_SSID = "ReapBenefit";                //Enter your SSID
const char* MY_PWD =  "solvesmalldentbig";          //Enter your password

int value;
String user1;

//For pushing data into google sheet
const char* host = "script.google.com";                 //Do Not Change
const int httpsPort = 443;                //Do Not Change

WiFiClientSecure client;

// SHA1 fingerprint of the certificate, do not change
const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";

String GAS_ID = "AKfycbzYAanq99TNhKtw91ZmW2xO1VGF0w4dI5PA9z5v-36dNrnrt5i3"; //GAS ID is obtained after deploying your script as web app

int show;

void setup() {
  Serial.begin(115200);
  show = 0;
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
}

void wificonnect()
{
  Serial.print("Connecting to " + *MY_SSID);
  WiFi.begin(MY_SSID, MY_PWD);
  while (WiFi.status() != WL_CONNECTED) //not connected,..waiting to connect
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Credentials accepted! Connected to wifi\n ");
}

void sendData(int val, String user)
{
  Serial.print("connecting to ");
  Serial.println(host);
  if (client.connect(host, httpsPort))
  {
    if (client.verify(fingerprint, host))
    {
      Serial.println("certificate matches");
    }
    else
    {
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
    return;
  }
}


void loop() {
  if (show == 0)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("LED Off");
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("LED On");
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("LED Off");
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("LED On");
    delay(500);
  }

  else if (show == 1)
  {
    wificonnect();

  }

  else if (show == 2)
  {
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

  else if (show == 3)
  {
    value = 100;
    user1 = "Test";
    sendData(value, user1);
    delay(2000);
    WiFi.disconnect();
    Serial.println("Wifi disconnected");
  }

  delay(2000);
  show = (show + 1) ;
}
