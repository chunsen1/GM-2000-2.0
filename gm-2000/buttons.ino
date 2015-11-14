unsigned long buttonPressTime = 0;



/*
 * returns the code of the pressed button
 * BUTTON_NONE if no button is pressed
 */
uint8_t getButton() {
  if (millis() - buttonPressTime >= BUTTON_PRESS_TIME) {
    buttonPressTime = millis(); // reset press time

    uint16_t keyValue = analogRead(PIN_BUTTONS);

    if (keyValue > 790) return BUTTON_NONE;
    if (keyValue < 50) {
      //return BUTTON_UP;
      return BUTTON_RIGHT;
    }
    if (keyValue < 195) {
      //return BUTTON_RIGHT;
      return BUTTON_UP;
    }
    if (keyValue < 380) {
      return BUTTON_DOWN;
    }
    if (keyValue < 555) {
      return BUTTON_LEFT;
    }
    if (keyValue < 790) {
      return BUTTON_SELECT;
    }

    return BUTTON_NONE;
  }
}
