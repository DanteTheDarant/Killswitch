String lat(unsigned long ms) {
  float lat = 0;
  float lon = 0;
  unsigned long fix_age;
  unsigned long start = millis();
  do {
    while (gpsSerial.available()) { // check for gps data
      if (gps.encode(gpsSerial.read())) { // encode gps data
        gps.f_get_position(&lat, &lon, &fix_age); // get latitude and longitude

        // Check validity of data
        if (fix_age == TinyGPS::GPS_INVALID_AGE) {
          Serial.println("No satellite found");
        } else if (fix_age > 5000) {
          Serial.println("Possible stale data");
        } else {
          // Convert into degrees-decimal-minutes-format
          return convertPos(lat, true);
        }
      }
    }
  } while (millis() - start < ms);
}

String lon(unsigned long ms) {
  float lat = 0;
  float lon = 0;
  unsigned long fix_age;
  unsigned long start = millis();
  do {
    while (gpsSerial.available()) { // check for gps data
      if (gps.encode(gpsSerial.read())) // encode gps data
      {
        gps.f_get_position(&lat, &lon); // get latitude and longitude

        if (fix_age == TinyGPS::GPS_INVALID_AGE) {
          Serial.println("No satellite found");
        } else if (fix_age > 5000) {
          Serial.println("Possible stale data");
        } else {
          // Convert into degrees-decimal-minutes-format
          return convertPos(lon, false);
        }
      }
    }
  } while (millis() - start < ms);
}

String convertPos(float value, boolean lat) {
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

String getSpeed(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (gpsSerial.available()) { // check for gps data
      if (gps.encode(gpsSerial.read())) // encode gps data
      {
        return String(gps.f_speed_knots(), 0);
      }
    }
  } while (millis() - start < ms);
}

String getCourse(unsigned long ms) { // VIRKER IKKE, SKAL ARBEJDES PÅ
  unsigned long start = millis();
  do {
    while (gpsSerial.available()) { // check for gps data
      if (gps.encode(gpsSerial.read())) // encode gps data
      {
        return String(gps.f_course(), 1);
      }
    }
  } while (millis() - start < ms);
}

String getTime(unsigned long ms) { // VIRKER IKKE, SKAL ARBEJDES PÅ
  int gpsYear;
  unsigned long fix_age;
  byte gpsMonth, gpsDay, gpsHour, gpsMinute, gpsSecond, gpsHundredth;
  unsigned long start = millis();
  do {
    while (gpsSerial.available()) { // check for gps data
      if (gps.encode(gpsSerial.read())) // encode gps data
      {
        gps.crack_datetime(&gpsYear, &gpsMonth, &gpsDay, &gpsHour, &gpsMinute, &gpsSecond, &gpsHundredth, &fix_age);
        String tid = "";
        tid.concat(gpsHour + 1);
        tid.concat(":");
        if (gpsMinute < 10) {
          tid.concat("0");
        }
        tid.concat(gpsMinute);
        return tid;
      }
    }
  } while (millis() - start < ms);
}