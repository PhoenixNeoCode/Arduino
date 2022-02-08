///////////////////////////////////////////////////////////
//LCD GND = Power GND
//LCD VCC = Power 5V
//LCD SDA = Analog In A4
//LCD SCL = Analog In A5
//IR Avoid Sensor GND = Power GND
//IR Avoid Sensor + = Power 5V
//IR Avoid Sensor out = Digital Pin 2
//LED + = Digital Pin 13
//LED - = GND
///////////////////////////////////////////////////////////

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
const int LED =13;
const int SENSOR=2;
int value;
LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(SENSOR, INPUT);
  
  lcd.init(); 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("HELLO");
  delay(1000);
   lcd.setCursor(3,1);
  lcd.print("MARC STEINER");
  delay(1000);
}


void loop()
{
  value=digitalRead(SENSOR);
  digitalWrite(LED, !value);
  lcd.clear();
  if(value==0){
    lcd.setCursor(5,1);
    lcd.print("!STOP!");
  }
  delay(30);
}
