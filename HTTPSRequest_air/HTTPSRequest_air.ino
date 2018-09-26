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

const char* ssid = "hyohyo";
const char* password = "qogyrud1";

const char* host = "iot.emirim.kr";
const int httpsPort = 443;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char* fingerprint = "35 85 74 EF 67 35 A7 CE 40 69 50 F3 C0 F6 80 CF 80 3B 2E 19";

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;

float MeasuredDust=0;
float DustDensity = 0;

int LedPower=16;   // GPIO16 --> Sensor's LED pin (3rd Pin)
int DelayTime=280;
int DelayTime2=40;
int statusCode=0;
float OffTime=9680;
  
void setup() {
  pinMode(4, OUTPUT);
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
  
}

void loop() {
  if(statusCode == '1'){
      digitalWrite(4,HIGH);
      Serial.print("\nturnon++");
  }
  else if(statusCode == '0'){
      digitalWrite(4,LOW);
      Serial.print("\nturnoff--");
  }
  float FineDust = 0.0;
  delayMicroseconds(DelayTime);
  
  //먼지 센서를 값을 0.0V~3.3V 을 0~1024 값으로 변경해 줌.
  MeasuredDust=analogRead(A0); // read the dust value 
  delayMicroseconds(DelayTime2);
  delayMicroseconds(OffTime);

  // 볼트를 기준으로 미세먼지 값으로 변환해 줌, 데이터 시트을 참고 오차가 존재합니다.

  FineDust = 0.17*MeasuredDust*(3.3/1024)*1000; 
    

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

  String urlControl = "/api/aircontrol/";
  urlControl += "1";
  String urlDust = "/api/air/";
  urlDust += "1/"+(String)MeasuredDust+"/"+(String)FineDust;
  
  if (isnan(MeasuredDust) || isnan(FineDust)) {
    Serial.println("Fail to read!!"); 
  } else {
    if(FineDust>=20.0){
    }
    else {
    }
    Serial.print("\nMeasuredDust: ");
    Serial.print(MeasuredDust);
    Serial.print("FineDust: ");
    Serial.print(FineDust);   
 
  }
  //미세먼지 값 보내기
  Serial.print("requesting URL: ");
  Serial.println(urlDust);

  client.print(String("GET ") + urlDust + " HTTP/1.1\r\n" +
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

  
  //디바이스 작동코드
  Serial.print("requesting URL: ");
  Serial.println(urlControl);

  client.print(String("GET ") + urlControl + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line2 = client.readStringUntil('\n');
    if (line2 == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line2 = client.readStringUntil('\n');
  statusCode = line2.charAt(1);
  Serial.println((String)statusCode);
  Serial.println("closing connection");
}
