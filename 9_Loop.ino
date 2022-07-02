uint16_t sunnySecondsToday = 0;
bool hasWatered = false;
struct tm lastExecutionTime;

void postSetup() {
  time_t t = RTC.getEpoch();
  lastExecutionTime = *localtime(&t);
}

void loop() {
  // get current time
  time_t t = RTC.getEpoch();
  struct tm thisTime;
  thisTime.tm_year += 1900; // bugfix
  thisTime = *localtime(&t);

  // get current light value
  float lux = lightMeter.readLightLevel();

  // get WaterLevel
  bool waterLevel = WaterLevelSensor::read();

  // get soilMoisture
  uint16_t soilMoisture = SoilSensor::read();

  if (thisTime.tm_mday != lastExecutionTime.tm_mday) { // Day has changed
    sunnySecondsToday = 0;
    hasWatered = false;
  }

  if (lux > 20000) sunnySecondsToday++; // relative bright light

  Serial.print(thisTime.tm_mday);
  Serial.print(".");
  Serial.print(thisTime.tm_mon);
  Serial.print(".");
  Serial.print(thisTime.tm_year);
  Serial.print(" ");
  Serial.print(thisTime.tm_hour);
  Serial.print(":");
  Serial.print(thisTime.tm_min);
  Serial.print(":");
  Serial.println(thisTime.tm_sec);
  
  
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");


  Serial.print("Soil: ");
  Serial.print(soilMoisture);
  Serial.println("%");

  Serial.print("Waterlevel: ");
  Serial.println(waterLevel);



  lastExecutionTime = thisTime;
  delay(1000);
}