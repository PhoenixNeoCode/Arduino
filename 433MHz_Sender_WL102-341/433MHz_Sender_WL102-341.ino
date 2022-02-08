///////////////////////////////////////////////////////////
//Library: https://github.com/sui77/rc-switch
//433 MHz Sender (WL102-341) GND = Power GND
//433 MHz Sender (WL102-341) VCC = Power 5V
//433 MHz Sender (WL102-341) Data = Digital Pin 10
/////////////////////////////////////////////////////////// 

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();


void setup() {
  mySwitch.enableTransmit(10);  // Der Sender wird an Pin 10 angeschlossen
}
void loop() {
  mySwitch.send(16076992, 24); // Der 433mhz Sender versendet die Dezimalzahl „16076992“
    delay(100);
  mySwitch.send(16076848, 24);
    delay(200);
  mySwitch.send(16076812, 24);
    delay(300);
  mySwitch.send(16076803, 24);
    delay(400);
  mySwitch.send(16077568, 24);
  
  delay(1000);  // Eine Sekunde Pause, danach startet der Sketch von vorne
}  
