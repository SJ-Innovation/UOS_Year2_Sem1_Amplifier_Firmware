

#include "pins.h"
#include "Digipots.h"
#include "LCD.h"
#include "TimedAction.h"
#include "RotEnc.h"
#include "ChannelStore.h"


uint32_t ErrorState = 0; // [Free:24][DigipotChannels:8]



void setup() {
  Serial.begin(115200);

  LCDInit(&ErrorState);
  LCDObj.setCursor(0, 3);
  DigipotInit(&ErrorState);
  LCDObj.print(F("."));

  EncoderInit(&ErrorState);
  LCDObj.print(F("."));

  SoftPWMInit(&ErrorState);
  LCDObj.print(F("."));

  InitChannelStore(&ErrorState);
  SetEncoderColour(MasterConfig.Volume, MasterConfig.Bass, MasterConfig.Treble);
  UpdateDigipots();
  for (int i = 0; i < 17; i++) {
    LCDObj.print(".");
    delay(50);
  }

  LCDMenuDraw();

}


void Halt() {
  DigipotWriteWiper(DIGIPOT_CH_1_0, 0);
  DigipotWriteWiper(DIGIPOT_CH_1_1, 0);
  DigipotWriteWiper(DIGIPOT_CH_1_2, 0);
  DigipotWriteWiper(DIGIPOT_CH_1_3, 0);
  DigipotWriteWiper(DIGIPOT_CH_2_0, 0);
  DigipotWriteWiper(DIGIPOT_CH_2_1, 0);
  DigipotWriteWiper(DIGIPOT_CH_2_2, 0);
  DigipotWriteWiper(DIGIPOT_CH_2_3, 0);
  noInterrupts();
  for (int i = 2; i < A5; i++) {
    pinMode(i, INPUT);
  }
  while (true) { };
}


void ErrorChecker() {
  CheckDigipots(&ErrorState);

  if (ErrorState != 0) {
    LCDObj.clear();
    LCDObj.setCursor(0, 0);
    LCDObj.print("       HALTED!      ");
    LCDObj.setCursor(0, 1);
    LCDObj.print("   Critical Error   ");
    LCDObj.setCursor(0, 1);
    LCDObj.print("Shutdown for safety!");

    LCDObj.setCursor(0, 3);
    LCDObj.print("Error: #");
    LCDObj.print(ErrorState);
    Halt();
  }
}


TimedAction ActionErrorChecker = TimedAction(1000, ErrorChecker);


void loop() {
  if (UserInputHappened()) {
    bool RedrawNeeded = false;
    if (EncoderButtonEdge == EncState_t::RISING_EDGE) {
      RedrawNeeded += LCDSelect();
    }
    if (EncoderMovement) {
      RedrawNeeded += LCDMove(EncoderMovement);
    }
    if (RedrawNeeded) {
      LCDMenuDraw();
      UpdateDigipots();
      SaveToEEPROM();
      SetEncoderColour(MasterConfig.Volume, MasterConfig.Bass, MasterConfig.Treble);
    }

  }

  ActionErrorChecker.check();
}
