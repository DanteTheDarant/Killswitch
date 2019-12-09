void initDisplay() { //initialiserer display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  //initialiser display - 0x3C er I2C adressen
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000);
  display.clearDisplay();
}

void topGUI(String nowM) { //tegner top linjen paa skaermen med navnet paa nuvaerende menu som input

  //tjekker om der er forbindelse med GPS og armbaand foerst
  if (interruptDisable == 0) { //laver en fyldt trekant hvis der er forbindelse til armbånd
    display.fillTriangle(0, 0, 8, 0, 4, 7, SSD1306_WHITE);
  }
  else if (interruptDisable == 1) { //laver en ikke fyldt trekant hvis forbindelse til armbånd er brudt
    display.drawTriangle(0, 0, 8, 0, 4, 7, SSD1306_WHITE);
    tone(buzzer, 800, 100);
    //Serial.println("Alarmtone, bip"); //accepttest 4
  }
  if (forbindelse == 1) { //laver en fyldt trekant hvis der er forbindelse til GPS
    display.fillCircle(posUr_x - 7, 4, 4, SSD1306_WHITE);
  }
  else if (forbindelse == 0) { //laver en ikke fyldt trekant hvis forbindelse til GPS er brudt
    display.drawCircle(posUr_x - 7, 4, 4, SSD1306_WHITE);
  }

  display.setTextSize(1);
  display.setTextColor(SSD1306_INVERSE);
  display.setCursor(posNowM_x, pos0_y);
  display.println(nowM);
  display.setCursor(posUr_x, pos0_y);
  display.println(GPSTime); //tiden hentet fra GPS'en
  display.drawLine(0, 9, 127, 9, SSD1306_WHITE); //linje til at adskille top linje fra resten
}


void midGUI(String element1, String element2) { //tegner main delen af GUI'et med de to hovedelemter som input (som strings)
  display.setTextSize(2);
  display.setCursor(1, pos1_y);
  display.println(element1);
  display.setCursor(1, pos2_y);
  display.println(element2);
}


void bottomGUI(String nextM, String backM) {//delen af UI'et der viser navigation til andre menuer - input er strings med forrige og naeste menu
  display.setCursor(0, pos3_y);
  display.println(backM);
  display.setCursor(128 - (nextM.length() * 12), pos3_y);
  display.println(nextM);

  // vaerdier til linjer omkring tekst position
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


void connectGUI() { //GUI til connection med armbaand menu
  display.clearDisplay();
  display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE); //gør baggrunden hvid
  topGUI("CONNECT");
  display.setTextColor(SSD1306_INVERSE);
  display.setCursor(1, 12);
  display.setTextSize(2);
  display.println("Tilslut");
  display.setCursor(1, 47);
  display.setTextSize(1);
  display.println("Bypass ved tryk paa reset");
  display.display();
}

void gpsGUI() { //GUI til GPS menu
  display.clearDisplay();
  String main1 = GPSLat;              //variabel indeholdende koordinat
  String main2 = GPSLon;              //variabel indeholdende koordinat
  String nuvaerendeMenu = gpsMenu;    //nuvaerende menu
  String forrigeMenu = gemtDistMenu;  //menu paa venstre knap
  String naesteMenu = kursMenu;       //menu paa hoejre knap
  topGUI(nuvaerendeMenu);
  midGUI(main1, main2);
  bottomGUI(naesteMenu, forrigeMenu);
  //Serial.println("SKAERM ER OPDATERET"); //acceptest 3
  display.display();
}

void kursGUI() { //GUI til kurs og fart menu
  display.clearDisplay();
  String main1 = GPSSpeed;            //variabel indeholdende hastighed
  String main2 = GPSCourse;           //variabel indeholdende kurs
  String nuvaerendeMenu = kursMenu;   //nuvaerende menu
  String forrigeMenu = gpsMenu;       //menu paa venstre knap
  String naesteMenu = gemtPosMenu;    //menu paa hoejre knap
  topGUI(nuvaerendeMenu);
  midGUI(main1, main2);
  bottomGUI(naesteMenu, forrigeMenu);
  display.display();
}

void gPosGUI() { //GUI til menuen med den gemte position
  display.clearDisplay();
  String main1 = gemtLat;               //gemt koordinat N
  String main2 = gemtLon;               //gemt koordinat E
  String nuvaerendeMenu = gemtPosMenu;  //nuvaerende menu
  String forrigeMenu = kursMenu;        //menu paa venstre knap
  String naesteMenu = gemtDistMenu;     //menu paa hoejre knap
  topGUI(nuvaerendeMenu);
  midGUI(main1, main2);
  bottomGUI(naesteMenu, forrigeMenu);
  display.display();
}

void gDistGUI() { //GUI til menuen med distance og kurs til gemt position
  display.clearDisplay();
  String main1 = "dist";                //funktion for dist til gemt pos
  String main2 = "kurs";                //funktion for kurs til gemt pos
  String nuvaerendeMenu = gemtDistMenu; //nuvaerende menu
  String forrigeMenu = gemtPosMenu;     //menu paa venstre knap
  String naesteMenu = gpsMenu;          //menu paa hoejre knap
  topGUI(nuvaerendeMenu);
  midGUI(main1, main2);
  bottomGUI(naesteMenu, forrigeMenu);
  display.display();
}

void alarmGUI() { //GUI til alarmen
  display.clearDisplay();
  display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  display.drawTriangle(41, 1, 86, 1, 64, 40, SSD1306_BLACK);
  display.setTextSize(1);
  display.setCursor(1, 44);
  display.println("MOB, position gemt");
  display.setCursor(1, 54);
  display.println("Bypass ved tryk reset");
  display.display();
}
