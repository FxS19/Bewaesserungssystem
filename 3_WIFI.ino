namespace WiFiManager {
  enum class WifiMode {OFF, AP, STA};
  WifiMode currentWifiMode = WifiMode::OFF;
  
  
  void start() {
    switch (currentWifiMode) {
      case WifiMode::OFF: {
        break;
      }
      case WifiMode::AP: {
        Serial.print("Open hotsopt: Giessanlage");
        Serial.println(ssid);
        WiFi.mode(WIFI_AP);
        WiFi.softAP("Giessanlage", "");
        break;
      }
      case WifiMode::STA: {
        Serial.print("Connecting to ");
        Serial.println(ssid);
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, pass);
        break;
      }
    }
  }

  void setup() {
    currentWifiMode = WifiMode::STA;
    WiFiManager::start();
    uint8_t ctr = 0;
    while(ctr < 10 &&  WiFi.status() != WL_CONNECTED) {
      ctr++;
      delay(1000);
      Serial.print(".");
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
    Serial.println();
    digitalWrite(LED_BUILTIN, HIGH);
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println(WiFi.localIP());
    } else {
      currentWifiMode = WifiMode::AP;
      WiFiManager::start();
    }
  }

  void disableWifi(){
    WiFi.mode( WIFI_OFF );
  }
}
