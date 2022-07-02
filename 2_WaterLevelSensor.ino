namespace WaterLevelSensor {
  enum value {
    empty = false,
    full = true
  };

  void setup() {
    pinMode(WATER_LEVEL_SENSOR_PIN, INPUT_PULLUP);
  }

  bool read() {
    return digitalRead(WATER_LEVEL_SENSOR_PIN);
  }
}

