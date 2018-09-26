
#include "SSD1306.h" 
#include <DHT.h>
#define DHTPIN  5   // D0
#define DHTTYPE DHT22

SSD1306  display(0x3c, D3, D5);
DHT dht(DHTPIN, DHTTYPE, 15); 

float MeasuredDust=0;
float DustDensity = 0;

int LedPower=16;   // GPIO16 --> Sensor's LED pin (3rd Pin)
int DelayTime=280;
int DelayTime2=40;
float OffTime=9680;

void setup(){
  Serial.begin(9600);
  pinMode(LedPower, OUTPUT);
  pinMode(4, OUTPUT);
  
  dht.begin();
  
  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);  
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 10, "앱앤미");
  display.display();  
  delay(1000);
} 

void loop(){
  float FineDust = 0.0;
    
  digitalWrite(LedPower, LOW); // power on the LED
  delayMicroseconds(DelayTime);

  //먼지 센서를 값을 0.0V~3.3V 을 0~1024 값으로 변경해 줌.
  MeasuredDust=analogRead(A0); // read the dust value 
  delayMicroseconds(DelayTime2);
  digitalWrite(LedPower, HIGH); // turn the LED off
  delayMicroseconds(OffTime);

  // 볼트를 기준으로 미세먼지 값으로 변환해 줌, 데이터 시트을 참고 오차가 존재합니다.
  FineDust = 0.17*MeasuredDust*(3.3/1024)*1000; 

  
  float t, h;
  t = dht.readTemperature();
  h = dht.readHumidity(); 

  //아날로그로 읽어 들인 0-1024 수치
  Serial.print("Measured Value = ");
  Serial.print(MeasuredDust);

  // 미세먼지 수치 출력
  Serial.print(",  Dust Density(ug/m3) = ");
  Serial.println(FineDust);

  if (isnan(MeasuredDust) || isnan(FineDust)) {
    Serial.println("Fail to read!!");
    display.clear();    
    display.drawString(0, 2, "Fail to read!!");
    display.display();        
  } else {
    if(FineDust==0.0){
      digitalWrite(4,LOW);
      Serial.print("\nturnoff--");
    }
    else {
      digitalWrite(4,HIGH);
      Serial.print("\nturnon++");
    }
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 34, "Dust :           ");
    display.drawString(56, 34, (String)FineDust);  
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 2, "Temp  :             C");
    display.drawString(56, 2, (String)t);  
    display.drawString(0, 18, "Humid :           %");
    display.drawString(56, 18, (String)h);  
    display.display();       
    Serial.print("\nMeasuredDust: ");
    Serial.print(MeasuredDust);
    Serial.print("FineDust: ");
    Serial.print(FineDust);
    Serial.print("Temperture: ");
    Serial.print(t);
    Serial.print(" ^C, \t");
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %");
    
 
  }

  delay(3000);
}
