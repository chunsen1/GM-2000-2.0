#define PIN_CHIP_SELECT 4 //pin for accessing the sd
#define MAX_LENGTH_FILENAME 8
#define CONFIG_FILE "conf.txt" //config file to store the current logfile name


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
void storeMeasurement(struct data *currentData) {
  char filename[14] = "";
  readConfig(filename, true);
  createLogFile(); //writes headers if file does not exist
  File datafile = SD.open(filename, FILE_WRITE);
  // if the file is available, write to it:
  if (datafile) {
    datafile.print(currentData->lat, 10);
    datafile.print(",");
    datafile.print(currentData->lng, 10);
    datafile.print(",");
//    datafile.print(currentData->year);
//    datafile.print("-");
//    datafile.print(currentData->month);
//    datafile.print("-");
//    datafile.print(currentData->day);
//    datafile.print(",");
//    datafile.print(currentData->hoD);
//    datafile.print(":");
//    datafile.print(currentData->minD);
//    datafile.print(":");
//    datafile.print(currentData->secD);
//    datafile.print(",");
    datafile.print(currentData->date);
    datafile.print(",");
    datafile.print(currentData->time);
    datafile.print(",");
    datafile.print(currentData->tempBTS, 10);
    datafile.print(",");
    datafile.print(currentData->tempAir, 10);
    datafile.println("");

    // dataFile.println(position);
    datafile.close();
    //DEBUG_PRINTLN("Measurement done");
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
  delay(1000);
  updateCmdMenu(BUTTON_NONE);
}


/*
 * reads the logfilename from the config file to filename
 */
void readConfig(char* filename, bool extension) {
  uint8_t pos = 0;
  char character;
  File configFile = SD.open(CONFIG_FILE);
  if (configFile) {
    while (configFile.available()) {
      character = configFile.read();
      //while( configFile.available() && (character != '[')){
      //  character = configFile.read();
      //}
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
      //Debugging Printing
      // write("Current config", buffer);
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

    uint8_t lastLetter = 0;
    for ( int i = 14; i > 0; i--) {
      if (filename[i - 1] == 0) {
        lastLetter = i - 1;
      } else break;
    }
    if (extension) {
      filename[lastLetter] = '.';
      filename[lastLetter + 1] = 't';
      filename[lastLetter + 2] = 'x';
      filename[lastLetter + 3] = 't';
    }
    //DEBUG_PRINTLN(filename);
    return;
  }
  else {
    // if the file didn't open, print an error:
    write("Error read config");
    while (true);
    //DEBUG_PRINTLN("Error opening configuration");
    return;
  }
}

/*
 * writes the current logfilename to the config file
 * filename=[value.txt]
 */
void writeConfig(char* filename) {
  // Delete the old One
  SD.remove(CONFIG_FILE);
  // Create new one
  File logFile = SD.open(CONFIG_FILE, FILE_WRITE);
  logFile.print("[");
  logFile.print("filename=");
  logFile.print(filename);
  logFile.println("]");
  // close the file:
  logFile.close();
  //prepare new logfile
  createLogFile();
}

/*
 *creates logfile with file header
 */
void createLogFile() {
  char filename[14] = "";
  readConfig(filename, true);
  if (SD.exists(filename)) { //if file already exists, skip writing header
    return;
  }
  File logFile = SD.open(filename, FILE_WRITE);
  if (logFile) {
    logFile.println("lat, lng, date, time, tempBTS, tempAir");
    logFile.close();
  } else {
    write("Error creating logfile");
    //DEBUG_PRINT("Error creating logfile: ");
    //DEBUG_PRINTLN(filename);
  }
}
