///////////////////////////////////////////////////////////
//LCD GND = Power GND
//LCD VCC = Power 5V
//LCD SDA = Analog In A4
//LCD SCL = Analog In A5
//RFID Sensor SDA = Digital Pin 10
//RFID Sensor SCK = Digital Pin 13
//RFID Sensor MOST (neben SCK)= Digital Pin 11
//RFID Sensor MISO (neben MOST)= Digital Pin 12
//RFID Sensor IRQ= none
//RFID Sensor GND= Power GND
//RFID Sensor RST= Digital Pin 9
//RFID Sensor 3.3V= Power 3.3V
/////////////////////////////////////////////////////////// 
#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

#define RST_PIN 9
#define SS_PIN  10

#define STATE_STARTUP       0
#define STATE_STARTING      1
#define STATE_WAITING       2
#define STATE_SCAN_INVALID  3
#define STATE_SCAN_VALID    4
#define STATE_SCAN_MASTER   5
#define STATE_ADDED_CARD    6
#define STATE_REMOVED_CARD  7

const int cardArrSize = 10;
const int cardSize    = 4;
byte cardArr[cardArrSize][cardSize];
byte masterCard[cardSize] = {166,172,148,53};//########### ENTER YOUR MASTERCARD CODE HERE (BASED ON SERIAL OUTPUT)
byte readCard[cardSize];
byte cardsStored = 0;

// Create MFRC522 instance
MFRC522 mfrc522(SS_PIN, RST_PIN);


LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

byte currentState = STATE_STARTUP;
unsigned long LastStateChangeTime;
unsigned long StateWaitTime;

//------------------------------------------------------------------------------------
int readCardState()
{
  int index;

  Serial.print("Card Data - ");
  for(index = 0; index < 4; index++)  {
    readCard[index] = mfrc522.uid.uidByte[index];
    Serial.print(readCard[index]);
    if (index < 3){
      Serial.print(",");
    }
  }
  Serial.println(" ");
  //Check Master Card
  if ((memcmp(readCard, masterCard, 4)) == 0) {
    return STATE_SCAN_MASTER;
  }
  if (cardsStored == 0) {
    return STATE_SCAN_INVALID;
  }
  for(index = 0; index < cardsStored; index++)  {
    if ((memcmp(readCard, cardArr[index], 4)) == 0) {
      return STATE_SCAN_VALID;
    }
  }
 return STATE_SCAN_INVALID;
}
//------------------------------------------------------------------------------------
void addReadCard(){
  int cardIndex;
  int index;

  if (cardsStored <= 20)  {
    cardsStored++;
    cardIndex = cardsStored;
    cardIndex--;
  }

  for(index = 0; index < 4; index++)  {
    cardArr[cardIndex][index] = readCard[index];
  }
}
//------------------------------------------------------------------------------------
void removeReadCard() {
  int cardIndex;
  int index;
  boolean found = false;
  
  for(cardIndex = 0; cardIndex < cardsStored; cardIndex++)  {
    if (found == true)    {
      for(index = 0; index < 4; index++)  {
        cardArr[cardIndex-1][index] = cardArr[cardIndex][index];
        cardArr[cardIndex][index] = 0;
      }
    }   
    if ((memcmp(readCard, cardArr[cardIndex], 4)) == 0) {
      found = true;
    }
  }

  if (found == true)  {
    cardsStored--;
  }
}

//------------------------------------------------------------------------------------
void updateState(byte aState) {
  if (aState == currentState)  {
    return;
  }
  // do state change
  switch (aState)  {
    case STATE_STARTING:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("RFID Scanner");
      lcd.setCursor(0,1);
      lcd.print("Starting up");
      StateWaitTime = 1000;
      break;
    case STATE_WAITING:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Waiting for Card");
      lcd.setCursor(0,1);
      lcd.print("to be swiped");
      StateWaitTime = 0;
      break;
    case STATE_SCAN_INVALID:
      if (currentState == STATE_SCAN_MASTER) {
        addReadCard();
        aState = STATE_ADDED_CARD;
        
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Card Scanned");
        lcd.setCursor(0,1);
        lcd.print("Card Added");
        StateWaitTime = 2000;
      }
      else if (currentState == STATE_REMOVED_CARD) {
        return;
      }
      else {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Card Scanned");
        lcd.setCursor(0,1);
        lcd.print("Invalid Card");
        StateWaitTime = 2000;
      }
      break;
    case STATE_SCAN_VALID:
      if (currentState == STATE_SCAN_MASTER) {
        removeReadCard();
        aState = STATE_REMOVED_CARD;
        
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Card Scanned");
        lcd.setCursor(0,1);
        lcd.print("Card Removed");
        StateWaitTime = 2000;
      }
      else if (currentState == STATE_ADDED_CARD) {
        return;
      }
      else {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Card Scanned");
        lcd.setCursor(0,1);
        lcd.print("valid Card");
        StateWaitTime = 2000;
      }
      break;
    case STATE_SCAN_MASTER:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Master Card");
      lcd.setCursor(0,1);
      lcd.print("Cards = ");
      lcd.setCursor(8,1);
      lcd.print(cardsStored);
      StateWaitTime = 5000;
      break;
  }

  /*lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(aState);
  lcd.setCursor(0,1);
  lcd.print(currentState);*/

  currentState = aState;
  LastStateChangeTime = millis();
}

void setup() {
  SPI.begin();         // Init SPI Bus
  mfrc522.PCD_Init();  // Init MFRC522
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();

  LastStateChangeTime = millis();
  updateState(STATE_STARTING);
  Serial.begin(9600);
}

void loop() {
  byte cardState;
  if ((currentState != STATE_WAITING) && (StateWaitTime > 0) && (LastStateChangeTime + StateWaitTime < millis()))  {
    updateState(STATE_WAITING);
  }
  // Look for new cards 
  if ( ! mfrc522.PICC_IsNewCardPresent()) { 
    return; 
  } 
  // Select one of the cards 
  if ( ! mfrc522.PICC_ReadCardSerial()) { 
    return; 
  }
  cardState = readCardState();
  updateState(cardState);
}
