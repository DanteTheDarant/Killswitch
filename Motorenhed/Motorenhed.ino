#include <SPI.h> //libraries needed for at bruge display
#include <Wire.h> //findes hvis man søger i Arduino IDE'en
#include <Adafruit_GFX.h> //til at tegne ting osv
#include <Adafruit_SSD1306.h>//Til Display
#include <SoftwareSerial.h>
#include <TinyGPS.h>//til GPS

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

void setup() {
  Serial.begin(9600);
  Serial.println("TEST");
  initDisplay();
  gpsGUI();
}

void loop() {
  Serial.println(getTime(500));

}
