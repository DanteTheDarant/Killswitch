#include <SPI.h> //libraries needed for at bruge seriel kommunikation
#include <Wire.h>
#include <Adafruit_GFX.h> //til at tegne ting paa display
#include <Adafruit_SSD1306.h>//Til Display
#include <TinyGPS.h>//til GPS
#include <nRF24L01.h>//til radio
#include <RF24.h>//til radio

//pin opsaetning
const int motorPin = 14; //pin hvorpaa relae er connect
const int knapL = 18;
const int knapR = 19; //kontakter til at styre rundt i menuer
const int buzzer = A11;
const int resetButton = A0;

//Display opsaetning
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Radio opsaetning
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00000";
unsigned long lastMessage = 2000000;
int textInt = 0;

//GPS opsaetning
TinyGPS gps; // Creating GPS object
//GPS-variabler
String GPSLat = "";
String GPSLon = "";
String GPSSpeed = "";
String GPSCourse = "";
String GPSTime = "";
String gemtLat = "";
String gemtLon = "";

//GUI vaerdier til menu position
const int pos0_y = 0;     //y-position paa info linje i top
const int posNowM_x = 15; //x-position for nuvaerende menu
const int posUr_x = 97;   //x-position for uret
const int pos1_y = 12;    //y-position paa foerste main element
const int pos2_y = 30;    //y-position paa andet main element
const int pos3_y = 49;    //y-position paa menuskift-element
unsigned long ms = 1000; //til GPS


// Menu navne
const String velkomstMenu = "Welcome";      // idk hvorfor det er paa engelsk
const String connectMenu = "Armb.";
const String gpsMenu = "GPS";
const String kursMenu = "Kurs";
const String gemtPosMenu = "G.Pos";
const String gemtDistMenu = "G.Dist";
const String alarmMenu = "Alarm";

int forbindelse = 0;//fortaeller om der er GPS forbindelse

unsigned int menuCount = 10000; //counter til hvilken menu vi er i.
//Starter hoej for at undgaa under/overflow

//checks til alarm og interrupts
bool knapCheck = 0;         //sikrer at der ikke bliver trykket dobbelt på menuknapper
bool alarm = 0;             //bliver 1/true naar mand over bord
bool interruptDisable = 0;  //deaktiverer funktionen der slukker motor hvis 1/true

void setup() {
  Serial.begin(38400);
  Serial2.begin(9600);
  initDisplay();  //initialiserer displayet
  radioSetup();   //initialiserer radio
  pinMode(motorPin, OUTPUT);
  pinMode(resetButton, INPUT);
  attachInterrupt(digitalPinToInterrupt(knapL), bMenu, RISING);
  attachInterrupt(digitalPinToInterrupt(knapR), fMenu, RISING);
  pinMode(buzzer, OUTPUT);
  digitalWrite(motorPin, HIGH);
  connectGUI();
  int i = 0;
  while (i == 0) { //saa armbaand faar forbindelse foer kode slukker motoren
    if (digitalRead(resetButton) == HIGH) {
      digitalWrite(motorPin, LOW);
      interruptDisable = 1;
      i = 1;
    }
    if (textInt == 11) {
      digitalWrite(motorPin, LOW);  
      lastMessage = millis();
      i = 1;
    }
  }
}

void loop() {
  GPSValues();
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
  int x = 1;
  while (alarm == 1) {
    if (digitalRead(resetButton) == HIGH) {
      digitalWrite(motorPin, LOW);
      alarm = 0;
      noTone(buzzer);
    }
    else if (x == 1) {
      alarmGUI();
      tone(buzzer, 5000);
      x = 0;
    }
  }
  knapCheck = 0;
}
