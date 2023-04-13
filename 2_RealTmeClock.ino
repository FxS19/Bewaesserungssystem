namespace RealTimeClock {
  enum class RTCMode {ONLINE, OFFLINE};
  RTCMode rtcMode = RTCMode::OFFLINE;
  time_t lastOfflineSync;
  
  void timeIsSetCb(bool from_sntp /* <= this parameter is optional */) {
    Serial.println(F("Time was set"));
    if (from_sntp) {
      Serial.println();
      Serial.println(F("Updating RTC"));
      time_t now = time(nullptr);
      DateTime dtNow = DateTime(now);
      Serial.print(F("UNIX time: "));
      Serial.println(dtNow.unixtime());
      RTC.adjust(dtNow);
      Serial.print(F("RTC time is: "));
      Serial.println(RTC.now().unixtime());
    }
  }
  
  void setup() {
    // Setup RTC
    if (! RTC.begin()) {
      Serial.println(F("Couldn't find RTC"));
      Serial.flush();
      while (1) delay(10);
    }

    RTC.disable32K();
    RTC.clearAlarm(1);
    RTC.clearAlarm(2);
    // setup RTC time
    // it will be used until NTP server will send us real current time
    Serial.println(F("Manually setting some time from the RTC:"));
    time_t rtc = RTC.now().unixtime();
    timeval tv = { rtc, 0 };
    settimeofday(&tv, nullptr);

    lastOfflineSync = rtc;
    
    settimeofday_cb(RealTimeClock::timeIsSetCb);
    
    configTime(MY_TZ, F("0.de.pool.ntp.org"), F("1.de.pool.ntp.org"), F("2.de.pool.ntp.org"));
  }

  void update() {
    if (rtcMode == RTCMode::OFFLINE) {
      time_t timeNow = time(nullptr);
      if (timeNow - lastOfflineSync > 3600) {
        timeval tv = { RTC.now().unixtime(), 0 };
        settimeofday(&tv, nullptr);
        lastOfflineSync = time(nullptr);;
      }
    }
  }

  void offlineMode() {
    rtcMode = RTCMode::OFFLINE;
    lastOfflineSync = time(nullptr);
  }

  void onlineMode() {
     rtcMode = RTCMode::ONLINE;
  }
}
