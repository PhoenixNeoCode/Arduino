///////////////////////////////////////////////////////////
//LCD GND = Power GND
//LCD VCC = Power 5V
//LCD SDA = Analog In A4
//LCD SCL = Analog In A5
//Hum&Temp Sensor GND = Power GND
//Hum&Temp Sensor + = Power 5V
//Hum&Temp Sensor Out = Digital Pin 7
/////////////////////////////////////////////////////////// 
#include <dht.h>
#include <LiquidCrystal_I2C.h>
#define DHT11_PIN 7
dht DHT;
LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  lcd.init();
  lcd.backlight();
}

void loop() {
  int chk = DHT.read11(DHT11_PIN);
  lcd.setCursor(0,0); 
  lcd.print("Temp: ");
  lcd.print(DHT.temperature);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(DHT.humidity);
  lcd.print("%");
  delay(1000);

}
