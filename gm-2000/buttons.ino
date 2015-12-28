/*
 * returns the code of the pressed button
 * BUTTON_NONE if no button is pressed
 */
uint8_t getButton() {
  uint16_t keyValue = analogRead(PIN_BUTTONS);

  if (keyValue > 790) return BUTTON_NONE;
  if (keyValue < 50) {
    return BUTTON_UP;
    //DEBUG_PRINTLN(F("BUTTON_RIGHT"));
    //return BUTTON_RIGHT;
  }
  if (keyValue < 195) {
    return BUTTON_RIGHT;
    //DEBUG_PRINTLN(F("BUTTON_UP"));
    //return BUTTON_UP;
  }
  if (keyValue < 380) {
    //DEBUG_PRINTLN(F("BUTTON_DOWN"));
    return BUTTON_DOWN;
  }
  if (keyValue < 555) {
    //DEBUG_PRINTLN(F("BUTTON_LEFT"));
    return BUTTON_LEFT;
  }
  if (keyValue < 790) {
    //DEBUG_PRINTLN(F("BUTTON_SELECT"));
    return BUTTON_SELECT;
  }

  return BUTTON_NONE;
  //  }
}
