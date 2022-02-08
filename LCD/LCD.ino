///////////////////////////////////////////////////////////
//LCD GND = Power GND
//LCD VCC = Power 5V
//LCD SDA = Analog In A4
//LCD SCL = Analog In A5
///////////////////////////////////////////////////////////  
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("HELLO");
  delay(1000);
   lcd.setCursor(3,1);
  lcd.print("MARC STEINER");
}


void loop()
{
}
