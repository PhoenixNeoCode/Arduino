///////////////////////////////////////////////////////////
//Driver 28BYJ-48 GND = Power GND
//Driver 28BYJ-48 +5V = Power 5V
//Driver 28BYJ-48 IN1 = Digital Pin 2
//Driver 28BYJ-48 IN2 = Digital Pin 3
//Driver 28BYJ-48 IN3 = Digital Pin 4
//Driver 28BYJ-48 IN4 = Digital Pin 5
/////////////////////////////////////////////////////////// 
int IN1=2;
int IN2=3;
int IN3=4;
int IN4=5;

int currentStep=0;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

}

void loop() {
  switch(currentStep){
    case 0:
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        break;
     case 1:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        break;
      case 2:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        break;
      case 3:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        break;
  }
  currentStep=(++currentStep<4)?currentStep:0;
  delay(50);

}
