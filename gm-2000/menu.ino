
void updateCmdMenu(int8_t button) {

  if (button == BUTTON_UP) {
    if (menuState > MENU_MIN) {
      menuState--;
      DEBUG_PRINTLN(F("button UP"));
    } else menuState = MENU_MAX;
  }
  else if (button == BUTTON_DOWN) {
    if (menuState < MENU_MAX) {
      menuState++;
      DEBUG_PRINTLN(F("button DOWN"));
    } else menuState = MENU_MIN;
  }

  switch (menuState) {
    case (MENU_TIME):
      write("date");
      overwrite(0, 1, "time");
      break;

    case (MENU_POSITION):
      write("lon");
      overwrite(0, 1, "lat");
      break;

    case (MENU_POS2):
      write("#sat");
      overwrite(0, 1, "hdop");
      break;

    case (MENU_TEMPERATURE):
      write("bts");
      overwrite(0, 1, "air");
      break;

    case (MENU_CONF):
      write (1, 0, "Config");
      break;
  }
}


/*
 * prints the menu according to menuState
 * updates the displayed values only
 */
void updateMenuTime() {
  clearline(5, 0);
  overwrite(5, 0, currentData.date);
  clearline(5, 1);
  overwrite(5, 1, currentData.time);
}

void updateMenuTemp() {
  clearline(4, 0);
  lcd.setCursor(4, 0);
  lcd.print(currentData.tempBTS, 5);
  clearline(4, 1);
  lcd.setCursor(4, 1);
  lcd.print(currentData.tempAir, 5);
}

void updateMenuPos() {
  clearline(4, 0);
  lcd.setCursor(4, 0);
  lcd.print(currentData.lat, 7);
  clearline(4, 1);
  lcd.setCursor(4, 1);
  lcd.print(currentData.lng, 7);
}

void updateMenuPos2() {
  clearline(5, 0);
  lcd.setCursor(5, 0);
  lcd.print(currentData.numberSatellites);
  clearline(5, 1);
  lcd.setCursor(5, 1);
  lcd.print(currentData.hdop);
}

