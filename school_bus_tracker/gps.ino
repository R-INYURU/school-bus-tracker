void refreshGPS() {
  smartDelay(100);

  if (millis() > 2000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
}

void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (GPS_Serial.available())
      gps.encode(GPS_Serial.read());
  } while (millis() - start < ms);
}

