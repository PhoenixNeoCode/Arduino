///////////////////////////////////////////////////////////
//LDR GND = Power GND
//LDR +5V (after Resistor 220 Ohm) = Power 5V
//LDR data (before Resistor 220 Ohm) = Analog A0
//LED GND = Power GND
//LED + = Digital Pin 13
/////////////////////////////////////////////////////////// 
int LDR=0;
int LDRValue=0;
int Light_sensitivity=50;
void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);

}

void loop() {
  LDRValue=analogRead(LDR);
  Serial.println(LDRValue);
  delay(300);
  if(LDRValue<Light_sensitivity){
    digitalWrite(13, HIGH);
  }
  else{
    digitalWrite(13, LOW);
  }

}
