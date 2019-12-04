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
  display.println(getTime()); //funktion der henter tid fra GPS
  display.drawLine(0, 9, 127, 9, SSD1306_WHITE); //linje til at adskille top linje fra resten
}

void midGUI(String element1, String element2) {
  //main del af UI'et
  display.setTextSize(2);
  display.setCursor(1, pos1_y);
  display.println(element1);
  display.setCursor(1, pos2_y);
  display.println(element2);
}

void bottomGUI(String nextM, String backM) {
  //delen af UI'et der viser de andre menuer
  display.setCursor(0, pos3_y);
  display.println(backM);
  display.setCursor(128 - (nextM.length() * 12), pos3_y);
  display.println(nextM);

  // vaerdier til linjernes position
  int hStregPos = pos3_y - 2;                         // linjens hoejde
  int lStregPos = backM.length() * 12;                // venstre/midter-position
  int rStregPos = 127 - (nextM.length() * 12) - 2;    // hoejre/midter-position

  // venstre linje rundt om tekst
  display.drawLine(0, hStregPos, lStregPos, hStregPos, SSD1306_WHITE);
  display.drawLine(lStregPos, hStregPos, lStregPos, 63, SSD1306_WHITE);

  // hoejre linje rundt om tekst
  display.drawLine(127, hStregPos, rStregPos, hStregPos, SSD1306_WHITE);
  display.drawLine(rStregPos, hStregPos, rStregPos, 63, SSD1306_WHITE);
}

void connectGUI() { //GUI til connection med armbånd menu
  display.clearDisplay();
  display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  topGUI("CONNECT");
  String connectText = "Tilslut";
  display.setTextColor(SSD1306_INVERSE);
  display.setCursor(1,12);
  display.setTextSize(2);
  display.println(connectText);
  display.setCursor(1,47);
  display.setTextSize(1);
  display.println("Bypass ved tryk på begge knapper");
  display.display();
}

void gpsGUI() { //GUI til GPS menu
  display.clearDisplay();
  String main1 = GPSLat;               //funktion for koordinat
  String main2 = GPSLon;               //funktion for koordinat
  String nuvaerendeMenu = gpsMenu;    //nuvaerende menu
  String forrigeMenu = gemtDistMenu;  //menu på venstre knap
  String naesteMenu = kursMenu;       //menu på hoejre knap
  topGUI(nuvaerendeMenu);
  midGUI(main1, main2);
  bottomGUI(naesteMenu, forrigeMenu);
  display.display();
}

void kursGUI() { //GUI til kurs og fart menu
  display.clearDisplay();
  String main1 = GPSSpeed;          //funktion for hastighed
  String main2 = "201";               //funktion for kurs skal indsættes
  String nuvaerendeMenu = kursMenu;   //nuvaerende menu
  String forrigeMenu = gpsMenu;       //menu på venstre knap
  String naesteMenu = gemtPosMenu;    //menu på hoejre knap
  topGUI(nuvaerendeMenu);
  midGUI(main1, main2);
  bottomGUI(naesteMenu, forrigeMenu);
  display.display();
}

void gPosGUI() { //GUI til menuen med den gemte position
  display.clearDisplay();
  String main1 = "gemt1";               //gemt koordinat N
  String main2 = "gemt2";               //gemt koordinat E
  String nuvaerendeMenu = gemtPosMenu;  //nuvaerende menu
  String forrigeMenu = kursMenu;        //menu på venstre knap
  String naesteMenu = gemtDistMenu;     //menu på højre knap
  topGUI(nuvaerendeMenu);
  midGUI(main1, main2);
  bottomGUI(naesteMenu, forrigeMenu);
  display.display();
}

void gDistGUI() {
  display.clearDisplay();
  String main1 = "dist";                //funktion for dist til gemt pos
  String main2 = "kurs";                //funktion for kurs til gemt pos
  String nuvaerendeMenu = gemtDistMenu; //nuvaerende menu
  String forrigeMenu = gemtPosMenu;     //menu på venstre knap
  String naesteMenu = gpsMenu;          //menu på hoejre knap
  topGUI(nuvaerendeMenu);
  midGUI(main1, main2);
  bottomGUI(naesteMenu, forrigeMenu);
  display.display();
}

void alarmGUI() {
  display.clearDisplay();
  display.fillRect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,SSD1306_WHITE);
  display.drawTriangle(41,1,86,1,64,40,SSD1306_BLACK);
  display.setTextSize(1);
  display.setCursor(1,44);
  display.println("MOB, position gemt");
  display.setCursor(1,54);
  display.println("Bypass ved tryk på begge knapper");
  display.display();
}
