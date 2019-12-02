void radioCheck() {
  int textInt = 0;
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    textInt = atoi(text);
  }
  if (textInt == 11) {
    lastmessage = millis();
    //Serial.print("Lastmessage: ");Serial.println(lastmessage);
  }
  //Serial.print("Læst værdi: ");Serial.println(textInt);
}
void radioSetup() {
  radio.begin();
  radio.maskIRQ(1, 1, 0); //Masks alle IRQ triggers, udover receive(1 er mask, 0 er ingen mask).
  radio.openReadingPipe(0, address);
  radio.setChannel(0);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  attachInterrupt(1, radioCheck, FALLING); //Opsætter et interrupt for Arduino interrupt pin 1(D3), for hvornår værdien falder fra 3.3 volt. Her kører den vores ISR.

  //Setting up timer1 interrupt at ca 5.56Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 5.56hz increments
  OCR1A = 11240;// (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 256 prescaler
  TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei(); //tillad interrupts
}

ISR(TIMER1_COMPA_vect) {
  if (lastmessage + 2000 < millis()) {
//    Serial.print("Lastmessage: "); Serial.println(lastmessage);
//    Serial.print("millis: "); Serial.println(millis());
//    Serial.print("Stop motoren");
    digitalWrite(motorPin, HIGH);
  }
}