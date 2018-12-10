#pragma once

#include "Arduino.h"

typedef struct {
  uint8_t Volume;
  uint8_t Treble;
  uint8_t Bass;
} ChannelConfig_t;

enum class DigipotUnit_t : int8_t {
  RAW_DIGIPOT_STEPS = 0, DIGIPOT_PERCENT = 1, CUTOFF_FREQ_HZ = 2
};

extern const char VolSuffixes[3][3];
extern const char BassSuffixes[3][3];
extern const char TrebSuffixes[3][3];

extern ChannelConfig_t LeftChannelConfig;
extern ChannelConfig_t RightChannelConfig;
extern ChannelConfig_t MasterConfig;
extern DigipotUnit_t DigipotUnit;

void InitChannelStore(uint32_t *ErrorCode);

ChannelConfig_t CalculateOverall(ChannelConfig_t *Master, ChannelConfig_t *Modifier);

void SaveToEEPROM();

bool LoadFromEEPROM();