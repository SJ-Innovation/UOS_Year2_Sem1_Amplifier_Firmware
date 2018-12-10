#pragma once
#include "Arduino.h"

void EncoderInit(uint32_t *ErrorCode);
void SoftPWMInit(uint32_t *ErrorCode);

enum class EncState_t : uint8_t {
  NO_CHANGE = 0, RISING_EDGE = 1, FALLING_EDGE = 2
};

extern int32_t EncoderMovement;
extern EncState_t EncoderButtonEdge;
bool UserInputHappened();

EncState_t EncSwitchEdge();
void SetEncoderColour(uint8_t R, uint8_t G, uint8_t B);