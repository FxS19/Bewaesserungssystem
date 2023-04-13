#define MAIN_PUMP_MAX_L_PER_MINUTE 5.0
#define SECONDARY_PUMP_MAX_L_PER_MINUTE 12.0

namespace Pumps {
  uint8_t mainPumpPercentage = 0;
  uint8_t secondaryPumpPercentage = 0;
  
  void setup() {
    pinMode(PUMP_MAIN_PIN, OUTPUT);
    pinMode(PUMP_SECONDARY_PIN, OUTPUT);  
  }

  void validatePercentage(uint8_t &percentage) {
    if (percentage > 100) percentage = 100; 
  }

  void setMain(uint8_t percentage) {
    validatePercentage(percentage);
    if(WaterLevelSensor::read()) {
      analogWrite(PUMP_MAIN_PIN, (uint8_t)(percentage * 2.5));
      mainPumpPercentage = percentage;
    } else {
      analogWrite(PUMP_MAIN_PIN, 0);
      mainPumpPercentage = 0;
    }
  }

  void setSecondary(uint8_t percentage) {
    validatePercentage(percentage);
    analogWrite(PUMP_SECONDARY_PIN, (uint8_t)(percentage * 2.5));
    mainPumpPercentage = percentage;
  }
}
