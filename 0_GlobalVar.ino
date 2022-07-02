#define WATER_LEVEL_SENSOR_PIN D5
#define SOIL_SENSOR_PIN A0
#define PUMP_MAIN_PIN D6
#define PUMP_SECONDARY_PIN D7

static DS3231 RTC;

BH1750 lightMeter;

const char * ssid = STASSID; // your network SSID (name)
const char * pass = STAPSK;  // your network password
WiFiUDP udp;
EasyNTPClient ntpClient(udp, "pool.ntp.org", (2*60*60)); // IST = GMT + 5:30

// struct that defines the data we would like to log
struct LogData
{
    float temp;
    bool waterLevel;
    bool hasWatered;
    float lux;
    uint8_t soilMoisture;
};

SPIFFSLogger<LogData> logger("/log/", 7); //folder on SPIFFS and Days
