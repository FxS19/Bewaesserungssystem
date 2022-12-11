//#define WATER_MANAGEMENT_DEBUG

namespace WaterManagement {
  float literToWater = 0; // Setzen, um Bewässerung zu starten
  time_t literUpdatedTime;
  time_t pumpStateUpdatedTime;
  uint8_t pumpSpeed = 0;
  enum class pumpState {OFF, STARTED, ON};
  pumpState currentPumpeState = pumpState::OFF;

  void setup() {
    pumpStateUpdatedTime = time(nullptr);
    literUpdatedTime = time(nullptr);
  }

  void water(float liter) {
    if (liter == 0) {
      Pumps::setMain(0);
    }
    literUpdatedTime = time(nullptr);
    literToWater = liter;
    pumpStateUpdatedTime = time(nullptr);
    pumpSpeed = (uint8_t)EEPROM.read(MAIN_PUMP_SPEED_ADRESS);
  }

  void update() {
    time_t currentTime = time(nullptr);
    
    if (Pumps::mainPumpPercentage > 0) {
      literToWater -=  ((currentTime - literUpdatedTime)/60.0) * MAIN_PUMP_MAX_L_PER_MINUTE * (Pumps::mainPumpPercentage/100.0);
      Serial.print(F("WaterManagement:"));
      Serial.print(literToWater);
      Serial.println(F("l"));
    }
    literUpdatedTime = currentTime;

    switch (currentPumpeState) {
      case pumpState::OFF: {
        Serial.println(F("WaterManagement:OFF"));
        if (literToWater > 0) {
            EnergyManagement::deactivateSleepMode();
            Pumps::setMain(100);
            currentPumpeState = pumpState::STARTED; 
            pumpStateUpdatedTime = currentTime;
        }
        break;
      }
      case pumpState::STARTED: {
        Serial.println(F("WaterManagement:\tSTARTED"));
        if (currentTime - pumpStateUpdatedTime <= 5) { //5 seconds full throttle to bleed air and build up preassure in the hose system
          Pumps::setMain(100);
        } else {
          Pumps::setMain(pumpSpeed);
          currentPumpeState = pumpState::ON;
          pumpStateUpdatedTime = currentTime;
        }
        break;
      }
      case pumpState::ON: {
        Serial.print(F("WaterManagement:\tON"));
        if (Pumps::mainPumpPercentage == 0) {
          Serial.print(F(" - Cooling\t"));
        }
        Serial.print(currentTime - pumpStateUpdatedTime);
        Serial.print("s");
        int motorControllerCooldown = (100 - pumpSpeed) * 5; // the controller heats up more when the pump is running solwer
        if (currentTime - pumpStateUpdatedTime >= 60 + motorControllerCooldown 
            && Pumps::mainPumpPercentage > 0) { // Wait at least 60 seconds
          Pumps::setMain(0); // Let pump and controller cool down
          pumpStateUpdatedTime = currentTime;
        } else if (currentTime - pumpStateUpdatedTime >= 60 + motorControllerCooldown 
                  && Pumps::mainPumpPercentage == 0) {// start next watering period
          currentPumpeState = pumpState::STARTED;
          pumpStateUpdatedTime = currentTime;
        }
        Serial.println();
        break;
      }
      default: currentPumpeState = pumpState::OFF;
    }

    
    if (Pumps::mainPumpPercentage > 0 && literToWater < 0) { // Bewässerung zuende (kann immer eintreten)
      Serial.println(F("WaterManagement:\tFinished"));
      Pumps::setMain(0);      
      currentPumpeState = pumpState::OFF;
      literToWater = 0;
      EnergyManagement::activateAutoSleepMode();
    }
  }

  float literNeededToday(uint8_t soilMoisture, uint16_t sunnySeconds){
    if (soilMoisture >= (uint8_t)EEPROM.read(OK_SOIL_MOISTURE_ADRESS)) return 0; // Boden ist noch feucht genug
    
    float sunnyHours = sunnySeconds / 3600.0f;
    if (sunnyHours > 7) {
      sunnyHours = 10.0f;
    }
    float sunCompensation = 0.5f + sunnyHours / 20.0f;
    float soilHumidityCompensation = 1.0f - ( soilMoisture / 200.0f );
    
    uint mlPerEndpoint = 0;
    {
      byte bytes[2] {
        EEPROM.read(BASE_ML_PER_WATERING_POINT_ADRESS), 
        EEPROM.read(BASE_ML_PER_WATERING_POINT_ADRESS + 1)
      };
      mlPerEndpoint = *(uint*)bytes;
    }

    uint8_t wateringPoints = (uint8_t)EEPROM.read(WATERING_POINTS_ADRESS);
    
    #ifdef WATER_MANAGEMENT_DEBUG
    Serial.print(F("WaterManagement:\t (sunCompensation)"));
    Serial.print(sunCompensation);
    Serial.print(F(" * (soilHumidityCompensation)"));
    Serial.print(soilHumidityCompensation);
    Serial.print(F(" * (mlPerEndpoint)"));
    Serial.print((uint)mlPerEndpoint);
    Serial.print(F(" * (wateringPoints)"));
    Serial.print(wateringPoints);
    Serial.print(F(" = "));
    Serial.print((sunCompensation * soilHumidityCompensation * (mlPerEndpoint / 1000.0f) * wateringPoints));
    Serial.println(" liter");
    #endif
    return (sunCompensation * soilHumidityCompensation * (mlPerEndpoint / 1000.0f) * wateringPoints);
  }
}
