/*
    HTTP over TLS (HTTPS) example sketch

    This example demonstrates how to use
    WiFiClientSecure class to access HTTPS API.
    We fetch and display the status of
    esp8266/Arduino project continuous integration
    build.

    Limitations:
      only RSA certificates
      no support of Perfect Forward Secrecy (PFS)
      TLSv1.2 is supported since version 2.4.0-rc1

    Created by Ivan Grokhotkov, 2015.
    This example is in public domain.
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include "SSD1306.h" 
#include <DHT.h>

#define DHTPIN  16   // D0
#define DHTTYPE DHT22

SSD1306  display(0x3c, D3, D5);
DHT dht(DHTPIN, DHTTYPE, 15); 

const char* ssid = "hyohyo";
const char* password = "qogyrud1";

const char* host = "iot.emirim.kr";
const int httpsPort = 443;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char* fingerprint = "35 85 74 EF 67 35 A7 CE 40 69 50 F3 C0 F6 80 CF 80 3B 2E 19";

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  
void setup() {
  Serial.begin(115200);
  
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("#");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("DHT22/SSD1306");
  Serial.println("ThingSpeak");
  
  dht.begin();

  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);  
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 10, "DHT22/SSD1306");
  display.display();  
  delay(2000);
  
}

void loop() {
  float t, h;
  String  temp, humi;
  t = dht.readTemperature();
  h = dht.readHumidity(); 
  temp = t;  
  humi = h;  
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Fail to read!!");
    display.clear();    
    display.drawString(0, 2, "Fail to read!!");
    display.display();        
  } else {
    Serial.print("Temperture: ");
    Serial.print(t);
    Serial.print(" ^C, \t\n");
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\n");

    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.drawString(100, 0, "o");
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 2, "Temp  :             C");
    display.drawString(56, 2, temp);  
    display.drawString(0, 18, "Humid :           %");
    display.drawString(56, 18, humi);  
    display.display();      
  }

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

  String url = "/api/sensor/";
  url += "0/"+(String)t+"/"+(String)h;
  
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
  Serial.println(line);
  Serial.println("closing connection");

  
  delay(5000);
}
