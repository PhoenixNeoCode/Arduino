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
//IR Receiver GND = Power GND
//IR Receiver +5V = Power 5V
//IR Receiver Signal = Digital Pin 2
//GPS NEO-6M-0-001 GND = Power GND
//GPS NEO-6M-0-001 +3.3V = Power 3.3V
//GPS NEO-6M-0-001 TX = Digital Pin 4
//GPS NEO-6M-0-001 RX = Digital Pin 3 (!Reststor 10K GND to PIN3 AND 4.6k from RX to PIN3)
///////////////////////////////////////////////////////////  
#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <dht.h>
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
#define DHT11_PIN 7

uint8_t IRProtocol = 0;
uint16_t IRAddress = 0;
uint32_t IRCommand = 0;
int ActiveIRKey=1;//IR DEFAULT KEY

//GPS CALLS
static void smartdelay(unsigned long ms);
static String print_float(float val, float invalid, int len, int prec);
static String print_int(unsigned long val, unsigned long invalid, int len);
static String print_date(TinyGPS &gps);
static String print_str(const char *str, int len);

LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
TinyGPS gps;
SoftwareSerial ss(4, 3);
dht DHT;

void setup(){
  ss.begin(9600);
  pinMode(trigPin_1, OUTPUT);
  pinMode(echoPin_1, INPUT);
  pinMode(trigPin_2, OUTPUT);
  pinMode(echoPin_2, INPUT);
  pinMode(trigPin_3, OUTPUT);
  pinMode(echoPin_3, INPUT);
  attachInterrupt(digitalPinToInterrupt(pinIR), IRLinterrupt<IR_USER>, CHANGE);
  lcd.init();
  lcd.backlight();
  IRCommand=62220;
}

void loop(){
  //DEFINE VARS
  float speed;
  float duration_1, distance_1;
  float duration_2, distance_2;
  float duration_3, distance_3;
  int chk = DHT.read11(DHT11_PIN);
  speed = 331.4 + (0.606 * DHT.temperature) + (0.0124 * DHT.humidity);
  //GPS VARS
  float flat_GPS, flon_GPS;
  unsigned long age_GPS, date_GPS, time_GPS, chars_GPS = 0;
  unsigned short sentences_GPS = 0, failed_GPS = 0;
  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
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
  lcd.backlight();
  Serial.println(IRCommand);
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
if (IRProtocol) {
    if(IRCommand==62220){ //Num 1       
      ActiveIRKey=1;
    }
    if(IRCommand==59160){ //Num 2
      ActiveIRKey=2;
    }
    if(IRCommand==41310){ //Num 3
      ActiveIRKey=3;
    }
    if(IRCommand==63240){ //Num 4
      ActiveIRKey=4;
    }
    IRProtocol = 0;
}
/////////////////////////////////////////////////
//Reprint LCD Values
    if(ActiveIRKey==1){ //Num 1    
      lcd.setCursor(0,0); 
      lcd.print(DHT.temperature);
      lcd.print((char)223);
      lcd.print("C");
      lcd.setCursor(8,0);
      lcd.print(DHT.humidity);
      lcd.print("%");
     
      lcd.setCursor(0,1);
      lcd.print("Dist1 ");
      lcd.setCursor(6,1);
      lcd.print(distance_1);
    }
    if(ActiveIRKey==2){ //Num 2
      lcd.setCursor(0,0); 
      lcd.print(DHT.temperature);
      lcd.print((char)223);
      lcd.print("C");
      lcd.setCursor(8,0);
      lcd.print(DHT.humidity);
      lcd.print("%");
      
      lcd.setCursor(0,1);
      lcd.print("Dist2 ");
      lcd.setCursor(6,1);
      lcd.print(distance_2);
    }
    if(ActiveIRKey==3){ //Num 3
      lcd.setCursor(0,0); 
      lcd.print(DHT.temperature);
      lcd.print((char)223);
      lcd.print("C");
      lcd.setCursor(8,0);
      lcd.print(DHT.humidity);
      lcd.print("%");
      
      lcd.setCursor(0,1);
      lcd.print("Dist3 ");
      lcd.setCursor(6,1);
      lcd.print(distance_3);
    }
    if(ActiveIRKey==4){ //Num 4
      lcd.print(" N ");
      lcd.print(flat_GPS, 6);
      lcd.setCursor(0, 1);
      lcd.print(" E ");
      lcd.print(flon_GPS, 6);
    }
  smartdelay(500);
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
void decodeIR(const uint16_t duration) {
  decodeNec<IR_EXTRA_ACCURACY>(duration);
  decodePanasonic<IR_EXTRA_ACCURACY>(duration);
}
//GPS METHODS====================
static void smartdelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
static String print_float(float val, float invalid, int len, int prec) {
  String retVal="";
  if (val == invalid)  {
    while (len-- > 1) {
      retVal+='*';
    }
    retVal+=' ';
  }
  else  {
    retVal=String(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i = flen; i < len; ++i)
      retVal+=' ';
  }
  return retVal; 
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
  return sz;
    smartdelay(0);
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
  return print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
    smartdelay(0);
}

static String print_str(const char *str, int len) {
  String retValue="";
  int slen = strlen(str);
  for (int i = 0; i < len; ++i) {
    if(i < slen){
      retValue+=str[i];
    }
    else{
      retValue+=' ';
    }
  }
  return retValue;
    smartdelay(0);
}
