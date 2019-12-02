#include <SPI.h> //libraries needed for at bruge display
#include <Wire.h> //findes hvis man søger i Arduino IDE'en
#include <Adafruit_GFX.h> //til at tegne ting osv
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

SoftwareSerial gpsSerial(3, 4); // Setting up GPS-pins for RX and TX
TinyGPS gps; // Creating GPS object

unsigned long ms = 1000;

const int pos0_y = 0;   //y-position på info linje i top
const int posNowM_x = 15; //x-position for nuværende menu
const int posUr_x = 97; //x-position for uret
const int pos1_y = 12;  //y-position på første main element
const int pos2_y = 30;  //y-position på andet main element
const int pos3_y = 49;  //y-position på menuskift-element


// Menuer
const String velkomstMenu = "Welcome";      // idk hvorfor det er på engelsk
const String connectMenu = "Armb.";
const String gpsMenu = "GPS";
const String kursMenu = "Kurs";
const String gemtPosMenu = "G.Pos";
const String gemDistMenu = "G.Dist";
const String alarmMenu = "Alarm";

int forbindelse = 0;


String lat(unsigned long ms) {
  float lat = 0;
  float lon = 0;
  unsigned long fix_age;
  unsigned long start = millis();
  do {
    while (gpsSerial.available()) { // check for gps data
      if (gps.encode(gpsSerial.read())) { // encode gps data
        gps.f_get_position(&lat, &lon, &fix_age); // get latitude and longitude

        // Check validity of data
        if (fix_age == TinyGPS::GPS_INVALID_AGE) {
          Serial.println("No satellite found");
        } else if (fix_age > 5000) {
          Serial.println("Possible stale data");
        } else {
          // Convert into degrees-decimal-minutes-format
          return convertPos(lat, true);
        }
      }
    }
  } while (millis() - start < ms);
}

String lon(unsigned long ms) {
  float lat = 0;
  float lon = 0;
  unsigned long fix_age;
  unsigned long start = millis();
  do {
    while (gpsSerial.available()) { // check for gps data
      if (gps.encode(gpsSerial.read())) // encode gps data
      {
        gps.f_get_position(&lat, &lon); // get latitude and longitude

        if (fix_age == TinyGPS::GPS_INVALID_AGE) {
          Serial.println("No satellite found");
        } else if (fix_age > 5000) {
          Serial.println("Possible stale data");
        } else {
          // Convert into degrees-decimal-minutes-format
          return convertPos(lon, true);
        }
      }
    }
  } while (millis() - start < ms);
}

String convertPos(float value, boolean lat) {
  int deg = (int)value;
  float decMin = value - (float)deg;
  decMin = decMin * 60.0;
  String coordinate = "";
  if (lat == true) {
    if (abs(deg) == deg || deg == 0) {
      coordinate.concat("N:");
    } else {
      coordinate.concat("S:");
    }

  } else {
    if (abs(deg) == deg || deg == 0) {
      coordinate.concat("E:");
    } else {
      coordinate.concat("W:");
    }
  }

  if (deg < 10) {
    coordinate.concat("00");
  } else if (deg < 100) {
    coordinate.concat("0");
  }
  coordinate.concat(deg);
  coordinate.concat(" ");
  if (decMin < 10) {
    coordinate.concat("0");
  }
  coordinate.concat(String(decMin, 1));
  return coordinate;
}

String getSpeed(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (gpsSerial.available()) { // check for gps data
      if (gps.encode(gpsSerial.read())) // encode gps data
      {
        return String(gps.f_speed_knots(), 0);
      }
    }
  } while (millis() - start < ms);
}

String getCourse(unsigned long ms) { // VIRKER IKKE, SKAL ARBEJDES PÅ
  unsigned long start = millis();
  do {
    while (gpsSerial.available()) { // check for gps data
      if (gps.encode(gpsSerial.read())) // encode gps data
      {
        return String(gps.f_course(), 1);
      }
    }
  } while (millis() - start < ms);
}

String getTime(unsigned long ms) { // VIRKER IKKE, SKAL ARBEJDES PÅ
  int gpsYear;
  unsigned long fix_age;
  byte gpsMonth, gpsDay, gpsHour, gpsMinute, gpsSecond, gpsHundredth;
  unsigned long start = millis();
  do {
    while (gpsSerial.available()) { // check for gps data
      if (gps.encode(gpsSerial.read())) // encode gps data
      {
        gps.crack_datetime(&gpsYear, &gpsMonth, &gpsDay, &gpsHour, &gpsMinute, &gpsSecond, &gpsHundredth, &fix_age);
        String tid = "";
        tid.concat(gpsHour + 1);
        tid.concat(":");
        if (gpsMinute < 10) {
          tid.concat("0");
        }
        tid.concat(gpsMinute);
        return tid;
      }
    }
  } while (millis() - start < ms);
}


void setup() {
  Serial.begin(9600);
  Serial.println("TEST");
  initDisplay();
  gpsGUI();  
}

void loop() {
  Serial.println(getTime(1000));
//  topGUI(nuvaerendeMenu);
  
}
