namespace RealTimeClock {
  enum class RTCMode {ONLINE, OFFLINE};
  RTCMode rtcMode = RTCMode::OFFLINE;
  time_t lastOfflineSync;
  
  void timeIsSetCb(bool from_sntp /* <= this parameter is optional */) {
    Serial.println(F("Time was set"));
    if (from_sntp) {
      Serial.println(F("Updating RTC"));
      time_t now = time(nullptr);
      Serial.println(now);
      Serial.println(now);
      RTC.setEpoch(now);
    }
  }
  
  void setup() {
    // Setup RTC
    RTC.begin();
    if (! RTC.isRunning()) RTC.startClock();
    RTC.setHourMode(CLOCK_H24);
    RTC.disableAlarm1();
    RTC.disableAlarm2();
    // setup RTC time
    // it will be used until NTP server will send us real current time
    Serial.println(F("Manually setting some time from the RTC:"));
    time_t rtc = RTC.getEpoch();
    timeval tv = { rtc, 0 };
    settimeofday(&tv, nullptr);

    lastOfflineSync = rtc;
    
    settimeofday_cb(RealTimeClock::timeIsSetCb);
    
    configTime(MY_TZ, "0.de.pool.ntp.org", "1.de.pool.ntp.org", "2.de.pool.ntp.org");
  }

  void update() {
    if (rtcMode == RTCMode::OFFLINE) {
      time_t timeNow = time(nullptr);
      if (timeNow - lastOfflineSync > 3600) {
        timeval tv = { RTC.getEpoch(), 0 };
        settimeofday(&tv, nullptr);
        lastOfflineSync = time(nullptr);;
      }
    }
  }

  time_t syncWithRTCProvider()
  {
    return RTC.getEpoch();
  }

  void offlineMode() {
    rtcMode = RTCMode::OFFLINE;
    lastOfflineSync = time(nullptr);
  }

  void onlineMode() {
     rtcMode = RTCMode::ONLINE;
  }
}
