

#include "RotEnc.h"
#include "pins.h"
#include "digitalWriteFast.h"
#include "Encoder.h"
#include <TimerOne.h>

Encoder Enc(ENC_QUAD1_PIN, ENC_QUAD2_PIN);

void EncoderInit(uint32_t *ErrorCode) {
  Enc.write(0);
  pinMode(ENC_V_PIN, OUTPUT);
  digitalWrite(ENC_V_PIN, HIGH);
  pinMode(ENC_SW_PIN, INPUT);
  pinMode(ENC_LED_R_PIN, OUTPUT);
  pinMode(ENC_LED_G_PIN, OUTPUT);
  pinMode(ENC_LED_B_PIN, OUTPUT);
  digitalWrite(ENC_LED_R_PIN, HIGH);
  digitalWrite(ENC_LED_G_PIN, HIGH);
  digitalWrite(ENC_LED_B_PIN, HIGH);
}


bool ReadEncButton() {
  noInterrupts();
  bool RState = digitalReadFast(ENC_LED_R_PIN);
  bool GState = digitalReadFast(ENC_LED_G_PIN);
  bool BState = digitalReadFast(ENC_LED_B_PIN);
  digitalWriteFast(ENC_LED_R_PIN, HIGH);
  digitalWriteFast(ENC_LED_G_PIN, HIGH);
  digitalWriteFast(ENC_LED_B_PIN, HIGH);
  pinMode(ENC_V_PIN, INPUT_PULLUP); // Must be normal PinMode. Not Fast variant. INPUT_PULLUP issue.
  pinModeFast(ENC_SW_PIN, OUTPUT);
  digitalWriteFast(ENC_SW_PIN, LOW);
  bool SwSwate = digitalReadFast(ENC_V_PIN);
  pinModeFast(ENC_SW_PIN, INPUT);
  pinModeFast(ENC_V_PIN, OUTPUT);
  digitalWriteFast(ENC_V_PIN, HIGH);
  digitalWriteFast(ENC_LED_R_PIN, RState);
  digitalWriteFast(ENC_LED_G_PIN, GState);
  digitalWriteFast(ENC_LED_B_PIN, BState);
  interrupts();
  return !SwSwate;

}


#define SWITCH_BOUNCE_DELAY_US 1000

EncState_t EncSwitchEdge() {
  static bool LastState;
  static uint32_t LastChangeTime;
  bool CurrentState = ReadEncButton();
  if (CurrentState != LastState) {
    LastState = CurrentState;
    uint32_t CurrentTime = micros();
    if (CurrentTime - LastChangeTime >= SWITCH_BOUNCE_DELAY_US) {
      LastChangeTime = CurrentTime;
      if (CurrentState) {
        return EncState_t::RISING_EDGE;
      }
      else {
        return EncState_t::FALLING_EDGE;
      }
    }
  }
  return EncState_t::NO_CHANGE;
}

volatile uint8_t EncColourR = 0;
volatile uint8_t EncColourG = 0;
volatile uint8_t EncColourB = 0;

void SetEncoderColour(uint8_t R, uint8_t G, uint8_t B) {
  noInterrupts();
  EncColourR = R;
  EncColourG = G;
  EncColourB = B;
  interrupts();
}

void SoftPWMHandler() {
  static uint8_t RolloverCounter = 0;
  digitalWriteFast(ENC_LED_R_PIN, RolloverCounter >= EncColourR);
  digitalWriteFast(ENC_LED_G_PIN, RolloverCounter >= EncColourG);
  digitalWriteFast(ENC_LED_B_PIN, RolloverCounter >= EncColourB);
  RolloverCounter++;
}

void SoftPWMInit(uint32_t *ErrorCode) {
  Timer1.initialize(1000000 / (UINT8_MAX * 100));
  Timer1.attachInterrupt(SoftPWMHandler);
}

int32_t EncoderMovement = 0;
EncState_t EncoderButtonEdge = EncState_t::NO_CHANGE;

bool UserInputHappened() {
  EncoderMovement = Enc.read() / 4;
  if (EncoderMovement) { Enc.write(0); }
  EncoderButtonEdge = EncSwitchEdge();
  return (EncoderMovement != 0) || (EncoderButtonEdge != EncState_t::NO_CHANGE);
}