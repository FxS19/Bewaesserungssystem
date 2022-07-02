void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  SoilSensor::setup();
  WaterLevelSensor::setup();
  Pumps::setup();
  RealTmeClock::setup();

  // Setup light sensor
  lightMeter.begin(BH1750::CONTINUOUS_LOW_RES_MODE);

  SPIFFS.begin();

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  logger.init();

  // Start WIFI
  WiFiManager::setup();

  // Start WebServer
  WebServer::setup();

  if (WiFi.status() == WL_CONNECTED) {
    // Update Time on RTC if network is available
    configTime(0, 0, "pool.ntp.org");
    unsigned long time = ntpClient.getUnixTime();
    RTC.setEpoch(time);
  } else {
    
  }

  postSetup();
}