/* 
 * Opsaetter radio funktionaliteten og CTC interrupt 
 * Radiodelen indeholder også opsaetningen af interruptet
 * CTC (clear timer on compare): Goer saaledes at arduinoen taeller op til 
 * en given vaerdi. Den taeller med hastigheden af clockfrekvens delt med en 
 * prescaler. Se linje 25-26 for prescaler vaerdi
 */
void radioSetup() {
  radio.begin();
  radio.maskIRQ(1, 1, 0); //Masks alle IRQ triggers, udover receive(1 er mask, 0 er ingen mask).
  radio.openReadingPipe(0, address); 
  radio.setChannel(0);
  radio.setPALevel(RF24_PA_MIN);  //sende styrken. Minimum er rigeligt til vores behov
  radio.startListening();         //begynder herfra at lytte efter beskeder
  attachInterrupt(1, radioCheck, FALLING); //Opsaetter et interrupt for Arduino interrupt pin 1(D3), for hvornaar vaerdien falder fra 3.3 volt. Her koerer den vores ISR.

  //Setting up timer1 interrupt at ca 1.11 Hz eller 900 ms
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1.11 Hz eller 900 ms increments
  OCR1A = 18749;// (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 256 prescaler
  TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei(); //tillad interrupts
}
