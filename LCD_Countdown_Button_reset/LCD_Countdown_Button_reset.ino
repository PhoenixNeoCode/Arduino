///////////////////////////////////////////////////////////
//LCD GND = Power GND
//LCD VCC = Power 5V
//LCD SDA = Analog In A4
//LCD SCL = Analog In A5
///////////////////////////////////////////////////////////  
///////////////////////////////////////////////////////////  
// vereinfachte version
// https://www.arduino.cc/en/Tutorial/StateChangeDetection
///////////////////////////////////////////////////////////  

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
int timeSecondOriginal=1800;
int timeSecond=timeSecondOriginal;
LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
// this constant won't change:
const int  buttonPin = 2;    // the pin that the pushbutton is attached to

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
int timeCounter=1;
int timeCounterInterval=20;

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}


void loop() {
  if(timeSecond<0){
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("GAME OVER");
    delay(1000000);
  }else{
    ////////////////////////////////////
    // read the pushbutton input pin:
    buttonState = digitalRead(buttonPin);
    // compare the buttonState to its previous state
    if (buttonState != lastButtonState) {
      // if the state has changed, increment the counter
      if (buttonState == HIGH) {
        // if the current state is HIGH then the button went from off to on:
        buttonPushCounter++;
        Serial.println("on - button pressed");
        Serial.print("number of button pushes: ");
        Serial.println(buttonPushCounter);
      } else {
        // if the current state is LOW then the button went from on to off:
        Serial.println("off - button released");
        //timeSecond=timeSecond+30;
        timeCounterInterval=timeCounterInterval-5;
      }
    }
    ////////////////////////////////////
    if(timeCounter<timeCounterInterval){
      timeCounter=timeCounter+1;
    }
    else{
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print(timeSecond);
      lcd.setCursor(1,1);
      lcd.print(formatSecondToTime(timeSecond));
      timeSecond=timeSecond-1;
      timeCounter=1;
    }
  }
  delay(50);
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;
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

