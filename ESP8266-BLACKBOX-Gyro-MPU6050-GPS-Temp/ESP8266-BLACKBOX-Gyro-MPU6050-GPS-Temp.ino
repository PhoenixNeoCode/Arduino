///////////////////////////////////////////////////////////
//Tools-->Boards-->Node MCU 1.0 (ESP-12E) Module
//MPU6050 VCC = Power 3V
//MPU6050 GND = Power G
//MPU6050 SCL = D1
//MPU6050 SDA = D2
//GPS NEO-6M-0-001 GND = Power GND
//GPS NEO-6M-0-001 +3.3V = Power 3.3V
//GPS NEO-6M-0-001 TX = Digital Pin 5
//GPS NEO-6M-0-001 RX = Digital Pin 6 (!Reststor 10K GND to PIN3 AND 4.6k from RX to PIN3)
///////////////////////////////////////////////////////////
#include <DHT.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
TinyGPS gps;
SoftwareSerial ss(10, 9);

Adafruit_MPU6050 mpu;

double arx, ary, arz;

/*
static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);
*/
#define DHTPIN 7     
#define DHTTYPE DHT22  
DHT dht(DHTPIN, DHTTYPE);

void setup(void) {
  Serial.begin(115200);
  //ss.begin(115200);
  
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

//  dht.begin();

  Serial.println("");
  delay(100);
}

void loop() {
  float flat_GPS, flon_GPS;
  unsigned long age_GPS, date_GPS, time_GPS, chars_GPS = 0;
  unsigned short sentences_GPS = 0, failed_GPS = 0;
  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;

  Serial.print(print_int(gps.satellites(), TinyGPS::GPS_INVALID_SATELLITES, 5));
  gps.f_get_position(&flat_GPS, &flon_GPS, &age_GPS);
  Serial.print(print_float(flat_GPS, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6));
  Serial.print(print_float(flon_GPS, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6));
  Serial.print(print_date(gps));
  Serial.print(print_float(gps.f_altitude(), TinyGPS::GPS_INVALID_F_ALTITUDE, 7, 2));
  Serial.print(print_float(gps.f_course(), TinyGPS::GPS_INVALID_F_ANGLE, 7, 2));
  Serial.print(print_float(gps.f_speed_kmph(), TinyGPS::GPS_INVALID_F_SPEED, 6, 2));
  Serial.print(print_str(gps.f_course() == TinyGPS::GPS_INVALID_F_ANGLE ? "*** " : TinyGPS::cardinal(gps.f_course()), 6));
  Serial.println();


  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

//////////
arx = (180/3.141592) * atan(a.acceleration.x / sqrt(sq(a.acceleration.y) + sq(a.acceleration.z))); 
ary = (180/3.141592) * atan(a.acceleration.y / sqrt(sq(a.acceleration.x) + sq(a.acceleration.z)));
arz = (180/3.141592) * atan(sqrt(sq(a.acceleration.y) + sq(a.acceleration.x)) / a.acceleration.z);
//////////

  /* Print out the values */
  Serial.print("Angle X: ");
  Serial.print(arx);
  Serial.print(", Y: ");
  Serial.print(ary);
  Serial.print(", Z: ");
  Serial.print(arz);
  Serial.println(" °");
  
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");


  Serial.print("Temperature Gyro: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");
/*
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
*/
  Serial.println("");
  smartdelay(500);
}

////GPS functions
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
}
