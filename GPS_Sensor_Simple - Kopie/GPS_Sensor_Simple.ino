///////////////////////////////////////////////////////////
//GPS NEO-6M-0-001 GND = Power GND
//GPS NEO-6M-0-001 +3.3V = Power 3.3V
//GPS NEO-6M-0-001 TX = Digital Pin 4
//GPS NEO-6M-0-001 RX = Digital Pin 3 (!Reststor 10K GND to PIN3 AND 4.6k from RX to PIN3)
///////////////////////////////////////////////////////////
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TinyGPS.h>
TinyGPS gps;
SoftwareSerial ss(4, 3);
LiquidCrystal_I2C lcd(0x3F, 16, 2);
static void smartdelay(unsigned long ms);
static String print_float(float val, float invalid, int len, int prec);
static String print_int(unsigned long val, unsigned long invalid, int len);
static String print_date(TinyGPS &gps);
static String print_str(const char *str, int len);

void setup() {
  Serial.begin(9600);

  Serial.print("Testing TinyGPS library v. "); Serial.println(TinyGPS::library_version());
  Serial.println("Sats Latitude  Longitude  Date       Time     Date Alt    Course Speed Card");
  Serial.println("     (deg)     (deg)                          Age  (m)    --- from GPS ----");
  Serial.println("---------------------------------------------------------------------------");

  ss.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop() {
  float flat, flon;
  unsigned long age, date, time, chars = 0;
  unsigned short sentences = 0, failed = 0;
  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
  
  lcd.clear();
  lcd.print(" N ");
  lcd.print(flat, 6);
  lcd.setCursor(0, 1);
  lcd.print(" E ");
  lcd.print(flon, 6);
  
  Serial.print(print_int(gps.satellites(), TinyGPS::GPS_INVALID_SATELLITES, 5));
  gps.f_get_position(&flat, &flon, &age);
  Serial.print(print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6));
  Serial.print(print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6));
  Serial.print(print_date(gps));
  Serial.print(print_float(gps.f_altitude(), TinyGPS::GPS_INVALID_F_ALTITUDE, 7, 2));
  Serial.print(print_float(gps.f_course(), TinyGPS::GPS_INVALID_F_ANGLE, 7, 2));
  Serial.print(print_float(gps.f_speed_kmph(), TinyGPS::GPS_INVALID_F_SPEED, 6, 2));
  Serial.print(print_str(gps.f_course() == TinyGPS::GPS_INVALID_F_ANGLE ? "*** " : TinyGPS::cardinal(gps.f_course()), 6));
  Serial.println();

  smartdelay(500);
}
static void smartdelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
static String print_float(float val, float invalid, int len, int prec) {
  String retVal="";
  if (val == invalid)  {
    while (len-- > 1) {
      retVal+='*';
    }
    retVal+=' ';
  }
  else  {
    retVal=String(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i = flen; i < len; ++i)
      retVal+=' ';
  }
  return retVal; 
}

static String print_int(unsigned long val, unsigned long invalid, int len) {
  char sz[32];
  if (val == invalid) {
    strcpy(sz, "*******");
  }
  else {
    sprintf(sz, "%ld", val);
  }
  sz[len] = 0;
  for (int i = strlen(sz); i < len; ++i)
    sz[i] = ' ';
  if (len > 0)
    sz[len - 1] = ' ';
  return sz;
    smartdelay(0);
}

static String print_date(TinyGPS &gps) {
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE) {
    return "********** ******** ";
  }
  else  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d ",
            month, day, year, hour, minute, second);
    return sz;
  }
  return print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
    smartdelay(0);
}

static String print_str(const char *str, int len) {
  String retValue="";
  int slen = strlen(str);
  for (int i = 0; i < len; ++i) {
    if(i < slen){
      retValue+=str[i];
    }
    else{
      retValue+=' ';
    }
  }
  return retValue;
    smartdelay(0);
}

