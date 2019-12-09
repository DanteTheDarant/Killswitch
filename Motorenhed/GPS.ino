/*
   Funktion der gemmer fart, kurs, lon, lat og tid
*/
void GPSValues() {
  forbindelse = 0;
  float tempLat, tempLon, tempCourse;
  unsigned long fix_age;
  int gpsYear;
  byte gpsMonth, gpsDay, gpsHour, gpsMinute, gpsSecond, gpsHundredth;
  unsigned long start = millis();
  do {
    while (Serial2.available()) { // check for gps data
      if (gps.encode(Serial2.read())) { // encode gps data
        gps.f_get_position(&tempLat, &tempLon, &fix_age);

        // Check validity of data
        if (fix_age == TinyGPS::GPS_INVALID_AGE) {
          Serial.println("No satellite found");
        }
        else if (fix_age > 5000) {
          Serial.println("Possible stale data");
        }
        else {
          GPSTime = "";
          GPSCourse = "";
          tempCourse = gps.f_course();
          if (tempCourse < 10) {
            GPSCourse.concat("00");
          } else if (tempCourse < 100) {
            GPSCourse.concat("0");
          }
          GPSCourse.concat(String(tempCourse, 1)); //sidste tal er antal decimaler
          GPSCourse.concat(" grader");
          GPSSpeed = String(gps.f_speed_knots(), 1); //sidste tal er antal decimaler
          GPSSpeed.concat(" knob");

          GPSLat = convertPos(tempLat, true);
          GPSLon = convertPos(tempLon, false);

          gps.crack_datetime(&gpsYear, &gpsMonth, &gpsDay, &gpsHour, &gpsMinute, &gpsSecond, &gpsHundredth);
          GPSTime.concat(gpsHour + 1);
          GPSTime.concat(":");
          if (gpsMinute < 10) {
            GPSTime.concat("0");
          }
          GPSTime.concat(gpsMinute);
          forbindelse = 1;
        }
      }
    }
  } while (millis() - start < ms);
  if (forbindelse == 0) { //hvis der ingen forbindelse er, printer den fejl
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
  int deg = (int)value;
  float decMin = value - (float)deg;
  decMin = decMin * 60.0;
  String coordinate = "";
  if (lat == true) {
    if (abs(deg) == deg || deg == 0) {
      coordinate.concat("N:");
    } else {
      coordinate.concat("S:");
    }

  } else {
    if (abs(deg) == deg || deg == 0) {
      coordinate.concat("E:");
    } else {
      coordinate.concat("W:");
    }
  }

  if (deg < 10) {
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
