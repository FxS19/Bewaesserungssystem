/*
 * Benötigt eine Datei 00_SSID.ino
 * Mit dem Inhalt:
 *
 * #ifndef STASSID
 * #define STASSID "<<SSID>>"
 * #define STAPSK  "<<PASSWORD>>"
 * #endif
 */

#include <string>
#include <WiFiUdp.h>
#include <Wire.h>
#include "RTClib.h"
#include <BH1750.h>
#include <TZ.h>
#include <Schedule.h> //?
#include <coredecls.h> // settimeofday_cb()
#include <time.h>
#include <ESP8266WiFi.h>
#include <sys/time.h>
#include <LittleFS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <Eeprom24C32_64.h>
