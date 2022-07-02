namespace WiFiManager {
  void setup() {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);

    for (int i = 0; (i < 20) && (WiFi.status() != WL_CONNECTED) ; i++) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(500);
      Serial.print(".");
      Serial.print(i);
    }
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("");

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    } else {
      //Open Hotspot for configuration
      WiFi.mode(WIFI_AP);
      WiFi.softAP("Giessanlage", "");
    }
  }
}