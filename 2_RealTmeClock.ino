namespace RealTmeClock {
  void setup() {
    // Setup RTC
    RTC.begin();
    if (! RTC.isRunning()) RTC.startClock();
    RTC.setHourMode(CLOCK_H24);
    RTC.disableAlarm1();
    RTC.disableAlarm2();
  }
}