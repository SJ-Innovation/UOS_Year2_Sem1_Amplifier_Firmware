#include "ChannelStore.h"
#include "EEPROM.h"

ChannelConfig_t LeftChannelConfig;
ChannelConfig_t RightChannelConfig;
ChannelConfig_t MasterConfig;
DigipotUnit_t DigipotUnit;
const char VolSuffixes[3][3] = {"  ", "% ", "% "};
const char BassSuffixes[3][3] = {"  ", "% ", "Hz"};
const char TrebSuffixes[3][3] = {"  ", "% ", "Hz"};

void InitChannelStore(uint32_t *ErrorCode) {
  if (!LoadFromEEPROM()) {
    LeftChannelConfig.Volume = 254;
    LeftChannelConfig.Bass = 254;
    LeftChannelConfig.Treble = 254;
    RightChannelConfig.Volume = 254;
    RightChannelConfig.Bass = 254;
    RightChannelConfig.Treble = 254;
    MasterConfig.Volume = 128;
    MasterConfig.Treble = 128;
    MasterConfig.Bass = 128;
    DigipotUnit = DigipotUnit_t::DIGIPOT_PERCENT;
    SaveToEEPROM();
  }
}

ChannelConfig_t CalculateOverall(ChannelConfig_t *Master, ChannelConfig_t *Modifier) {
  ChannelConfig_t OverallConfig;
  OverallConfig.Volume = (Master->Volume * Modifier->Volume) / UINT8_MAX;
  OverallConfig.Bass = (Master->Bass * Modifier->Bass) / UINT8_MAX;
  OverallConfig.Treble = (Master->Treble * Modifier->Treble) / UINT8_MAX;
  return OverallConfig;
}

const char CompileTime[] = __TIME__;

void SaveToEEPROM() {
  uint16_t CurrentAddr = 0;
  EEPROM.put(CurrentAddr, CompileTime);
  CurrentAddr += sizeof(CompileTime);

  EEPROM.put(CurrentAddr, MasterConfig);
  CurrentAddr += sizeof(MasterConfig);

  EEPROM.put(CurrentAddr, RightChannelConfig);
  CurrentAddr += sizeof(RightChannelConfig);

  EEPROM.put(CurrentAddr, LeftChannelConfig);
  CurrentAddr += sizeof(LeftChannelConfig);

  EEPROM.put(CurrentAddr, DigipotUnit);
  CurrentAddr += sizeof(DigipotUnit);
}

bool LoadFromEEPROM() {
  uint16_t CurrentAddr = 0;
  char EEPROMCompileTime[sizeof(CompileTime)*sizeof(CompileTime[0])];
  EEPROM.get(CurrentAddr, EEPROMCompileTime);
  CurrentAddr += sizeof(CompileTime);

  if (strcmp(CompileTime, EEPROMCompileTime) != 0) {
    Serial.println(F("EEPROM: Something Changed. Resetting Settings."));
    return false;
  }

  Serial.println(F("EEPROM: Same Config As Last Start. Loading From EEPROM."));

  EEPROM.get(CurrentAddr, MasterConfig);
  CurrentAddr += sizeof(MasterConfig);
  EEPROM.get(CurrentAddr, RightChannelConfig);
  CurrentAddr += sizeof(RightChannelConfig);
  EEPROM.get(CurrentAddr, LeftChannelConfig);
  CurrentAddr += sizeof(LeftChannelConfig);
  EEPROM.get(CurrentAddr, DigipotUnit);
  CurrentAddr += sizeof(DigipotUnit);
  return true;
}