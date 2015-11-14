/*
 * Initialisierung des GPS
 */
void initGPS() {
  softwareSerial.begin(9600);


  while (!softwareSerial) {
    DEBUG_PRINT("Serial nicht erreichbar");
  }
}

/*
 * read the SoftwareSerial so that no bufferOverflow occurs
 */
bool feedGPS() {
  while (softwareSerial.available() > 0) {
    if (gps.encode(softwareSerial.read()))
      return true;
  }
  return false;
}

/*
 * gets GPS position from gps
 * writes data directly to struct
 */
void getGPSPosition(struct data *position) {
  position->lat = (float)gps.location.lat();
  position->lng = (float)gps.location.lng();

  position->numberSatellites = int(gps.satellites.value());
  position->hdop = int(gps.hdop.value() / 100.0);
}

/*
 * gets GPS time from gps
 * writes data directly to struct
 */
void getGPSTime(struct data *position) {
  position->year = (gps.date.year());
  position->month = (gps.date.month());
  position->day = (gps.date.day());
  sprintf(position->date, "%02d.%02d.%4d",
          position->day, position->month, position->year);

  position->hoD = int(gps.time.hour());
  position->minD = int(gps.time.minute());
  position->secD = int(gps.time.second());
  sprintf(position->time, "%02d:%02d:%02d",
          position->hoD, position->minD, position->secD);
}

