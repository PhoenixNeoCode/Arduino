///////////////////////////////////////////////////////////
//Joystick GND = Power GND
//Joystick +5V = Power 5V
//Joystick Switch = Digital Pin 2
//Joystick Y-Axis = Analog Pin A1
//Joystick X-Axis = Analog Pin A0
/////////////////////////////////////////////////////////// 
const int SW_pin =2;
const int X_pin =0;
const int Y_pin =1;

void setup() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(115200);

}

void loop() {
  Serial.print("Switch:  ");
  Serial.print(digitalRead(SW_pin));
  Serial.print("\n");
  Serial.print("X-axis: ");
  Serial.print(analogRead(X_pin));
  Serial.print("\n");
  Serial.print("Y-axis: ");
  Serial.print(analogRead(Y_pin));
  Serial.print("\n\n");
  delay(500);
}
