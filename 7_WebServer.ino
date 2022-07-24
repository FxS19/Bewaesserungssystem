namespace WebServer {
  // Create AsyncWebServer object on port 80
  AsyncWebServer server(80);

  void setup() {
    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

    server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "text/plain", String(ESP.getFreeHeap()));
    });

    // Send a GET request to <IP>/water/<number>
    server.on("^\\/water\\/([0-9]+)$", HTTP_GET, [] (AsyncWebServerRequest *request) {
      String literString = request->pathArg(0);
      WaterManagement::water(literString.toInt());
      request->send(200, "text/plain", "");
    });

    //****************************
    //*****SETTINGS ENDOPINTS*****
    //****************************
    {
      //*************************
      //*****WATERING_POINTS*****
      //*************************
      server.on("^\\/wateringPoints\\/([0-9]+)$", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String literString = request->pathArg(0);
        EEPROM.write(WATERING_POINTS_ADRESS, (uint8_t)literString.toInt());
        if (!EEPROM.commit()) {
          request->send(500, F("text/plain"), F("ERROR! EEPROM commit failed"));
        } else {
          request->send(200, F("text/plain"), "OK");
        }
      });
      
      server.on("/wateringPoints", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, F("text/plain"), String((uint8_t)EEPROM.read(WATERING_POINTS_ADRESS)));
      });
  
      //************************************
      //*****BASE_ML_PER_WATERING_POINT*****
      //************************************
      server.on("^\\/baseMlPerWateringPoint\\/([0-9]+)$", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String valueString = request->pathArg(0);
        EEPROM.write(BASE_ML_PER_WATERING_POINT_ADRESS, (uint8_t)valueString.toInt());
        if (!EEPROM.commit()) {
          request->send(500, F("text/plain"), F("ERROR! EEPROM commit failed"));
        } else {
          request->send(200, F("text/plain"), "OK");
        }
      });
      
      server.on("/baseMlPerWateringPoint", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, F("text/plain"), String((uint8_t)EEPROM.read(BASE_ML_PER_WATERING_POINT_ADRESS)));
      });
  
      //***********************
      //*****SOIL_MOISTURE*****
      //***********************
      server.on("^\\/okSoilMoisture\\/([0-9]+)$", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String valueString = request->pathArg(0);
        EEPROM.write(OK_SOIL_MOISTURE_ADRESS, (uint8_t)valueString.toInt());
        if (!EEPROM.commit()) {
          request->send(500, F("text/plain"), F("ERROR! EEPROM commit failed"));
        } else {
          request->send(200, F("text/plain"), "OK");
        }
      });
      
      server.on("/okSoilMoisture", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, F("text/plain"), String((uint8_t)EEPROM.read(OK_SOIL_MOISTURE_ADRESS)));
      });
  
      //*************************
      //*****MAIN_PUMP_SPEED*****
      //*************************
      server.on("^\\/mainPumpSpeed\\/([0-9]+)$", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String valueString = request->pathArg(0);
        EEPROM.write(MAIN_PUMP_SPEED_ADRESS, (uint8_t)valueString.toInt());
        if (!EEPROM.commit()) {
          request->send(500, F("text/plain"), F("ERROR! EEPROM commit failed"));
        } else {
          request->send(200, F("text/plain"), "OK");
        }
      });
      
      server.on("/mainPumpSpeed", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, F("text/plain"), String((uint8_t)EEPROM.read(MAIN_PUMP_SPEED_ADRESS)));
      });
      
      server.on("/mainPumpSpeed", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, F("text/plain"), String((uint8_t)EEPROM.read(MAIN_PUMP_SPEED_ADRESS)));
      }); 

      server.on("^\\/log\\/([0-9]+)$", HTTP_GET, [](AsyncWebServerRequest *request){
        String valueString = request->pathArg(0);
        struct Log::LogData data = Log::read(valueString.toInt());
        request->send(200, F("text/plain"), 
        "{\"time\":"+ String(data.time) + 
        ", \"soil\": " + String(data.soil) + 
        ", \"lux\":" + String(data.lux) + 
        ", \"watered\":" + String((bool)(data.flags & 0b00000001)) +
        ", \"waterlevel\":" + String((bool)((data.flags >> 1) & 0b00000001)) +
        "}");
        
      }); 

      //*************************
      //*****Wasser für heute
      //*************************
      server.on("/autoWaterToday", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String valueString = request->pathArg(0);
        request->send(200, F("text/plain"), String(waterForToday));
      });

      //*************************
      //*****temperature
      //*************************
      server.on("/temperature", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String valueString = request->pathArg(0);
        request->send(200, F("text/plain"), String(RTC.getTemp()));
      });

      //*************************
      //*****Bodenfeuchtigkeit
      //*************************
      server.on("/soilMoisture", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String valueString = request->pathArg(0);
        request->send(200, F("text/plain"), String(SoilSensor::read()));
      });

      //*************************
      //*****Wasser status
      //*************************
      server.on("/waterStatus", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String valueString = request->pathArg(0);
        request->send(200, F("text/plain"), String(WaterLevelSensor::read()));
      });

      //*************************
      //*****lux
      //*************************
      server.on("/lux", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String valueString = request->pathArg(0);
        request->send(200, F("text/plain"), String(lightMeter.readLightLevel()));
      });
    }
    
    server.begin();
  }
}
