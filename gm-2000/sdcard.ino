#define PIN_CHIP_SELECT 4 //pin for accessing the sd
#define CONFIG_FILE "conf.txt" //config file to store the current logfile name

long counter = 0;

/*
 * opens communication to sd card
 */
void initSDCard() {
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH); // Add this line

  // see if the card is present and can be initialized:
  if (!SD.begin(PIN_CHIP_SELECT)) {
    //if (!SD.begin(10, 11, 12, 13)) {
    write("Card failed", "or not present");
    while (true);
    //DEBUG_PRINTLN("Card failed, or not present");
    // don't do anything more:
    return;
  }
  write("SD initialized");
  delay(1000);
  //DEBUG_PRINTLN("Card initialized.");
  lcd.clear();
}

/*
 * store measurement to SD
 */
void storeMeasurement(struct temperature *currentData) {
  createLogFile(); //writes headers if file does not exist
  DEBUG_PRINTLN(filename);
  File datafile = SD.open(filename, FILE_WRITE);
  // if the file is available, write to it:
  if (datafile) {
    datafile.print(counter++);
    datafile.print(",");
    datafile.print(fix_data.latitude(), 10);
    datafile.print(",");
    datafile.print(fix_data.longitude(), 10);
    datafile.print(",");

    datafile.print(fix_data.dateTime.full_year(fix_data.dateTime.year));
    datafile.print('-');
    if (fix_data.dateTime.month < 10) datafile.print('0');
    datafile.print(fix_data.dateTime.month) ;
    datafile.print('-');
    if (fix_data.dateTime.date < 10) datafile.print('0');
    datafile.print(fix_data.dateTime.date);
    datafile.print(",");

    if (fix_data.dateTime.hours < 10) datafile.print('0');
    datafile.print(fix_data.dateTime.hours);
    datafile.print(':');
    if (fix_data.dateTime.minutes < 10) datafile.print('0');
    datafile.print(fix_data.dateTime.minutes);
    datafile.print(':');
    if (fix_data.dateTime.seconds < 10) datafile.print('0');
    datafile.print(fix_data.dateTime.seconds);

    datafile.print(",");
    datafile.print(currentData->tempBTS, 5);
    datafile.print(",");
    datafile.println(currentData->tempAir, 5);

    // dataFile.println(position);
    datafile.close();
    //DEBUG_PRINTLN("Measurement done");
    writeConfig(filename, false);
    DEBUG_PRINTLN(F("write config to SD"));
  }
  // if the file isn't open, pop up an error:
  else {
    write("Error writing", "measurement to file");
    //waitButton();
    //DEBUG_PRINTLN("Error open file");
    delay(3000);
  }

  write("measurement", "stored to SD");
  //TODO: get rid of delay
  delay(750);
  updateCmdMenu(BUTTON_NONE);
}


/*
 * reads the logfilename from the config file to filename
 */
void readConfig(char* filename) {
  uint8_t pos = 0;
  char character;
  File configFile = SD.open(CONFIG_FILE);

  String settingValue;

  if (configFile) {
    while (configFile.available()) {
      character = configFile.read();
      //while( configFile.available() && (character != '[')){
      //  character = configFile.read();
      //}

      // read the current filename
      while (configFile.available() && (character != '=')) { //&& pos < 14) {
        character = configFile.read();
      }
      character = configFile.read(); // leave out the '='
      while (configFile.available() && (character != ']') && pos < 14) {
        filename[pos] = character;
        pos++;
        //settingValue = settingValue + character;
        character = configFile.read();
      }

      //read the current # of measurement
      while (configFile.available() && (character != '=')) { //&& pos < 14) {
        character = configFile.read();
      }
      character = configFile.read(); // leave out the '='
      while ((configFile.available()) && (character != ']')) {
        settingValue = settingValue + character;
        character = configFile.read();
      }
      if (character == ']') {
        counter = toLong(settingValue);
      }
    }
    // close the file:
    configFile.close();
    uint8_t length;

    for (length = 14; length > 0; length--) {
      if (filename[length - 1] == ' ') {
        filename[length - 1] = 0;
      }
      else break;
    }
    return;
  }
  else {
    // if the file didn't open, print an error:
    write("Error read config");
    while (true);
    DEBUG_PRINTLN(F("Error opening configuration"));
    return;
  }
}

long toLong(String settingValue) {
  char longbuf[settingValue.length() + 1];
  settingValue.toCharArray(longbuf, sizeof(longbuf));
  long l = atol(longbuf);
  return l;
}

/*
 * writes the current logfilename to the config file
 * filename=[value.txt] \n counter=[n]
 */
void writeConfig(char* filename, boolean newCounter) {
  if (newCounter) counter = 1;
  // Delete the old One
  SD.remove(CONFIG_FILE);
  // Create new one
  File logFile = SD.open(CONFIG_FILE, FILE_WRITE);
  logFile.print("[");
  logFile.print("filename=");
  logFile.print(filename);
  logFile.println("]");
  logFile.print("[");
  logFile.print("counter=");
  logFile.print(counter);
  logFile.println("]");
  DEBUG_PRINT(F("filename: "));
  DEBUG_PRINTLN(filename);
  DEBUG_PRINT(F("counter: "));
  DEBUG_PRINTLN(counter);
  // close the file:
  logFile.close();
  //prepare new logfile
  createLogFile();
}

/*
 * Adds the file extension ".txt" to the given string
 */
void addFileExtension(char* extFilename) {
  uint8_t lastLetter = 0;
  for ( int i = 14; i > 0; i--) {
    if (extFilename[i - 1] == 0) {
      lastLetter = i - 1;
    } else break;
  }
  extFilename[lastLetter] = '.';
  extFilename[lastLetter + 1] = 't';
  extFilename[lastLetter + 2] = 'x';
  extFilename[lastLetter + 3] = 't';
}

/*
 *creates logfile with file header
 */
void createLogFile() {
  if (SD.exists(filename)) { //if file already exists, skip writing header
    return;
  }
  File logFile = SD.open(filename, FILE_WRITE);
  if (logFile) {
    logFile.println("#, lat, lng, date, time, tempBTS, tempAir");
    logFile.close();
  } else {
    write("Error creating logfile");
    //DEBUG_PRINT("Error creating logfile: ");
    //DEBUG_PRINTLN(filename);
  }
}
