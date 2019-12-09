/*
   Funktion der gemmer fart, kurs, lon, lat og tid
*/
void GPSValues() {
  forbindelse = 0;
  float tempLat, tempLon, tempCourse;                                   //Værdier til at bruge i funktionerne
  unsigned long fix_age;                                                //Værdier til at bruge i funktionerne
  int gpsYear;                                                          //Værdier til at bruge i funktionerne
  byte gpsMonth, gpsDay, gpsHour, gpsMinute, gpsSecond, gpsHundredth;   //Værdier til at bruge i funktionerne
  unsigned long start = millis();
  do {
    while (Serial2.available()) { // Så længe der er forbindelse til satellitterne
      if (gps.encode(Serial2.read())) { //Hvis der er noget data at læse, encode det, så det bliver normale tal
        gps.f_get_position(&tempLat, &tempLon, &fix_age); // Funktion til at få længe- og breddegrader

        // check for, om der er kommet et satellit-fix for nylig
        if (fix_age == TinyGPS::GPS_INVALID_AGE) { // Ingen forbindelse til satellitter
          Serial.println("No satellite found");
        }
        else if (fix_age > 5000) { // Gammel forbindelse til satellitter.
          Serial.println("Possible stale data");
        }
        else { // God data
          GPSTime = "";    //Tomme strings, som bliver fyldt længere nede
          GPSCourse = "";  //Tomme strings, som bliver fyldt længere nede
          tempCourse = gps.f_course(); //Funktion for at finde kursen i grader
          
          // For at kursen altid fylder 3 cifre, bliver der indsat et eller to 0'er
          if (tempCourse < 10) {
            GPSCourse.concat("00");
          } else if (tempCourse < 100) {
            GPSCourse.concat("0");
          }
          GPSCourse.concat(String(tempCourse, 1)); //sidste tal er antal decimaler
          GPSCourse.concat(" grader");
          GPSSpeed = String(gps.f_speed_knots(), 1); //sidste tal er antal decimaler
          GPSSpeed.concat(" knob");

          GPSLat = convertPos(tempLat, true);   //Omformatering til degrees-decimalminutes-format
          GPSLon = convertPos(tempLon, false);  //Omformatering til degrees-decimalminutes-format
          
          gps.crack_datetime(&gpsYear, &gpsMonth, &gpsDay, &gpsHour, &gpsMinute, &gpsSecond, &gpsHundredth); // Funktion for tid
          
          gpsHour += 1; //Tidszone-fix
          if (gpsHour < 10) { //Så timerne altid fylder 2 cifre
             GPSTime.concat("0");
          }
          GPSTime.concat(gpsHour); 
          GPSTime.concat(":");
             
          if (gpsMinute < 10) { //Så minutterne altid fylder 2 cifre
            GPSTime.concat("0");
          }
          GPSTime.concat(gpsMinute);
          forbindelse = 1;
        }
      }
    }
  } while (millis() - start < ms);
  if (forbindelse == 0) { //hvis der ingen forbindelse er, printer den "FEJL"
    GPSTime = "FEJL";
    GPSCourse = "FEJL";
    GPSSpeed = "FEJL";
    GPSLat = "FEJL";
    GPSLon = "FEJL";
  }
}

/*
   Funktion der modtager en float og en bool
   Floaten er en GPS vaerdi der bliver omregnet til det rette format
   Bool'en skal vaere true eller false afhaengig af om vaerdien er latitude eller longitude
   Outputtet er en string med tilfoejet prefix
*/
String convertPos(float value, bool lat) {
  int deg = (int)value;                      //For at få alt inden kommaet
  float decMin = value - (float)deg;         //For at få alt efter kommaet
  decMin = decMin * 60.0;                    // I ddm-format er dm 1/60 af grader
  String coordinate = "";
  if (lat == true) { //Hvis lat er true, skal den vælge mellem N eller S.
    if (abs(deg) == deg || deg == 0) { //Hvis den er positiv, skal det være N, ellers S
      coordinate.concat("N:");
    } else {
      coordinate.concat("S:");
    }

  } else { //Hvis lat er false, skal den vælge mellem E eller W.
    if (abs(deg) == deg || deg == 0) { //Hvis den er positiv, skal det være E, ellers W
      coordinate.concat("E:");
    } else {
      coordinate.concat("W:");
    }
  }

  if (deg < 10) { // Sørger for at graderne altid har det samme antal cifre
    coordinate.concat("00");
  } else if (deg < 100) {
    coordinate.concat("0");
  }
  coordinate.concat(deg);
  coordinate.concat(" ");
  if (decMin < 10) {
    coordinate.concat("0");
  }
  coordinate.concat(String(decMin, 1));
  return coordinate;
}
