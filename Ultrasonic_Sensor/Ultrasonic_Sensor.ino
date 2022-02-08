///////////////////////////////////////////////////////////
//LCD GND = Power GND
//LCD VCC = Power 5V
//LCD SDA = Analog In A4
//LCD SCL = Analog In A5
//HC-SRO4 Ultrasonic Sensor GND = Power GND
//HC-SRO4 Ultrasonic Sensor VCC = Power 5V
//HC-SRO4 Ultrasonic Sensor Trig = Digital Pin 10 
//HC-SRO4 Ultrasonic Sensor Echo = Digital Pin 13
//Temp and Humidity Sensor GND = Power GND
//Temp and Humidity Sensor VCC = Power 5V
//Temp and Humidity Sensor Out = Digital Pin 7
///////////////////////////////////////////////////////////  
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <dht.h>

#define trigPin 10
#define echoPin 13
#define DHT11_PIN 7

LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
dht DHT;

void setup(){
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
}

void loop(){
  float duration, distance;
  float speed;
    int chk = DHT.read11(DHT11_PIN);
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  speed = 331.4 + (0.606 * DHT.temperature) + (0.0124 * DHT.humidity);
  distance = (duration / 2) * (speed / 10000);
  
  if (distance >= 400 || distance <= 2){
    Serial.print("Distance = ");
    Serial.println("Out of range");
  }
  else {
    Serial.print("Distance = ");
    Serial.print(distance);
    Serial.println(" cm");
  }
  lcd.backlight();
  lcd.setCursor(0,0); 
  lcd.print(DHT.temperature);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(8,0);
  lcd.print(DHT.humidity);
  lcd.print("%");
  
  lcd.setCursor(0,1);
  lcd.print("Distance ");
  lcd.setCursor(10,1);
  lcd.print(distance);
  delay(300);
}
