namespace WebServer {
  // Create AsyncWebServer object on port 80
  AsyncWebServer server(80);

  const char* PARAM_VALUE = "value";

  void setup() {
    server.serveStatic("/", LittleFS , "/").setDefaultFile("index.html");

    server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "text/plain", String(ESP.getFreeHeap()));
    });

    // Send a GET request to <IP>/water?value=<number>
    server.on("/water", HTTP_GET, [] (AsyncWebServerRequest *request) {
      if (request->hasParam(PARAM_VALUE)) {
        int value = request->getParam(PARAM_VALUE)->value().toInt();
        request->send(200, "text/plain", "OK");
        WaterManagement::water(value);
      } else {
        request->send(500, F("text/plain"), F("Request invalid"));
      }
    });


    //*************************
    //*****WATERING_POINTS*****
    //*************************      
    server.on("/wateringPoints", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam(PARAM_VALUE)) {
        int value = request->getParam(PARAM_VALUE)->value().toInt();
        EEPROM.write(WATERING_POINTS_ADRESS, (uint8_t)value);
        if (!EEPROM.commit()) {
          request->send(500, F("text/plain"), F("ERROR! EEPROM commit failed"));
        } else {
          request->send(200, F("text/plain"), "OK");
        }
      } else {
        request->send(200, F("text/plain"), String((uint8_t)EEPROM.read(WATERING_POINTS_ADRESS)));
      }
    });

    //************************************
    //*****BASE_ML_PER_WATERING_POINT*****
    //************************************    
    server.on("/baseMlPerWateringPoint", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam(PARAM_VALUE)) {
        uint16_t value = request->getParam(PARAM_VALUE)->value().toInt();
        Serial.println("Recieved Value: ");
        Serial.println(value);
        byte* bytes = (byte*)&value;
        EEPROM.write(BASE_ML_PER_WATERING_POINT_ADRESS    , bytes[0]);
        EEPROM.write(BASE_ML_PER_WATERING_POINT_ADRESS + 1, bytes[1]);
        if (!EEPROM.commit()) {
          request->send(500, F("text/plain"), F("ERROR! EEPROM commit failed"));
        } else {
          request->send(200, F("text/plain"), "OK");
        }
      } else {
        byte bytes[2] {
          EEPROM.read(BASE_ML_PER_WATERING_POINT_ADRESS), 
          EEPROM.read(BASE_ML_PER_WATERING_POINT_ADRESS + 1)
        };
        request->send(200, F("text/plain"), String(*(uint16_t*)bytes));
      }
    });

    //***********************
    //*****SOIL_MOISTURE*****
    //***********************
    server.on("/okSoilMoisture", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam(PARAM_VALUE)) {
        int value = request->getParam(PARAM_VALUE)->value().toInt();
        EEPROM.write(OK_SOIL_MOISTURE_ADRESS, (uint8_t)value);
        if (!EEPROM.commit()) {
          request->send(500, F("text/plain"), F("ERROR! EEPROM commit failed"));
        } else {
          request->send(200, F("text/plain"), "OK");
        }
      } else {
        request->send(200, F("text/plain"), String((uint8_t)EEPROM.read(OK_SOIL_MOISTURE_ADRESS)));
      }
    });

    //*************************
    //*****MAIN_PUMP_SPEED*****
    //*************************  
    server.on("/mainPumpSpeed", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam(PARAM_VALUE)) {
        int value = request->getParam(PARAM_VALUE)->value().toInt();
        EEPROM.write(MAIN_PUMP_SPEED_ADRESS, (uint8_t)value);
        if (!EEPROM.commit()) {
          request->send(500, F("text/plain"), F("ERROR! EEPROM commit failed"));
        } else {
          request->send(200, F("text/plain"), "OK");
        }
      } else {
        request->send(200, F("text/plain"), String((uint8_t)EEPROM.read(MAIN_PUMP_SPEED_ADRESS)));
      }
    });

    server.on("/log", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam(PARAM_VALUE)) {
        int value = request->getParam(PARAM_VALUE)->value().toInt();
        struct Log::LogData data = Log::read(value);

        request->send(200, F("text/plain"), 
        "{\"time\":"+ String(data.time) + 
        ", \"soil\": " + String(data.soil) + 
        ", \"lux\":" + String(data.lux) + 
        ", \"watered\":" + String((bool)(data.flags & 0b00000001)) +
        ", \"waterlevel\":" + String((bool)((data.flags >> 1) & 0b00000001)) +
        "}");
      } else {
        request->send(500, F("text/plain"), F("Invalid request"));
      }
      
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
      request->send(200, F("text/plain"), String(RTC.getTemperature()));
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

    //*************************
    //*****clock
    //*************************
    server.on("/clock", HTTP_GET, [] (AsyncWebServerRequest *request) {
      String valueString = request->pathArg(0);
      request->send(200, F("text/plain"), String(RTC.now().unixtime()));
    });
    
    server.begin();
  }
}
