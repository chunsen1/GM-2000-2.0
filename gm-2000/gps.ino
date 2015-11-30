/*
 * Initialisierung des GPS
 */
void initGPS() {
  gps_port.begin( 9600 );
}

/*
 * read the SoftwareSerial so that no bufferOverflow occurs
 */
bool feedGPS() {
  while (gps_port.available()) {

    //  Are we just getting garbage?
    static uint32_t chars_received = 0;
    chars_received++;
    if (chars_received > 1000) {
      chars_received = 0;
      DEBUG_PRINTLN(F("Invalid data received."));
    }

    if (gps.decode( gps_port.read() ) == NMEAGPS::DECODE_COMPLETED) {
      chars_received = 0;

      // If you have something quick to do, you can safely use gps.fix()
      //   members now.  For example, comparing the current speed
      //   against some limits and setting a flag would be ok.  Declare
      //   a global flag at the top of the file somewhere...
      //
      //     bool tooFast = false;
      //
      //   ...and set it right here, if a valid speed has been received:
      //
      //     if (gps.fix().valid.speed)
      //       tooFast = (gps.fix().speed() > 15.0); // nautical mph
      //
      // DO NOT do any printing or writing to an SD card *here*.
      //   Those operations can take a long time and may cause data loss.
      //   Instead, do those things in 'doSomeWork'.
      //
      // If you just need one piece of fix data, like the current second,
      //    you could copy one value like this:
      //
      //      if (gps.fix().valid.time)
      //        seconds = gps.fix().dateTime.seconds;
      //
      // If you need to use several pieces of the latest GPS data anywhere
      //   in your program, at any time, you can save a a complete copy of
      //   the entire GPS fix data collection, but you must do it *now*.

      if (gps.nmeaMessage == NMEAGPS::NMEA_RMC) {
        fix_data = gps.fix();
      }
    }
  }
}



