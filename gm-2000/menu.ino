
/*
 * Aktualisiert den menuState und zeichnet den statischen Inhalt des Menüs neu.
 */
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
      write ("Config");
      overwrite(0, 1, filename);
      break;
  }
}


/*
 * prints the menu according to menuState
 * updates the displayed values only
 */
void updateMenuTime() {

  char date[11];
  char time[9];
  sprintf(date, "%02d.%02d.%4d",
          fix_data.dateTime.date, fix_data.dateTime.month, fix_data.dateTime.full_year(fix_data.dateTime.year));


  sprintf(time, "%02d:%02d:%02d",
          fix_data.dateTime.hours, fix_data.dateTime.minutes, fix_data.dateTime.seconds);
  
  clearline(5, 0);
  overwrite(5, 0, date);
  clearline(5, 1);
  overwrite(5, 1, time);
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
  lcd.print(fix_data.latitude(), 7);
  clearline(4, 1);
  lcd.setCursor(4, 1);
  lcd.print(fix_data.longitude(), 7);
}

void updateMenuPos2() {
  clearline(5, 0);
  lcd.setCursor(5, 0);
  lcd.print(fix_data.satellites);
  clearline(5, 1);
  lcd.setCursor(5, 1);
  lcd.print((int)fix_data.hdop * 0.001);
}

void updateMenuFileEdit() {
  uint8_t offset = pos / 16;
  write(&newFilename[offset * 16]);
  lcd.setCursor(pos % 16, 0);
  lcd.cursor();
}

