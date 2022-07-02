namespace SoilSensor {
  void setup() {
    pinMode(SOIL_SENSOR_PIN, INPUT);
  }

  int16_t read() {
    uint8_t samples = 20;
    uint8_t delay_ms = 5;
    uint64_t accumulator = 0;
    for (uint16_t i = 0; i < samples; i++) {
        delay(delay_ms);
        accumulator += analogRead(SOIL_SENSOR_PIN);
    }
    // the sensor gives the dryness measure
    // substract it from the max value so that we get the moisture measure
    return 100 - 0.59 * ((accumulator / samples) - 315);
  }
}

