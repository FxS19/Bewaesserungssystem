void setup() {
  Serial.begin(921600);
  Serial.println();
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin(); // initialise the connection
  EEPROM.begin(EEPROM_SIZE);

  DS3231_Eeprom::setup();

  SoilSensor::setup();
  WaterLevelSensor::setup();
  Serial.println(F("Sensors ready"));
  Pumps::setup();
  Serial.println(F("Pumps ready"));
  RealTimeClock::setup();
  Serial.println(F("RTC ready"));

  // Setup light sensor
  lightMeter.begin(BH1750::CONTINUOUS_LOW_RES_MODE);

  // Start WIFI
  WiFiManager::setup();

  Log::setup();

  // Initialize SPIFFS
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  Serial.println(F("Start setting time online"));
  

  // Start WebServer
  Serial.println(F("WebServer setup"));
  WebServer::setup();

  // EnergyManagement vorbereiten
  Serial.println(F("EnergyManagement Setup"));
  EnergyManagement::setup();

  //Watermanagement vorbereiten
  Serial.println(F("WaterManagement setup"));
  WaterManagement::setup();

  postSetup();
  Serial.println(F("PostSetup done!"));
  Serial.println(F("starting loop..."));
  
}
