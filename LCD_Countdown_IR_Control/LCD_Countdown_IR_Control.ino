///////////////////////////////////////////////////////////
//LCD GND = Power GND
//LCD VCC = Power 5V
//LCD SDA = Analog In A4
//LCD SCL = Analog In A5
//IRRecv- = Power GND
//IRRecv+ = Power 5V
//IRRecvS = Digital 7
//Button Signal = Digital 2
//Active Buzzer+ = Digital 3
///////////////////////////////////////////////////////////  
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include <IRremote.h>
const int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;

const int  buttonPin = 2; //SNOOZE BUTTON
const int  buzzerPin = 3; //BUZZER SIGNAL

const int COUNTDOWN_INTERVAL = 1000;
const int MENU_INTERVAL = 200;

long seconds=0;
boolean COUNTDOWN_MODE=false;
String enteredSeconds="";
LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  lcd.init();
  lcd.backlight();
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop(){
  if(digitalRead(buttonPin)==HIGH){
    resetAlarm();
  }
  
  if (irrecv.decode(&results)){
    Serial.println(results.value);//read IR value
    if(results.value == 2498985700){//1
      enteredSeconds=appendNumber(1);
    }else if(results.value == 4129126878){//2
      enteredSeconds=appendNumber(2);
    }else if(results.value == 2172071812){//3
      enteredSeconds=appendNumber(3);
    }else if(results.value == 1301396414){//4
      enteredSeconds=appendNumber(4);
    }else if(results.value == 3345571616){//5
      enteredSeconds=appendNumber(5);
    }else if(results.value == 145526690){//6
      enteredSeconds=appendNumber(6);
    }else if(results.value == 2513627108){//7
      enteredSeconds=appendNumber(7);
    }else if(results.value == 324244318){//8
      enteredSeconds=appendNumber(8);
    }else if(results.value == 3430842824){//9
      enteredSeconds=appendNumber(9);
    }else if(results.value == 602597570){//0
      enteredSeconds=appendNumber(0);
    }else if(results.value == 299586532){//HOME
      resetAlarm();
    }else if(results.value == 2637951582){//RETURN (remove last sign)
      enteredSeconds=removeLastNumber();
    }else if(results.value == 3421054024){//PLAY
      startCountDown();
    }else if(results.value == 1791365666){//STOP
      resetAlarm();
    }else if(results.value == 3269679876){//PAUSE
      COUNTDOWN_MODE=false;
      enteredSeconds=seconds;
      lcd.clear();
      lcd.setCursor(1,1);
      lcd.print(formatSecondToTime(seconds));
    }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(enteredSeconds);
  delay(MENU_INTERVAL);
  irrecv.resume();
  }
  if(COUNTDOWN_MODE){
    if(seconds-1<0){
      endCountDownAndTriggerAlarm();
    } else {
      lcd.clear();
      lcd.setCursor(1,1);
      lcd.print(formatSecondToTime(seconds));
      seconds=seconds-1;
      delay(COUNTDOWN_INTERVAL);
    }
  }
  
}

String appendNumber(int number){
  return enteredSeconds+String(number);
}

String removeLastNumber(){
  return enteredSeconds.substring(0,enteredSeconds.length()-1);
}

void startCountDown(){
  COUNTDOWN_MODE=true;
  seconds=enteredSeconds.toInt();
}

void endCountDownAndTriggerAlarm(){
  lcd.setCursor(1,1);
  lcd.print("Countdown ended");
  digitalWrite(buzzerPin, HIGH);
  delay(buzzerPin);
}

void resetAlarm(){
  COUNTDOWN_MODE=false;
  enteredSeconds="";
  digitalWrite(buzzerPin, LOW);
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
