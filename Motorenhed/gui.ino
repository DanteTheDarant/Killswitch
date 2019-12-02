void initDisplay() { //initialiserer display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  //initialiser display - 0x3C er I2C adressen
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000);
  display.clearDisplay();
}

void topGUI(String nowM) { //tegner top linjen på skærmen med navnet på nuværende menu som input

  if (forbindelse == 1) { //laver en fyldt trekant hvis der er forbindelse
    display.fillTriangle(0, 0, 8, 0, 4, 7, SSD1306_WHITE);
  }
  else if (forbindelse == 0) { //laver en ikke fyldt trekant hvis forbindelse er brudt
    display.drawTriangle(0, 0, 8, 0, 4, 7, SSD1306_WHITE);
  }
  display.setTextSize(1);
  display.setTextColor(SSD1306_INVERSE);
  display.setCursor(posNowM_x, pos0_y);
  display.println(nowM);
  display.setCursor(posUr_x, pos0_y);
  display.println(getTime(1000)); //her skal indsættes en funktion der henter ur string

  display.drawLine(0, 9, 127, 9, SSD1306_WHITE);
}

void midGUI(String element1, String element2) {
  //main del af UI'et
  //display.fillRect(0, 9, 128, 35, SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(1, pos1_y);
  display.println(element1);
  display.setCursor(1, pos2_y);
  display.println(element2);
}

void bottomGUI(String nextM, String backM) {
  //delen af UI'et der viser de andre menuer
  /*
  if (nextM.length() >= 3)
  {
    nextM.remove(nextM.length()-1, 2);
  }
  */
  display.setCursor(0, pos3_y);
  display.println(backM);
  display.setCursor(128 - (nextM.length() * 12), pos3_y);
  display.println(nextM);

  // vaerdier til linjernes position
  int hStregPos = pos3_y - 2;                         // linjens hoejde
  int lStregPos = backM.length() * 12;                // venstre/midter-position
  int rStregPos = 127 - (nextM.length() * 12) - 2;    // hoejre/midter-position

  // venstre streg
  display.drawLine(0, hStregPos, lStregPos, hStregPos, SSD1306_WHITE);
  display.drawLine(lStregPos, hStregPos, lStregPos, 63, SSD1306_WHITE);

  // højre streg
  display.drawLine(127, hStregPos, rStregPos, hStregPos, SSD1306_WHITE);
  display.drawLine(rStregPos, hStregPos, rStregPos, 63, SSD1306_WHITE);
}

void kursGUI() {  
  display.clearDisplay();
  String main1 = "99 knob";  //funktion for hastighed skal indsættes
  String main2 = "201";      //funktion for kurs skal indsættes
  String nuvaerendeMenu = kursMenu;    //nuværende menu
  String forrigeMenu = gpsMenu; //menu på venstre knap
  String naesteMenu = gemtPosMenu;  //menu på højre knap

  topGUI(nuvaerendeMenu);
  midGUI(main1, main2);
  bottomGUI(naesteMenu, forrigeMenu);

  display.display();

}

void gpsGUI() {
  display.clearDisplay();
  String main1 = "N:056 31.2"; //funktion for koordinat skal indsættes
  String main2 = "E:009 42.1"; //funktion for koordinat skal indsættes
  String nuvaerendeMenu = gpsMenu;    //nuværende menu
  String forrigeMenu = gemtPosMenu; //menu på venstre knap
  String naesteMenu = kursMenu;  //menu på højre knap

  topGUI(nuvaerendeMenu);
  midGUI(main1, main2);
  bottomGUI(naesteMenu, forrigeMenu);
  display.display();
}
