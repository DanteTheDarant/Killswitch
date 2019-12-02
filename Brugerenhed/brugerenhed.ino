#include <SPI.h>                  //Includes the library for Serial Peripheral Interface
#include <nRF24L01.h>             //Includes the library for the specific radiomodule
#include <RF24.h>                 //Includes the library for the radiomodule
RF24 radio(7, 8);                 //Pins for CE og CSN
const byte address[6] = "00000";  //The transmitter's 5-digit address
const byte channel = 0;           //The transmitter's channel (0-124)
const char text[] = "11";         //The transmitted text
void setup() {
  radio.begin();                  //Begins radiocommunication
  radio.openWritingPipe(address); //Opens a datapipe on the address 
  radio.setPALevel(RF24_PA_MIN);  //Sets the level for Power Amplifier
  radio.setChannel(channel);      //The transmitter's channel
  radio.stopListening();          //The module is set to only transmit
  Serial.begin(9600);  
}


void loop() {
  radio.write(&text, sizeof(text));
  //Serial.println("Besked sendt");
  //radio.write(&address, sizeof(address));
  //Serial.print("Besked sendt på kanal: "); Serial.print(channel); Serial.print(" og adresse: "); Serial.print("0000"); Serial.write(address[4]); Serial.println();
  //Serial.print("Beskeden er: "); Serial.println(text);
  delay(200);
}
