
// arrays to hold device addresses
//Hendriks Sensoren
DeviceAddress addressTempBTS = {0x28, 0xFF, 0x19, 0x79, 0x62, 0x14, 0x03, 0x23};
// DeviceAddress addressTempBTS = {0x28, 0xFF, 0xDC, 0xFF, 0x03, 0x15, 0x03, 0xC9};
// DeviceAddress addressTempAir = {0x28, 0xFF, 0x68, 0x5B, 0x63, 0x15, 0x03, 0x9B};

void initTemperature() {
  // set the resolution of Sensor
  //sensors.setResolution(addressTempBTS, TEMPERATURE_PRECISION);
  //  sensors.setResolution(addressTempAir, TEMPERATURE_PRECISION);
}

void getTemperature(struct temperature *temperature) {
  // Setup a oneWire instance to communicate with any OneWire devices
  OneWire oneWire(ONE_WIRE_BUS);
  // Pass our oneWire reference to Dallas Temperature.
  DallasTemperature sensors(&oneWire);

  sensors.requestTemperatures();
  temperature->tempBTS = sensors.getTempCByIndex(0);
  //temperature->tempBTS = 35;

  //temperature->tempBTS = sensors.getTempC(addressTempBTS);
  //DEBUG_PRINTDEC(temperature->tempBTS);

  // Initialize DHT sensor for normal 16mhz Arduino
  DHT dht(DHTPIN, DHTTYPE);
  temperature->tempAir = dht.readTemperature();
  //temperature->tempAir = 34;//dht.readTemperature();

  //temperature->tempAir = sensors.getTempC(addressTempAir);
  //DEBUG_PRINTDEC(temperature->tempAir);
}
