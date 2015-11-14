/*
 * activates the lcd module
 */
void activate_lcd() {
  // activate LCD module
  lcd.begin (16,2); // for 16 x 2 LCD module
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
}

void print_init() {
  lcd.home (); // set cursor to 0,0
  lcd.print("GM-2000");
  delay(1000);   
  lcd.clear(); 
}

void write(char* text) {
  write(0, 0, text);
}

void write(char* text1, char* text2) {
  write(0, 0, text1);
  overwrite(0, 1, text2);
}

void write(short column, uint8_t row,  char* text) {
  lcd.clear();
  overwrite(column, row, text);
}

void overwrite(short column, uint8_t row, char* text) {
  lcd.setCursor(column, row);
  lcd.print(text);
}

void clearline(short column, uint8_t row) {
  lcd.setCursor(column, row);
  lcd.print("                ");
}
