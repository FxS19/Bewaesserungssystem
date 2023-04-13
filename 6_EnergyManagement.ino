#define ENERGY_MANAGEMENT_DEBUG
namespace EnergyManagement {
  time_t wifi_last_changed;
  bool wifiEnabled = true;
  int lastWifiSystemStatus;

  void update() {
    time_t t = time(nullptr);
    #ifdef ENERGY_MANAGEMENT_DEBUG
    //Serial.println("AP_Stations " + String(WiFi.softAPgetStationNum()));
    //Serial.println("Wifi Status " + String(WiFi.status()));
    //Serial.println("Wifi internal " + String(wifiEnabled));
    #endif
    if ((WiFi.status() != WL_CONNECTED && WiFi.softAPgetStationNum() == 0 ) && t - wifi_last_changed > 30 && wifiEnabled) {
      Serial.println(F("EnergyManager: WIFI Sleep Mode"));
      RealTimeClock::offlineMode();
      WiFiManager::disableWifi();
      wifiEnabled = false;
      wifi_last_changed = t;
    }

    if (!wifiEnabled && t - wifi_last_changed > 300) {
      Serial.println(F("EnergyManager: WIFI Activate"));
      WiFiManager::start();
      wifi_last_changed = t;
      wifiEnabled = true;
    }

    if (wifiEnabled) digitalWrite(LED_BUILTIN, LOW);

    if (lastWifiSystemStatus != WiFi.status() && WiFi.status() == WL_CONNECTED) {
      RealTimeClock::onlineMode();
    }
  }

  void activateAutoSleepMode(){
    WiFi.setSleepMode(WIFI_LIGHT_SLEEP, 3);  // Automatic Light Sleep, DTIM listen interval = 3
  }

  void deactivateSleepMode(){
    WiFi.setSleepMode(WIFI_NONE_SLEEP);  // do not sleep
  }

  void setup() {
    wifi_last_changed = time(nullptr);
    lastWifiSystemStatus = WiFi.status();
    activateAutoSleepMode();
  }
}
