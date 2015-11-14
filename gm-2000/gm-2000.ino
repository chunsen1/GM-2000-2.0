#include <LCD.h>
//#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include <SdFat.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>



#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINT(x)     Serial.print (x)
#define DEBUG_PRINTDEC(x)     Serial.print (x, DEC)
#define DEBUG_PRINTLN(x)  Serial.println (x)
#define LCD_PRINT(x) debug_lcd (x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTDEC(x)
#define DEBUG_PRINTLN(x)
#endif

#define BUTTON_NONE   0
#define BUTTON_UP    1
#define BUTTON_DOWN   2
#define BUTTON_RIGHT  3
#define BUTTON_LEFT   4

#define BUTTON_SELECT 2 //RIGHT == SELECT

#define START_MENU_STATE 1
#define MENU_MIN 1
#define MENU_MAX 5

#define MENU_TIME 1
#define MENU_POSITION 2
#define MENU_POS2 3
#define MENU_TEMPERATURE 4
#define MENU_CONF 5
#define MENU_FILE_EDIT 50

#define MAX_LENGTH_FILENAME 9

#define UPDATE_INTERVAL 1000

//device temperature sensor settings
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

//temperature settings
#define ONE_WIRE_BUS 17

#define PIN_BUTTONS 0 //buttons are wired to PIN 0
#define BUTTON_PRESS_TIME 100

//gps settings
//this pin will output the DTR signal
#define GPS_RXPIN 15 //tx on gps
#define GPS_TXPIN 16


//----------------------------------------------------------------------------------------------
// Variablen
//----------------------------------------------------------------------------------------------

uint8_t cmdMenu = BUTTON_NONE;
uint8_t cmdAction = BUTTON_NONE;
uint8_t menuState = START_MENU_STATE;
boolean isDirtyTime = false;
boolean isDirtyPos = false;
boolean isDirtyPos2 = false;
boolean isDirtyTemp = false;

long previousMillisGPSTime = 0; //used in main loop for delay
long previousMillisGPSPos = 0; //used in main loop for delay
long previousMillisTemp = 0; //used in main loop for delay

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

SdFat SD;
uint8_t filePos = 0;
char filename[16] = "";

TinyGPSPlus gps;
SoftwareSerial softwareSerial(GPS_RXPIN, GPS_TXPIN);

// arrays to hold device addresses
DeviceAddress outsideThermometer;

struct data {
  char date[11];
  char time[9];
  int year;
  byte month;
  byte day;
  byte hoD;
  byte minD;
  byte secD;
  float lat;
  float lng;
  byte numberSatellites;
  long hdop;
  float tempBTS;
  float tempAir;
};

data currentData;



//----------------------------------------------------------------------------------------------
// setup
//----------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);

  activate_lcd();
  print_init();

  initSDCard();
  initGPS();

  updateCmdMenu(BUTTON_NONE);
}

//----------------------------------------------------------------------------------------------
// loop
//----------------------------------------------------------------------------------------------
void loop() {
  unsigned long currentMillis = millis();

  feedGPS();



  // finde heraus, was beim nächsten Durchlauf geschehen soll
  if (cmdAction == 0 && cmdMenu == 0 && menuState != MENU_FILE_EDIT) {
    int8_t button = getButton();

    //    DEBUG_PRINT(F("button: "));
    //    DEBUG_PRINTLN(button);

    //Menü hoch und runter durchscrollen
    if (button == BUTTON_UP || button == BUTTON_DOWN) {
      cmdMenu = button;

      //je nach menuState eine Aktion ausführen
    } else if (button == BUTTON_LEFT || button == BUTTON_RIGHT) {
      cmdAction = button;
    }
    //return;
  }



  //Zustand bestimmen
  if (cmdMenu > 0 && menuState != MENU_FILE_EDIT) {
    updateCmdMenu(cmdMenu);
    cmdMenu = 0;
    cmdAction = 0;
    isDirtyTime = false;
    isDirtyPos = false;
    isDirtyTemp = false;
    return;
  }

  //Aktion
  if (menuState < MENU_CONF && cmdAction > 0) {
    //Messwert auf SD schreiben
    DEBUG_PRINTLN(F("Messwert auf SD speichern"));
    //Messwerte der anderen Bereiche vor dem Speichern nochmal aktualisieren
    if (menuState == MENU_TEMPERATURE) {
      getGPSPosition(&currentData);
      getGPSTime(&currentData);
    } else if (menuState < MENU_CONF) {
      getTemperature(&currentData);
    }
    storeMeasurement(&currentData);
    cmdAction = 0;
    return;
  }

  //Zustände abarbeiten
  if (menuState == MENU_TEMPERATURE) {
    if (isDirtyTemp) {
      //Display aktualisieren
      updateMenuTemp();
      DEBUG_PRINTLN(F("Display Temp"));
      isDirtyTemp = false;
    } else {
      // Daten holen
      if (currentMillis - previousMillisTemp > UPDATE_INTERVAL) {
        DEBUG_PRINTLN(F("Update Temp"));
        previousMillisTemp = currentMillis;
        getTemperature(&currentData);
        isDirtyTemp = true;
      }
    }
  }
  if (menuState == MENU_TIME) {
    if (isDirtyTime) {
      //Display aktualisieren
      DEBUG_PRINTLN(F("Display TIME"));
      updateMenuTime();
      isDirtyTime = false;
    } else {
      // Daten holen

      if (currentMillis - previousMillisGPSTime > UPDATE_INTERVAL && gps.time.isValid() && gps.time.isUpdated() && gps.date.isValid() && gps.date.isUpdated()) {
        DEBUG_PRINTLN(F("Update TIME"));
        previousMillisGPSTime = currentMillis;
        getGPSTime(&currentData);
        isDirtyTime = true;
      }
    }
  }
  if (menuState == MENU_POSITION) {
    if (isDirtyPos) {
      //Display aktualisieren
      DEBUG_PRINTLN(F("Display Pos"));
      updateMenuPos();
      isDirtyPos = false;
    } else {
      // Daten holen

      if (currentMillis - previousMillisGPSPos > UPDATE_INTERVAL && gps.location.isValid() && gps.location.isUpdated()) {
        DEBUG_PRINTLN(F("Update Pos"));
        previousMillisGPSPos = currentMillis;
        getGPSPosition(&currentData);
        isDirtyPos = true;
      }
    }
  }

  if (menuState == MENU_POS2) {
    if (isDirtyPos2) {
      //Display aktualisieren
      DEBUG_PRINTLN(F("Display Pos2"));
      updateMenuPos2();
      isDirtyPos2 = false;
    } else {
      if (currentMillis - previousMillisGPSPos > UPDATE_INTERVAL && gps.location.isValid() && gps.location.isUpdated()) {
        DEBUG_PRINTLN(F("Update Pos2"));
        previousMillisGPSPos = currentMillis;
        getGPSPosition(&currentData);
        isDirtyPos2 = true;


      }
    }
  }

  //Dateiname
  if (menuState == MENU_FILE_EDIT) {
    //Buchstabeneingabe

    return;
  }

}
