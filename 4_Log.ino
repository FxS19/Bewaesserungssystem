#define LOG_TIME_INTERVAL 1800 // (30 Minuten)
#define BYTES_PER_LOG_ENTRY 6
#define LOG_ENTRIES ( DS3231_EEPROM_LAST_LOG_ADRESS - DS3231_EEPROM_FIRST_LOG_ADRESS ) / BYTES_PER_LOG_ENTRY

//#define LOG_DEBUG

namespace Log {
  time_t lastLog = 0;

  
  struct LogData {
    time_t time; //wird errechnet
    uint8_t soil; //(1 Addresse)
    float lux;    //(4 Addressen)
    byte flags;   //(1 Addresse)
  };

  void clear() {
    DS3231_Eeprom::write(DS3231_EEPROM_TIME_CURRENT_ADDRESS, (time_t)0);
    DS3231_Eeprom::write(DS3231_EEPROM_CURRENT_LOG_ADDRESS, (word)DS3231_EEPROM_FIRST_LOG_ADRESS);
    for (word i = DS3231_EEPROM_FIRST_LOG_ADRESS; i < DS3231_EEPROM_LAST_LOG_ADRESS; i++) {
      DS3231_Eeprom::write(i, (byte)0x0);
    }
  }

  void write(LogData data) {
    unsigned int address = DS3231_Eeprom::read_word(DS3231_EEPROM_CURRENT_LOG_ADDRESS);
    #ifdef LOG_DEBUG
    Serial.print("WriteLog:\taddress:");
    Serial.print(address);
    #endif
    
    if (address + BYTES_PER_LOG_ENTRY > DS3231_EEPROM_LAST_LOG_ADRESS) address = DS3231_EEPROM_FIRST_LOG_ADRESS;
    
    DS3231_Eeprom::write(address, (byte)data.soil);
    address += 1;
    DS3231_Eeprom::write(address, data.lux);
    address += 4;
    DS3231_Eeprom::write(address, data.flags);
    address += 1;
    DS3231_Eeprom::write(DS3231_EEPROM_TIME_CURRENT_ADDRESS, data.time);
    
    DS3231_Eeprom::write(DS3231_EEPROM_CURRENT_LOG_ADDRESS, (int)address);
    lastLog = data.time;
  }

  LogData read(int negativeValueOffset) {
    if (negativeValueOffset > LOG_ENTRIES) { // Wert zu alt
      struct LogData data = {
        (time_t)0, //time
        0, //soil
        0, //lux
        0  //flags
      };
      return data;
    }
    unsigned int address = DS3231_Eeprom::read_int(DS3231_EEPROM_CURRENT_LOG_ADDRESS);
    unsigned int address_offset = negativeValueOffset * BYTES_PER_LOG_ENTRY;
    if (address - DS3231_EEPROM_FIRST_LOG_ADRESS < address_offset) { // overflow
      address = DS3231_EEPROM_LAST_LOG_ADRESS  + address;
    }
    address -= address_offset;
    struct LogData data = {
      DS3231_Eeprom::read_time_t(DS3231_EEPROM_TIME_CURRENT_ADDRESS) - LOG_TIME_INTERVAL * (time_t)negativeValueOffset, //time
      (uint8_t)DS3231_Eeprom::read_byte(address), //soil
      DS3231_Eeprom::read_float(address+=1), //lux
      DS3231_Eeprom::read_byte(address+=4) //flags
    };
    
    #ifdef LOG_DEBUG
    Serial.print(F("Reading from address "));
    Serial.print(address);
    Serial.print(F(" Curent log address "));
    Serial.print(DS3231_Eeprom::read_word(DS3231_EEPROM_CURRENT_LOG_ADDRESS));
    Serial.print(":\t");
    #endif
    
    return data;
  }

  time_t getLastLog() {
    return DS3231_Eeprom::read_time_t(DS3231_EEPROM_TIME_CURRENT_ADDRESS);
  }

  bool needsToLog() {
    if (lastLog > time(nullptr)) return true;
    return lastLog + LOG_TIME_INTERVAL < time(nullptr);
  }

  void setup() {
    lastLog = Log::getLastLog();
  }
}
