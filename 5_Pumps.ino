namespace Pumps {
  void setup() {
    pinMode(PUMP_MAIN_PIN, OUTPUT);
    pinMode(PUMP_SECONDARY_PIN, OUTPUT);  
  }

  void setMain(uint8_t percentage) {
    analogWrite(PUMP_MAIN_PIN, (uint8_t)(percentage * 2.5));
  }

  void setSecondary(uint8_t percentage) {
    analogWrite(PUMP_SECONDARY_PIN, (uint8_t)(percentage * 2.5));
  }
}