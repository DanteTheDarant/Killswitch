/*
 * bMenu og fMenu aendrer "menuCount" variablen 1 op eller ned
 * Bruger knaptjek til at sikre knappen ikke bouncer
 */
void bMenu() {
  if (knapCheck == 0) {
    menuCount--;
    knapCheck = 1;
  }
}

void fMenu() {
  if (knapCheck == 0) {
    menuCount++;
    knapCheck = 1;
  }
}

/*
 * CTC interrupt funktion der tjekker for om der er gaaet
 * over 2 sekunder siden sidst modtagne besked. Hvis "interruptDisable"
 * er 1/true gør funktionen intet
 * Hvis det er 2 sekunder siden funktionen besked modtaget slukkes motor
 */
ISR(TIMER1_COMPA_vect) {
  if (lastMessage + 680 < millis() && interruptDisable == 0) {
    Serial.println("hey");
    digitalWrite(motorPin, HIGH);
    gemtLat = GPSLat;
    gemtLon = GPSLon;
    alarm = 1;
    interruptDisable = 1;
    //saet funktion ind til at gemme koordinat her
  }
}

/* 
 * funktion der koerer hver gang radioen modtager signal
 * Aendrer variablen "lastMessage" til den nuværende tid
 * Goer ligeledes "interruptDisable" falsk hvis alarmen er slået fra
 * og forbindelse bliver genoprettet
 */
void radioCheck() { 
  Serial.println("hola");
  textInt = 0;
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    textInt = atoi(text);
  }
  if (textInt == 11) {
    lastMessage = millis();
  }
  if (interruptDisable == 1 && alarm == 0){ //for at aktivere doedmandsknappen igen naar bruger er tilbage
    interruptDisable = 0;
  }
}
