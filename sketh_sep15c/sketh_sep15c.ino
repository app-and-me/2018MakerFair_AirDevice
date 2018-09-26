#include <OLEDDisplay.h>
#include <OLEDDisplayFonts.h>
#include <OLEDDisplayUi.h>
#include <SH1106.h>
#include <SH1106Brzo.h>
#include <SH1106Spi.h>
#include <SH1106Wire.h>
#include <SSD1306.h>
#include <SSD1306Brzo.h>
#include <SSD1306Spi.h>
#include <SSD1306Wire.h>

#include <OLEDDisplay.h>
#include <OLEDDisplayFonts.h>
#include <OLEDDisplayUi.h>
#include <SH1106.h>
#include <SH1106Brzo.h>
#include <SH1106Spi.h>
#include <SH1106Wire.h>
#include <SSD1306.h>
#include <SSD1306Brzo.h>
#include <SSD1306Spi.h>
#include <SSD1306Wire.h>

#include "DHT.h"
#define DHTPIN  16       // Data 핀 설정
#define DHTTYPE DHT22   // 센서종류 설정
 
DHT dht(DHTPIN, DHTTYPE, 15);  // DHT 센서 설정
 
void setup() {
Serial.begin(9600); 
Serial.println("DHT22 TEST !!!");
 
  dht.begin();
}
 
void loop() {
delay(3000);
  float t = dht.readTemperature();
  float h = dht.readHumidity();
       
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.print(" ^C,\t");
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.println(" %");
  }
}
