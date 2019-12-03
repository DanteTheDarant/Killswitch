#include <SPI.h> //libraries needed for at bruge display
#include <Wire.h> //findes hvis man søger i Arduino IDE'en
#include <Adafruit_GFX.h> //til at tegne ting osv
#include <Adafruit_SSD1306.h>//Til Display
#include <TinyGPS.h>//til GPS
#include <nRF24L01.h>//til radio
#include <RF24.h>//til radio

//pin opsaetning
const int motorPin = 14; //pin hvorpå relæ er connect
const int knapL = 33;
const int knapR = 35; //kontakter til at styre rundt i menuer
const int buzzer = 12;

//Display opsaetning
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Radio opsætning
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00000";
unsigned long lastMessage = 2000000;
int textInt = 0;

//GPS opsætning
TinyGPS gps; // Creating GPS object
const int pos0_y = 0;     //y-position på info linje i top
const int posNowM_x = 15; //x-position for nuværende menu
const int posUr_x = 97;   //x-position for uret
const int pos1_y = 12;    //y-position på første main element
const int pos2_y = 30;    //y-position på andet main element
const int pos3_y = 49;    //y-position på menuskift-element
unsigned long ms = 1000; //til GPS

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
  Serial2.begin(9600);
  initDisplay();  //initialiserer displayet
  radioSetup();   //initialiserer GPS
  pinMode(motorPin, OUTPUT);
  pinMode(knapL, INPUT);
  pinMode(knapR, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(motorPin, LOW);
  connectGUI();
  int i = 0;
  while (i == 0) { //saa armbaand faar forbindelse foer kode slukker motoren
    if (digitalRead(knapL) == HIGH || digitalRead(knapR) == HIGH) {
      i = 1;
    }
    if (textInt == 11) {
      lastMessage = millis();
      i = 1;
    }
  }
}

void loop() {
  Serial.print(millis());
  Serial.println("  start");
  if (digitalRead(knapL) == HIGH) {
    menuCount--;
  }
  else if (digitalRead(knapR) == HIGH) {
    menuCount++;
  }
  switch (menuCount % 4) {
    case 0:
      gpsGUI();
      //en menu
      break;
    case 1:
      kursGUI();
      //en menu
      break;
    case 2:
      gPosGUI();
      //en menu
      break;
    case 3:
      gDistGUI();
      //en menu
      break;
  }
  Serial.print(millis());
  Serial.println("  stop");
}
