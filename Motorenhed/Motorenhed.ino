#include <SPI.h> //libraries needed for at bruge display
#include <Wire.h> //findes hvis man søger i Arduino IDE'en
#include <Adafruit_GFX.h> //til at tegne ting osv
#include <Adafruit_SSD1306.h>//Til Display
#include <SoftwareSerial.h>
#include <TinyGPS.h>//til GPS
#include <nRF24L01.h>//til radio
#include <RF24.h>//til radio

//Display opsætning
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Radio opsætning
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00000";
unsigned long lastmessage = 0;

//GPS opsætning
SoftwareSerial gpsSerial(4, 5); // Setting up GPS-pins for RX and TX
TinyGPS gps; // Creating GPS object
const int pos0_y = 0;     //y-position på info linje i top
const int posNowM_x = 15; //x-position for nuværende menu
const int posUr_x = 97;   //x-position for uret
const int pos1_y = 12;    //y-position på første main element
const int pos2_y = 30;    //y-position på andet main element
const int pos3_y = 49;    //y-position på menuskift-element
unsigned long ms = 1000; //til GPS

const int motorPin = 14; //pin hvorpå relæ er connect
const int knapL = 35;
const int knapR = 33; //kontakter til at styre rundt i menuer

// Menu navne
const String velkomstMenu = "Welcome";      // idk hvorfor det er på engelsk
const String connectMenu = "Armb.";
const String gpsMenu = "GPS";
const String kursMenu = "Kurs";
const String gemtPosMenu = "G.Pos";
const String gemtDistMenu = "G.Dist";
const String alarmMenu = "Alarm";

int forbindelse = 0;//fortaeller om der er GPS forbindelse

unsigned int menuCount = 10000; //counter til hvilken menu vi er i

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  initDisplay();  //initialiserer displayet
  radioSetup();   //initialiserer GPS
  pinMode(motorPin, OUTPUT);
  pinMode(knapL, INPUT);
  pinMode(knapR, INPUT);
  digitalWrite(motorPin, LOW);
  while (lastmessage <= 2000 || knapL == 1 && knapR == 1) { //checker om vores
    connectGUI();
  }

}

void loop() {
  if (knapL == 1) {
    menuCount--;
  }
  if (knapL == 1) {
    menuCount++;
  }
  switch (menuCount % 5) {
    case 0:
      connectGUI();
      //en menu
      break;
    case 1:
      gpsGUI();
      //en menu
      break;
    case 2:
      kursGUI();
      //en menu
      break;
    case 3:
      gPosGUI();
      //en menu
      break;
    case 4:
      gDistGUI();
      //en menu
      break;
  }
}
