#define DS3231_EEPROM_SUNNY_SECONDS_ADDRESS 0 //2 Addressen (16bit)
#define DS3231_EEPROM_HAS_WATERED_ADDRESS 2 // 1 Addresse (1 bit)
#define DS3231_EEPROM_LAST_BACKUP_ADDRESS 3 // 8 Addressen (32 Bit)
#define DS3231_EEPROM_TIME_CURRENT_ADDRESS 11 //(time_t) 8 Addressen (32bit)
#define DS3231_EEPROM_CURRENT_LOG_ADDRESS 19 //(word) 4 Addressen (16 bit)
//
// 10 Byte frei
//
#define DS3231_EEPROM_FIRST_LOG_ADRESS 33
#define DS3231_EEPROM_LAST_LOG_ADRESS 4095 // Es passen genau 677 Werte#

//#define EEPROM_DEBUG_READ
//#define EEPROM_DEBUG_WRITE


namespace DS3231_Eeprom {

  void setup() {
    eeprom_ds3231.initialize();
  }

  byte read_byte(word eeaddress ) {
    #ifdef EEPROM_DEBUG_READ
    Serial.print(F("\t\tRead Byte: "));
    Serial.print(eeaddress);
    Serial.print("\tdata: ");
    byte val = eeprom_ds3231.readByte(eeaddress);
    for (int i = 7; i >= 0; i--)
    {
        bool b = bitRead(val, i);
        Serial.print(b);
    }
    Serial.println();
    #endif
    return eeprom_ds3231.readByte(eeaddress);
  }

  void write(word eeaddress, byte data ) {
    #ifdef EEPROM_DEBUG_WRITE
    Serial.print(F("Write Byte: "));
    Serial.print(eeaddress);
    Serial.print("\tdata: ");
    for (int i = 7; i >= 0; i--)
    {
        bool b = bitRead(data, i);
        Serial.print(b);
    }
    Serial.println();
    #endif
    if (read_byte(eeaddress) != data) { 
      eeprom_ds3231.writeByte(eeaddress, data);
      delay(10);
    }
  }

  void write(word eeaddress, word data ) {//benötigt 4 Addressen
    #ifdef EEPROM_DEBUG_WRITE
    Serial.print(F("Write word: "));
    Serial.print(eeaddress);
    Serial.print("\tdata: ");
    Serial.println(data);
    #endif
    byte* bytes = (byte*)&data;
    write(eeaddress    , bytes[0]);
    write(eeaddress + 1, bytes[1]);
    write(eeaddress + 2, bytes[2]);
    write(eeaddress + 3, bytes[3]);
  }

  word read_word(word eeaddress) {//benötigt 4 Addressen
    byte bytes[4] {
      read_byte(eeaddress), 
      read_byte(eeaddress + 1), 
      read_byte(eeaddress + 2), 
      read_byte(eeaddress + 3)
    };
    #ifdef EEPROM_DEBUG_READ
    Serial.print(F("\t\tRead word: "));
    Serial.print(eeaddress);
    Serial.print("\tdata: ");
    Serial.println(*(int*)bytes);
    #endif
    return *(word*)bytes;
  }

  void write(word eeaddress, int data) {
    write(eeaddress, (word)data );
  }

  int read_int(word eeaddress) {
    return (int)read_word(eeaddress);
  }

  void write(word eeaddress, float data) {//benötigt 4 Addressen
    write(eeaddress, (word)data );
  }

  float read_float(word eeaddress) {//benötigt 4 Addressen
    return (float)read_word(eeaddress);
  }

  void write(word eeaddress, time_t data) {//benötigt 4 Addressen
    #ifdef EEPROM_DEBUG_WRITE
    Serial.print(F("Write time_t: "));
    Serial.print(eeaddress);
    Serial.print("\tdata: ");
    Serial.println(data);
    #endif
    byte* bytes = (byte*)&data;
    for (uint8_t i = 0; i < 8; i++){
      write(eeaddress + i, bytes[i]);
    }
  }

  float read_time_t(word eeaddress) {//benötigt 4 Addressen
    byte bytes[8];
    for (uint8_t i = 0; i < 8; i++){
      bytes[i] = read_byte(eeaddress + i);
    }
    #ifdef EEPROM_DEBUG_READ
    Serial.print(F("\t\tRead time_t: "));
    Serial.print(eeaddress);
    Serial.print("\tdata: ");
    Serial.println(*(time_t*) &bytes);
    #endif
    return *(time_t*) &bytes;
  }
  
}
