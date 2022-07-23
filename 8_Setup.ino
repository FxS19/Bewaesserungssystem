void setup() {
  Serial.begin(115200);
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

  SPIFFS.begin();

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
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
