namespace SoilSensor {
  void setup() {
    pinMode(SOIL_SENSOR_PIN, INPUT);
  }

  int16_t read() {
    uint8_t samples = 20;
    uint64_t accumulator = 0;
    for (uint16_t i = 0; i < samples; i++) {
        accumulator += analogRead(SOIL_SENSOR_PIN);
    }
    // the sensor gives the dryness measure
    // substract it from the max value so that we get the moisture measure
    int16_t ret = 100 - 0.59 * ((accumulator / samples) - 315);
    if (ret > 100) return 101;
    if (ret < 0) return 0;
    return ret;
  }
}
