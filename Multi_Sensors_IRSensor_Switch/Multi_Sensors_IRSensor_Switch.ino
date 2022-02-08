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
//Temp and Humidity Sensor Out = Digital Pin 6
//IR Receiver GND = Power GND
//IR Receiver +5V = Power 5V
//IR Receiver Signal = Digital Pin 3
//GPS NEO-6M-0-001 GND = Power GND
//GPS NEO-6M-0-001 +3.3V = Power 3.3V
//GPS NEO-6M-0-001 TX = Digital Pin 4
//GPS NEO-6M-0-001 RX = Digital Pin 3 (!Reststor 10K GND to PIN3 AND 4.6k from RX to PIN3)
///////////////////////////////////////////////////////////
#include <dht.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TinyGPS.h>
#include "IRLremote.h"
#define IRL_BLOCKING true
#define pinIR 2 //3 or 2 can be used for Interrupt signal on UNO and 2,3,18,19,20,21 on MEGA
#define trigPin_1 12
#define echoPin_1 13
#define trigPin_2 11
#define echoPin_2 10
#define trigPin_3 9
#define echoPin_3 8
#define DHT11_PIN 6

uint8_t IRProtocol = 0;
uint16_t IRAddress = 0;
uint32_t IRCommand = 0;

TinyGPS gps;
SoftwareSerial ss(4, 3);
dht DHT;
LiquidCrystal_I2C lcd(0x3F, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

int chk;
float temperatureSensor, humiditySensor, speedUltrasonicSensor;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin_1, OUTPUT);
  pinMode(echoPin_1, INPUT);
  pinMode(trigPin_2, OUTPUT);
  pinMode(echoPin_2, INPUT);
  pinMode(trigPin_3, OUTPUT);
  pinMode(echoPin_3, INPUT);
  attachInterrupt(digitalPinToInterrupt(pinIR), IRLinterrupt<IR_USER>, CHANGE);
  ss.begin(9600);
  lcd.init();
  lcd.backlight();
  IRCommand = 62220;
  chk = DHT.read11(DHT11_PIN);
  temperatureSensor = DHT.temperature;
  humiditySensor = DHT.humidity;
  speedUltrasonicSensor = 331.4 + (0.606 * temperatureSensor) + (0.0124 * humiditySensor);
}

