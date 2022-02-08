///////////////////////////////////////////////////////////
//LCD GND = Power GND
//LCD VCC = Power 5V
//LCD SDA = Analog In A4
//LCD SCL = Analog In A5
///////////////////////////////////////////////////////////  
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
int timeSecond=600;
LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup(){
  lcd.init();
  lcd.backlight();
}

void loop(){
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print(timeSecond);
  lcd.setCursor(1,1);
  lcd.print(formatSecondToTime(timeSecond));
  timeSecond=timeSecond-1;
  
  delay(1000);
}

String formatSecondToTime(int seconds){
  int runHours= seconds/3600;
  int secsRemaining=seconds%3600;
  int runMinutes=secsRemaining/60;
  int runSeconds=secsRemaining%60;

  char buf[21];
  sprintf(buf,"%02d:%02d:%02d",runHours,runMinutes,runSeconds);
  return buf;
}

