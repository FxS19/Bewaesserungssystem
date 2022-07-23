
void postSetup() {
  time_t t = time(nullptr);

  time_t last_backup = (time_t) DS3231_Eeprom::read_time_t(DS3231_EEPROM_LAST_BACKUP_ADDRESS);
  if (last_backup > t + 30) {
    last_backup = 0;
  }
  struct tm lastBackupTime = *localtime(&last_backup);

  lastExecutionTime = last_backup;
  lastExecutionLocalTime = lastBackupTime;

  if (lastExecutionLocalTime.tm_yday < lastBackupTime.tm_yday) {
    DS3231_Eeprom::write(DS3231_EEPROM_SUNNY_SECONDS_ADDRESS, 0);
    DS3231_Eeprom::write(DS3231_EEPROM_HAS_WATERED_ADDRESS, 0b00000000);
  }
  //Werte zurückspeichern
  sunnySecondsToday = (uint16_t)DS3231_Eeprom::read_int(DS3231_EEPROM_SUNNY_SECONDS_ADDRESS);
  hasWatered = (bool)DS3231_Eeprom::read_byte(DS3231_EEPROM_HAS_WATERED_ADDRESS);
}

#define PTM(w) \
  Serial.print(#w "="); \
  Serial.print(tm->tm_##w); \
  Serial.print(" ");

void printTm(const char* what, const tm* tm) {
  Serial.print(what);
  PTM(isdst); PTM(yday); PTM(wday);
  PTM(year);  PTM(mon);  PTM(mday);
  PTM(hour);  PTM(min);  PTM(sec);
  Serial.println();
}

void loop() {
  Serial.write(27);       // ESC command
  Serial.print("[2J");    // clear screen command
  Serial.write(27);
  Serial.print("[H");     // cursor to home command
  RealTimeClock::update();
  // get current time
  const time_t timeNow = time(nullptr);
  const struct tm timeNowLocal = *localtime(&timeNow);
  
  //thisTime.tm_year += 1900; // bugfix

  // get current light value
  float lux = lightMeter.readLightLevel();
  if (lux < 0) { // fehler, versuche den Sensor neu zu Initialisieren
    lightMeter.begin(BH1750::CONTINUOUS_LOW_RES_MODE);
  }

  // get WaterLevel
  bool waterLevel = WaterLevelSensor::read();

  // get soilMoisture
  uint8_t soilMoisture = SoilSensor::read();

  float temperature = RTC.getTemp();

  if (!waterLevel) Pumps::setMain(0); //Notabschaltung

  if (lux > 15000) sunnySecondsToday += timeNow - lastExecutionTime; // relative bright light

  if (timeNowLocal.tm_mday != lastExecutionLocalTime.tm_mday) { // Day has changed
    sunnySecondsToday = 0;
    hasWatered = false;
  }

  printTm("Uhrzeit:\t", &timeNowLocal);
  Serial.print(F("\t\t"));
  Serial.println(timeNow);

  Serial.print(F("LightMeter:\t"));
  Serial.print(lux);
  Serial.println("lux");
  
  Serial.print(F("Sunny seconds:\t"));
  Serial.println(sunnySecondsToday);
  
  Serial.print(F("Water Level:\t"));
  Serial.println(waterLevel ? "OK" : "LOW");

  Serial.print(F("Soil Moisture:\t"));
  Serial.print(soilMoisture);
  Serial.println(F("%"));

  // Starte Gießen um 20 Uhr
  if (timeNowLocal.tm_hour > 20 && !hasWatered) {
    WaterManagement::water(WaterManagement::literNeededToday(soilMoisture, sunnySecondsToday));
    hasWatered = true;
  }

  WaterManagement::update();

  EnergyManagement::update();

  {
    Serial.println("\nLogs:");
    for (int i = 1; i < 5; i++) {
      struct Log::LogData ld = Log::read(i);
      Serial.print(F("time: "));
      Serial.print(ld.time);
      Serial.print(" Soil: ");
      Serial.print(ld.soil);
      Serial.print("% Light: ");
      Serial.print(ld.lux);
      Serial.println("lux");
    }
  }

  if (Log::needsToLog()) {
    struct Log::LogData data = {
      timeNow, //time
      soilMoisture, //soil
      lux, //lux
      hasWatered | waterLevel << 1  //flags
    };
    Serial.println(F("Log Data"));    
    Log::write(data);
  }

  digitalWrite(LED_BUILTIN, HIGH);

  if (timeNow - lastBackupTime > 300) { // Backup alle 5 Minuten
    DS3231_Eeprom::write(DS3231_EEPROM_SUNNY_SECONDS_ADDRESS, (int)sunnySecondsToday);
    DS3231_Eeprom::write(DS3231_EEPROM_HAS_WATERED_ADDRESS, (byte)hasWatered);
    DS3231_Eeprom::write(DS3231_EEPROM_LAST_BACKUP_ADDRESS, timeNow);
    lastBackupTime = timeNow;
    Serial.println(F("Backup done"));
    waterForToday = WaterManagement::literNeededToday(soilMoisture, sunnySecondsToday);
  }
  
  lastExecutionLocalTime = timeNowLocal;
  lastExecutionTime = timeNow;
  delay(1000);//ersetzen durch echtes schlafen (wifi on/off, jenachdem)
}
