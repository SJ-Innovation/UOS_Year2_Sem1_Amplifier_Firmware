#pragma once

#include "Arduino.h"
#include "LiquidCrystal_I2C.h"

enum class MenuScreen {
  INIT, STATUS, MAIN_MENU, CHANGE_UNIT, CHANNEL_SEL, MASTER_SETTING, L_CHANNEL, R_CHANNEL
};

extern LiquidCrystal_I2C LCDObj;

void LCDInit(uint32_t *ErrorCode);

void LCDMenuDraw();

bool LCDSelect();

bool LCDMove(int32_t Amount);