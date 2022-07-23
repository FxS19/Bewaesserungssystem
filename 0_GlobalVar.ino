#define WATER_LEVEL_SENSOR_PIN D5
#define SOIL_SENSOR_PIN A0
#define PUMP_MAIN_PIN D6
#define PUMP_SECONDARY_PIN D7

#define DS3231_EEPROM_ADDRESS 0x57



//Interne EEPROM Adressen
#define EEPROM_SIZE 12
#define WATERING_POINTS_ADRESS 1
#define BASE_ML_PER_WATERING_POINT_ADRESS 2
#define OK_SOIL_MOISTURE_ADRESS 3
#define MAIN_PUMP_SPEED_ADRESS 4


#define MY_TZ TZ_Europe_Berlin

static DS3231 RTC;

BH1750 lightMeter;

const char * ssid = STASSID; // your network SSID (name)
const char * pass = STAPSK;  // your network password
WiFiUDP udp;

static Eeprom24C32_64 eeprom_ds3231(DS3231_EEPROM_ADDRESS);

//Loop
uint16_t sunnySecondsToday = 0;
bool hasWatered = false;
time_t lastExecutionTime;
time_t lastBackupTime = 0;
struct tm lastExecutionLocalTime;
float waterForToday = 0;

/*struct MyData
{
    float lux;
    bool waterLevel;
    uint8_t soil;
    float temperature;
};*/

// create a new logger which will store records of our MyData type in files with names like
// /log/mydata/YYYYMMDD, keeping 1 day of history
//SPIFFSLogger<MyData> logger("/log/mydata", 14);
