#include <SPI.h>//libraries til seriel kommunikation.
#include <Wire.h>
#include <Adafruit_GFX.h> //til at tegne grafik paa display
#include <Adafruit_SSD1306.h>//Til Display
#include <TinyGPS.h>//til GPS
#include <nRF24L01.h>//til radio
#include <RF24.h>//til radio

//pin opsaetning
const int motorPin = 14; //pin hvorpaa relae er connect
const int knapL = 18;
const int knapR = 19; //kontakter til at styre rundt i menuer
const int buzzer = A11; //
const int resetButton = A0; //kontakt til fx at disable alarmen

//Display opsaetning
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Radio opsaetning
RF24 radio(7, 8); // CE, CSN - pins
const byte address[6] = "00000"; 
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
const String velkomstMenu = "Welcome";
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
bool interruptDisable = 1;  //deaktiverer funktionen der slukker motor hvis 1/true
unsigned long lastMessage = 0; //tiden hvornaar sidste radiobesked blev modtaget

void setup() {
  //Serial.begin(38400);  //begynder kommunikation med serial monitor - til accepttest
  Serial2.begin(9600);  //begynder kommunikation med GPS
  initDisplay();        //initialiserer displayet
  radioSetup();         //initialiserer radio
  pinMode(motorPin, OUTPUT);
  pinMode(resetButton, INPUT);
  attachInterrupt(digitalPinToInterrupt(knapL), bMenu, RISING);  //laver interrupt pins til at reagere på stigning i spaending
  attachInterrupt(digitalPinToInterrupt(knapR), fMenu, RISING);
  pinMode(buzzer, OUTPUT);
  digitalWrite(motorPin, HIGH);
  connectGUI(); //tegner en "opstartsskaerm"
  int i = 0; // til at exit while loop
  while (i == 0) { //et loop der venter paa man disabler sikkerheden eller forbinder armbaand
    if (digitalRead(resetButton) == HIGH) {  
      digitalWrite(motorPin, LOW);
      i = 1;
    }
    if (textInt == 11) {
      digitalWrite(motorPin, LOW);
      lastMessage = millis();
      interruptDisable = 0; //aktiverer tjekket for forbindelse igen
      i = 1;
    }
  }
}

void loop() {
  GPSValues(); //opdaterer gps vaerdier 
  switch (menuCount % 4) { //afhaengig af menucount tegnes de forskellige menuer
    case 0:
      gpsGUI();   //gps skaermen
      break;
    case 1:
      kursGUI();  //kurs og far skaermen
      break;
    case 2:
      gPosGUI();  //skarmen med de gemte koordinater
      break;
    case 3:
      gDistGUI(); //skaermen med distance og retning til de gemte koordinater 
      break;
  }
  int x = 1; // til at exit while loop
  while (alarm == 1) { //loopet koden skal sidde i under alarmen
    if (digitalRead(resetButton) == HIGH) {
      digitalWrite(motorPin, LOW);
      alarm = 0;
      noTone(buzzer);
    }
    else if (x == 1) {  //skal kun koere en gang 
      alarmGUI();       //tegner alarmen
      tone(buzzer, 5000);
      x = 0;            //for at undgaa den koerer dette loop mere end en gang
    }
  }
  knapCheck = 0; //for at sikre knapperne kan skifte menu igen i naeste loop
}
