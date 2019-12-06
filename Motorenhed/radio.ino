/*
   Opsaetter radio funktionaliteten og CTC interrupt
   Radiodelen indeholder også opsaetningen af interruptet
   CTC (clear timer on compare): Goer saaledes at arduinoen taeller op til
   en given vaerdi. Den taeller med hastigheden af clockfrekvens delt med en
   prescaler. Se linje 25-26 for prescaler vaerdi
*/
void radioSetup() {
  radio.begin();
  radio.maskIRQ(1, 1, 0); //Masks alle IRQ triggers, udover receive(1 er mask, 0 er ingen mask).
  radio.openReadingPipe(0, address);
  radio.setChannel(0);
  radio.setPALevel(RF24_PA_MAX);  //sende styrken.
  radio.startListening();         //begynder herfra at lytte efter beskeder
  attachInterrupt(1, radioCheck, FALLING); //Opsaetter et interrupt for Arduino interrupt pin 1(D3), for hvornaar vaerdien falder fra 3.3 volt. Her koerer den vores ISR.

  //Setting up CTC interrupt at approx 3.33 Hz or 300 ms
  TCCR1A = 0; // Reset Timer Counter Control Registers
  TCCR1B = 0; // Same for B
  bitSet(TCCR1B, WGM12); // turn on CTC mode
  // Set Presaler:
  // 256: Set CS12
  bitSet(TCCR1B, CS12); // = 00000100 = Bit 2
  // Compare Mode
  bitSet(TIMSK1, OCIE1A); //Enable CompareInterrupt.
  OCR1A = 18749; // Set Compare Value ( < 65536 )
  sei(); //= interrupts(); Enable all Interrupts
}
