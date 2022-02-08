///////////////////////////////////////////////////////////
//LCD GND = Power GND
//LCD VCC = Power 5V
//LCD SDA = Analog In A4
//LCD SCL = Analog In A5
//HC-SRO4 Ultrasonic Sensor1 GND = Power GND
//HC-SRO4 Ultrasonic Sensor1 VCC = Power 5V
//HC-SRO4 Ultrasonic Sensor1 Trig = Digital Pin 12 
//HC-SRO4 Ultrasonic Sensor1 Echo = Digital Pin 13
//HC-SRO4 Ultrasonic Sensor2 GND = Power GND
//HC-SRO4 Ultrasonic Sensor2 VCC = Power 5V
//HC-SRO4 Ultrasonic Sensor2 Trig = Digital Pin 10 
//HC-SRO4 Ultrasonic Sensor2 Echo = Digital Pin 11
//HC-SRO4 Ultrasonic Sensor3 GND = Power GND
//HC-SRO4 Ultrasonic Sensor3 VCC = Power 5V
//HC-SRO4 Ultrasonic Sensor3 Trig = Digital Pin 8 
//HC-SRO4 Ultrasonic Sensor3 Echo = Digital Pin 9
//Temp and Humidity Sensor GND = Power GND
//Temp and Humidity Sensor VCC = Power 5V
//Temp and Humidity Sensor Out = Digital Pin 7
///////////////////////////////////////////////////////////  
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <dht.h>

#define trigPin_1 12
#define echoPin_1 13
#define trigPin_2 11
#define echoPin_2 10
#define trigPin_3 9
#define echoPin_3 8
#define DHT11_PIN 7

LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
dht DHT;

void setup(){
  Serial.begin (9600);
  pinMode(trigPin_1, OUTPUT);
  pinMode(echoPin_1, INPUT);
  pinMode(trigPin_2, OUTPUT);
  pinMode(echoPin_2, INPUT);
  pinMode(trigPin_3, OUTPUT);
  pinMode(echoPin_3, INPUT);
  lcd.init();
  lcd.backlight();
}

void loop(){
  //DEFINE VARS
  float speed;
  float duration_1, distance_1;
  float duration_2, distance_2;
  float duration_3, distance_3;
  int chk = DHT.read11(DHT11_PIN);
  speed = 331.4 + (0.606 * DHT.temperature) + (0.0124 * DHT.humidity);
////////////////////////////////////////////////////
//ACTIVATE ULTRASONIC TRIGGERS AND GET VALUES
  //SENSOR1
  digitalWrite(trigPin_1, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_1, LOW);
  duration_1 = pulseIn(echoPin_1, HIGH);
  distance_1 = (duration_1 / 2) * (speed / 10000);//RECALCULATE SPEED WITH TEMP AND HUMIDITY
  //SENSOR2
  digitalWrite(trigPin_2, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_2, LOW);
  duration_2 = pulseIn(echoPin_2, HIGH);
  distance_2 = (duration_2 / 2) * (speed / 10000);//RECALCULATE SPEED WITH TEMP AND HUMIDITY
  //SENSOR3
  digitalWrite(trigPin_3, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin_3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_3, LOW);
  duration_3 = pulseIn(echoPin_3, HIGH);
  distance_3 = (duration_3 / 2) * (speed / 10000);//RECALCULATE SPEED WITH TEMP AND HUMIDITY
////////////////////////////////////////////////////
//WRITE DISTANCE  
  //SENSOR1
  if (distance_1 >= 400 || distance_1 <= 2){
    Serial.print("Distance1: ");
    Serial.println("Out of range");
  }
  else {
    Serial.print("Distance1: ");
    Serial.print(distance_1);
    Serial.println(" cm");
  }
  //SENSOR2
  if (distance_2 >= 400 || distance_2 <= 2){
    Serial.print("Distance2: ");
    Serial.println("Out of range");
  }
  else {
    Serial.print("Distance2: ");
    Serial.print(distance_2);
    Serial.println(" cm");
  }
  //SENSOR3
  if (distance_3 >= 400 || distance_3 <= 2){
    Serial.print("Distance3: ");
    Serial.println("Out of range");
  }
  else {
    Serial.print("Distance3: ");
    Serial.print(distance_3);
    Serial.println(" cm");
  }
////////////////////////////////////////////////////
//WRITE TO LCD
  lcd.backlight();
  lcd.setCursor(0,0); 
  lcd.print(DHT.temperature);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(8,0);
  lcd.print(DHT.humidity);
  lcd.print("%");
  
  lcd.setCursor(0,1);
  lcd.print("Distance1 ");
  lcd.setCursor(10,1);
  lcd.print(distance_1);
  
  delay(300);
}