void loop() {
  //DEFINE VARS
  float speedUltrasonicSensor;
  float durationUsonic_1, distanceUsonic_1;
  float durationUsonic_2, distanceUsonic_2;
  float durationUsonic_3, distanceUsonic_3;

  //gps vars
  float flat, flon;
  unsigned long age, date, time, chars = 0;
  gps.f_get_position(&flat, &flon, &age);
  String satelliteGPS = print_int(gps.satellites(), TinyGPS::GPS_INVALID_SATELLITES, 5);
  String latitudeGPS = print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
  String longitudeGPS = print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
  String dateGPS = print_date(gps);
  String altitudeGPS = print_float(gps.f_altitude(), TinyGPS::GPS_INVALID_F_ALTITUDE, 7, 2);
  String courseGPS = print_float(gps.f_course(), TinyGPS::GPS_INVALID_F_ANGLE, 7, 2);
  String courseGPSDir = print_str(gps.f_course() == TinyGPS::GPS_INVALID_F_ANGLE ? "*** " : TinyGPS::cardinal(gps.f_course()), 6);
  String speedGPS = print_float(gps.f_speed_kmph(), TinyGPS::GPS_INVALID_F_SPEED, 6, 2);

  ////////////////////////////////////////////////////
  //ACTIVATE ULTRASONIC TRIGGERS AND GET VALUES
  //SENSOR1
  digitalWrite(trigPin_1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_1, LOW);
  durationUsonic_1 = pulseIn(echoPin_1, HIGH);
  distanceUsonic_1 = (durationUsonic_1 / 2) * (speedUltrasonicSensor / 10000);//RECALCULATE SPEED WITH TEMP AND HUMIDITY
  //SENSOR2
  digitalWrite(trigPin_2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_2, LOW);
  durationUsonic_2 = pulseIn(echoPin_2, HIGH);
  distanceUsonic_2 = (durationUsonic_2 / 2) * (speedUltrasonicSensor / 10000);//RECALCULATE SPEED WITH TEMP AND HUMIDITY
  //SENSOR3
  digitalWrite(trigPin_3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_3, LOW);
  durationUsonic_3 = pulseIn(echoPin_3, HIGH);
  distanceUsonic_3 = (durationUsonic_3 / 2) * (speedUltrasonicSensor / 10000);//RECALCULATE SPEED WITH TEMP AND HUMIDITY
  ////////////////////////////////////////////////////
  //WRITE DISTANCE
  //SENSOR1
  Serial.print(durationUsonic_1);
  if (distanceUsonic_1 >= 400 || distanceUsonic_1 <= 2) {
    Serial.println("Distance1: Out of range");
  }
  else {
    Serial.print("Distance1: ");
    Serial.print(distanceUsonic_1);
    Serial.println(" cm");
  }
  //SENSOR2
  Serial.print(durationUsonic_2);
  if (distanceUsonic_2 >= 400 || distanceUsonic_2 <= 2) {
    Serial.println("Distance2: Out of range");
  }
  else {
    Serial.print("Distance2: ");
    Serial.print(distanceUsonic_2);
    Serial.println(" cm");
  }
  //SENSOR3
  Serial.print(durationUsonic_3);
  if (distanceUsonic_3 >= 400 || distanceUsonic_3 <= 2) {
    Serial.println("Distance3: Out of range");
  }
  else {
    Serial.print("Distance3: ");
    Serial.print(distanceUsonic_3);
    Serial.println(" cm");
  }
  //GPS DATA SERIAL PRINT
  Serial.println("connected satellites: " + satelliteGPS);
  Serial.println("Latitude deg: " + latitudeGPS);
  Serial.println("Longitude deg: " + longitudeGPS);
  Serial.println("Timestamp: " + dateGPS);
  Serial.println("Altitude: (m ueber. M.): " + altitudeGPS);
  Serial.println("Course: " + (courseGPS) + " " + courseGPSDir);
  Serial.println("Speed: " + speedGPS + " km/h");
  Serial.print("Temp: ");
  Serial.print(temperatureSensor);
  Serial.println("C");
  Serial.print("Humidity: ");
  Serial.print(humiditySensor);
  Serial.println(" %");
  Serial.println("////////////////////////");
  ////////////////////////////////////////////////////
  //uint8_t oldSREG = SREG;
  lcd.backlight();
  lcd.clear();
  Serial.println("IRCommand:" + IRCommand);
  if (IRCommand == 62220) {//1
    lcd.setCursor(0, 1);
    lcd.print("Distance1 ");
    lcd.setCursor(10, 1);
    lcd.print(distanceUsonic_1);
  }
  if (IRCommand == 59160) {//2
    lcd.setCursor(0, 1);
    lcd.print("Distance2 ");
    lcd.setCursor(10, 1);
    lcd.print(distanceUsonic_2);
  }
  if (IRCommand == 41310) {//3
    lcd.setCursor(0, 1);
    lcd.print("Distance3 ");
    lcd.setCursor(10, 1);
    lcd.print(distanceUsonic_3);
  }
  if (IRCommand == 63240) {//4
    lcd.print(" N ");
    lcd.print(flat, 6);
    lcd.setCursor(0, 1);
    lcd.print(" E ");
    lcd.print(flon, 6);
  }
  if (IRCommand == 58140) {//5
    gps.f_get_position(&flat, &flon, &age);
    lcd.setCursor(0, 0);
    lcd.print("Satellites: " + satelliteGPS);
    lcd.setCursor(0, 1);
    lcd.print("Altitude: " + altitudeGPS + "m");
  }
  if (IRCommand == 46410) {//9
    lcd.setCursor(0, 0);
    lcd.print(DHT.temperature);
    lcd.print((char)223);
    lcd.print("C");
    lcd.setCursor(8, 0);
    lcd.print(DHT.humidity);
    lcd.print("%");
  }
  IRProtocol = 0;
  smartdelay(500);
  // delay(400);
}
void IREvent(uint8_t protocol, uint16_t address, uint32_t command) {
  // called when directly received a valid IR signal.
  // do not use Serial inside, it can crash your program!

  // dont update value if blocking is enabled
  if (IRL_BLOCKING && !IRProtocol) {
    // update the values to the newest valid input
    IRProtocol = protocol;
    IRAddress = address;
    IRCommand = command;
  }
}
//////////////////////////////
//GPS METHODS

static void smartdelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static String print_float(float val, float invalid, int len, int prec) {
  String returnVal = "";
  if (val == invalid)  {
    while (len-- > 1) {
      returnVal += '*';
    }
    returnVal += ' ';
    return returnVal;
  }
  else  {
    returnVal = val;
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i = flen; i < len; ++i)
      returnVal += ' ';
  }
}

static String print_int(unsigned long val, unsigned long invalid, int len) {
  char sz[32];
  if (val == invalid) {
    strcpy(sz, "*******");
  }
  else {
    sprintf(sz, "%ld", val);
  }
  sz[len] = 0;
  for (int i = strlen(sz); i < len; ++i)
    sz[i] = ' ';
  if (len > 0)
    sz[len - 1] = ' ';
  return (sz);
}

static String print_date(TinyGPS &gps) {
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE) {
    return "********** ******** ";
  }
  else  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d ",
            month, day, year, hour, minute, second);
    return sz;
  }
  print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
}

String print_str(const char *str, int len) {
  int slen = strlen(str);
  String returnVal = "";
  for (int i = 0; i < len; ++i) {
    if (i < slen) {
      returnVal += str[i];
    }
    else {
      returnVal += " ";
    }
  }
  return returnVal;
}

//ENDE GPS METHODS
/////////////////////////////////////
