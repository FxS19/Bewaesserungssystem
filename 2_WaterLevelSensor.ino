namespace WaterLevelSensor {
  enum value {
    empty = false,
    full = true
  };

  void setup() {
    pinMode(WATER_LEVEL_SENSOR_PIN, INPUT);
  }

  bool read() {
    pinMode(WATER_LEVEL_SENSOR_PIN, INPUT_PULLUP);
    bool data = !digitalRead(WATER_LEVEL_SENSOR_PIN);
    pinMode(WATER_LEVEL_SENSOR_PIN, INPUT);
    return data;
  }
}
