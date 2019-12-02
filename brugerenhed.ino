#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00000";    //Transmitterens 5-cifrede adresse 
const byte channel = 0;             //Transmitterens kanal (0-124)
void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(channel);        //Transmitterens kanal
  radio.stopListening();
  Serial.begin(2000000);
}
void loop() {
  const char text[] = "11";
  radio.write(&text, sizeof(text));
  //Serial.println("Besked sendt");
  //radio.write(&address, sizeof(address));
  Serial.print("Besked sendt på kanal: "); Serial.print(channel); Serial.print(" og adresse: "); Serial.print("0000"); Serial.write(address[4]); Serial.println();
  Serial.print("Beskeden er: "); Serial.println(text);
  delay(100);
}
