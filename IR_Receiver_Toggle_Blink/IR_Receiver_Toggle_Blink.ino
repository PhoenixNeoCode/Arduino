///////////////////////////////////////////////////////////
//IR Receiver GND = Power GND
//IR Receiver +5V = Power 5V
//IR Receiver Signal = Digital Pin 2
/////////////////////////////////////////////////////////// 
#include "IRLremote.h"
#define IRL_BLOCKING true
#define pinIR 2
#define LED_RED 3
#define LED_YELLOW 5
#define LED_GREEN 10

uint8_t IRProtocol = 0;
uint16_t IRAddress = 0;
uint32_t IRCommand = 0;

void setup() {
  // start serial debug output
  Serial.begin(115200);
  Serial.println("Startup");
  attachInterrupt(digitalPinToInterrupt(pinIR), IRLinterrupt<IR_USER>, CHANGE);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
      pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
}

void loop() {
  uint8_t oldSREG = SREG;
  cli();
  if (IRProtocol) {
    Serial.println();
    Serial.print("Protocol:");
    Serial.println(IRProtocol);
    Serial.print("Address:");
    Serial.println(IRAddress, HEX);
    Serial.print("Command:");
    Serial.println(IRCommand);
    if(IRCommand==62220){
      digitalWrite(LED_RED, HIGH);
    }
    if(IRCommand==59160){
      digitalWrite(LED_YELLOW, HIGH);
    }
    if(IRCommand==41310){
      digitalWrite(LED_GREEN, HIGH);
    }
    if(IRCommand==63240){
      digitalWrite(LED_RED, LOW);
    }
    if(IRCommand==58140){
      digitalWrite(LED_YELLOW, LOW);
    }
    if(IRCommand==42330){
      digitalWrite(LED_GREEN, LOW);
    }
    IRProtocol = 0;
  }
  SREG = oldSREG;
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    delay(100);
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(11, LOW);
    delay(100);
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(11, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(11, LOW);
    delay(100);
}

void IREvent(uint8_t protocol, uint16_t address, uint32_t command) {
  if (IRL_BLOCKING && !IRProtocol) {
    IRProtocol = protocol;
    IRAddress = address;
    IRCommand = command;
  }
}

void decodeIR(const uint16_t duration) {
  decodeNec<IR_EXTRA_ACCURACY>(duration);
  decodePanasonic<IR_EXTRA_ACCURACY>(duration);
}
