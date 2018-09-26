#include "SSD1306.h" 
#include <DHT.h>

#define DHTPIN  16   // D0
#define DHTTYPE DHT22

SSD1306  display(0x3c, D3, D5);
DHT dht(DHTPIN, DHTTYPE, 15); 
 
void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("DHT22/SSD1306");
  Serial.println("ThingSpeak");
  
  dht.begin();

  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);  
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 10, "DHT22/SSD1306");
  display.display();  
  delay(3000);
 
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
    Serial.print(" ^C, \t");
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %");
    
 
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
  delay(3000);
  
}
