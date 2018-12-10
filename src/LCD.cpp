


#include "LCD.h"
#include "ChannelStore.h"

LiquidCrystal_I2C LCDObj(0x27, 20, 4);


MenuScreen CurrentMenu = MenuScreen::STATUS;
int8_t CursorPosition = 0;
int8_t CursorPositionX = 0;
int8_t CursorPositionY = 0;
uint8_t *CurrentSelectedItem = NULL;
#define BLANK LCDObj.print(F("                    "));

char StrExtended[4];

uint8_t UnitedValVol(uint8_t RawVal) {
  switch (DigipotUnit) {
    case DigipotUnit_t::RAW_DIGIPOT_STEPS:
      return RawVal;
      break;

    case DigipotUnit_t::CUTOFF_FREQ_HZ:
    case DigipotUnit_t::DIGIPOT_PERCENT:
      return uint8_t(round(100*RawVal / 255.0F));
      break;

  }
  return 0;
}

uint8_t UnitedValBass(uint8_t RawVal) {
  switch (DigipotUnit) {
    case DigipotUnit_t::RAW_DIGIPOT_STEPS:
      return RawVal;
      break;

    case DigipotUnit_t::DIGIPOT_PERCENT:
      return uint8_t(round(100*RawVal / 255.0F));
      break;

    case DigipotUnit_t::CUTOFF_FREQ_HZ:
      return 0;
      break;

  }
  return 0;
}

uint8_t UnitedValTreb(uint8_t RawVal) {
  switch (DigipotUnit) {
    case DigipotUnit_t::RAW_DIGIPOT_STEPS:
      return RawVal;
      break;

    case DigipotUnit_t::DIGIPOT_PERCENT:
      return uint8_t(round(100*RawVal / 255.0F));
      break;

    case DigipotUnit_t::CUTOFF_FREQ_HZ:
      return 0;
      break;

  }
  return 0;
}


char *Extend(uint8_t Val) {
  StrExtended[3] = '\0';
  StrExtended[0] = '0' + (Val / 100);
  StrExtended[1] = '0' + ((Val % 100) / 10);
  StrExtended[2] = '0' + ((Val % 100) % 10);
  return StrExtended;
}

void LCDInit(uint32_t *ErrorCode) {
  LCDObj.init();
  LCDObj.setCursor(0, 0);
  LCDObj.backlight();
  LCDObj.print(F("  Stereo Amp V1.0   "));
  LCDObj.setCursor(0, 1);
  LCDObj.print(F("    Starting Up!    "));
  LCDObj.setCursor(0, 2);
}


void LCDDrawStatusScreen() {
  LCDObj.setCursor(0, 0);
  LCDObj.print(F("  L   Overall    R  "));
  LCDObj.setCursor(0, 1);
  LCDObj.print(F("Vol:       Vol:     "));
  LCDObj.setCursor(7, 1);
  LCDObj.print(VolSuffixes[(int8_t) DigipotUnit]);
  LCDObj.setCursor(18, 1);
  LCDObj.print(VolSuffixes[(int8_t) DigipotUnit]);
  LCDObj.setCursor(0, 2);
  LCDObj.print(F("Bas:       Bas:     "));
  LCDObj.setCursor(7, 2);
  LCDObj.print(BassSuffixes[(int8_t) DigipotUnit]);
  LCDObj.setCursor(18, 2);
  LCDObj.print(BassSuffixes[(int8_t) DigipotUnit]);
  LCDObj.setCursor(0, 3);
  LCDObj.print(F("Trb:       Trb:     "));
  LCDObj.setCursor(7, 3);
  LCDObj.print(TrebSuffixes[(int8_t) DigipotUnit]);
  LCDObj.setCursor(18, 3);
  LCDObj.print(TrebSuffixes[(int8_t) DigipotUnit]);

  ChannelConfig_t LOverall = CalculateOverall(&MasterConfig, &LeftChannelConfig);
  ChannelConfig_t ROverall = CalculateOverall(&MasterConfig, &RightChannelConfig);

  LCDObj.setCursor(4, 1);
  LCDObj.print(Extend(UnitedValVol(LOverall.Volume)));
  LCDObj.setCursor(4, 2);
  LCDObj.print(Extend(UnitedValBass(LOverall.Bass)));
  LCDObj.setCursor(4, 3);
  LCDObj.print(Extend(UnitedValTreb(LOverall.Treble)));

  LCDObj.setCursor(15, 1);
  LCDObj.print(Extend(UnitedValVol(ROverall.Volume)));
  LCDObj.setCursor(15, 2);
  LCDObj.print(Extend(UnitedValBass(ROverall.Bass)));
  LCDObj.setCursor(15, 3);
  LCDObj.print(Extend(UnitedValTreb(ROverall.Treble)));


}

void LCDDrawChannelSel() {
  LCDObj.setCursor(0, 0);
  LCDObj.print(F("   Choose Channel   "));
  LCDObj.setCursor(0, 1);
  LCDObj.print(F("       Master       "));
  LCDObj.setCursor(0, 2);
  LCDObj.print(F("   Left      Right  "));
  LCDObj.setCursor(0, 3);
  BLANK

}

void LCDDrawSettings(ChannelConfig_t *Config) {
  LCDObj.setCursor(0, 1);
  LCDObj.print(F("      Vol:          "));
  LCDObj.setCursor(0, 2);
  LCDObj.print(F("     Bass:          "));
  LCDObj.setCursor(0, 3);
  LCDObj.print(F("     Treb:          "));
  LCDObj.setCursor(11, 1);
  LCDObj.print(Extend(UnitedValVol(Config->Volume)));
  LCDObj.setCursor(11, 2);
  LCDObj.print(Extend(UnitedValBass(Config->Bass)));
  LCDObj.setCursor(11, 3);
  LCDObj.print(Extend(UnitedValTreb(Config->Treble)));

  LCDObj.setCursor(14, 1);
  LCDObj.print(VolSuffixes[(int8_t) DigipotUnit]);
  LCDObj.setCursor(14, 2);
  LCDObj.print(BassSuffixes[(int8_t) DigipotUnit]);
  LCDObj.setCursor(14, 3);
  LCDObj.print(TrebSuffixes[(int8_t) DigipotUnit]);
}

void LCDDrawMasterSettings() {
  LCDObj.setCursor(0, 0);
  LCDObj.print(F("  Master Settings   "));
  LCDDrawSettings(&MasterConfig);
}

void LCDDrawLeftSettings() {
  LCDObj.setCursor(0, 0);
  LCDObj.print(F("   Left Settings    "));
  LCDDrawSettings(&LeftChannelConfig);
}

void LCDDrawRightSettings() {
  LCDObj.setCursor(0, 0);
  LCDObj.print(F("   Right Settings   "));
  LCDDrawSettings(&RightChannelConfig);
}

void LCDDrawMainMenu() {
  LCDObj.setCursor(0, 0);
  LCDObj.print(F("      Main Menu     "));
  LCDObj.setCursor(0, 1);
  LCDObj.print(F("    Edit Settings   "));
  LCDObj.setCursor(0, 2);
  LCDObj.print(F("   Edit Value Unit  "));
  LCDObj.setCursor(0, 3);
  BLANK;
}

void LCDDrawChangeUnit() {
  LCDObj.setCursor(0, 0);
  LCDObj.print(F("     Units Used     "));
  LCDObj.setCursor(0, 1);
  LCDObj.print(F("  Raw Digipot Steps "));
  LCDObj.setCursor(0, 2);
  LCDObj.print(F("  Digipot Percent   "));
  LCDObj.setCursor(0, 3);
  LCDObj.print(F("  Cutoff Freq HzWIP "));
  LCDObj.setCursor(19, ((uint8_t) DigipotUnit) + (uint8_t) 1);
  LCDObj.print("<");
}

void LCDDrawCursor() {
  LCDObj.setCursor(CursorPositionX, CursorPositionY);
  LCDObj.print((CursorPositionX == 0 && CursorPositionY == 0) ? "<-" : "->");
}

void LCDMenuDraw() {
  bool DrawCursor = true;
  switch (CurrentMenu) {
    case MenuScreen::INIT:
      break;
    case MenuScreen::STATUS:
      LCDDrawStatusScreen();
      DrawCursor = false;
      break;
    case MenuScreen::MAIN_MENU:
      LCDDrawMainMenu();
      break;
    case MenuScreen::CHANGE_UNIT:
      LCDDrawChangeUnit();
      break;
    case MenuScreen::CHANNEL_SEL:
      LCDDrawChannelSel();
      break;
    case MenuScreen::MASTER_SETTING:
      LCDDrawMasterSettings();
      break;
    case MenuScreen::L_CHANNEL:
      LCDDrawLeftSettings();
      break;
    case MenuScreen::R_CHANNEL:
      LCDDrawRightSettings();
      break;
  }

  if (DrawCursor) {
    LCDDrawCursor();
  }


}

#define KEEP_WITHIN(Min, X, Max) X=constrain(X,Min,Max)
#define SAFE_EDIT_VAL(Var, EditAmount) \
  while (EditAmount) {\
    Var += abs(EditAmount) / EditAmount;\
    EditAmount -= abs(EditAmount) / EditAmount;\
    KEEP_WITHIN(1,Var,254);\
  }

void LCDMoveHandleStatus(int8_t MoveAmount) {
  SAFE_EDIT_VAL(MasterConfig.Volume, MoveAmount);
}

void LCDMoveHandleChannelSel(int8_t MoveAmount) {
  CursorPosition += MoveAmount;
  KEEP_WITHIN(0, CursorPosition, 3);
  switch (CursorPosition) {
    case 0:
      CursorPositionX = CursorPositionY = 0;
      break;
    case 1:
      CursorPositionX = 5;
      CursorPositionY = 1;
      break;
    case 2:
      CursorPositionX = 1;
      CursorPositionY = 2;
      break;
    case 3:
      CursorPositionX = 11;
      CursorPositionY = 2;
      break;

  }

}

void LCDMoveHandleGenericSettings(int8_t MoveAmount) {
  CursorPosition += MoveAmount;
  KEEP_WITHIN(0, CursorPosition, 3);
  CursorPositionX = 0;
  CursorPositionY = CursorPosition;
}

void LCDMoveMainMenu(int8_t MoveAmount) {
  CursorPosition += MoveAmount;
  KEEP_WITHIN(0, CursorPosition, 2);
  CursorPositionX = 0;
  CursorPositionY = CursorPosition;
}

void LCDMoveChangeUnit(int8_t MoveAmount) {
  CursorPosition += MoveAmount;
  KEEP_WITHIN(0, CursorPosition, 2);
  CursorPositionX = 0;
  CursorPositionY = CursorPosition;
}

bool LCDMove(int32_t Amount) {
  if (CurrentSelectedItem == NULL) {
    switch (CurrentMenu) {
      case MenuScreen::INIT:
        break;

      case MenuScreen::STATUS:
        LCDMoveHandleStatus(Amount);
        break;

      case MenuScreen::MAIN_MENU:
        LCDMoveMainMenu(Amount);
        break;

      case MenuScreen::CHANGE_UNIT:
        LCDMoveChangeUnit(Amount);
        break;

      case MenuScreen::CHANNEL_SEL:
        LCDMoveHandleChannelSel(Amount);
        break;

      case MenuScreen::MASTER_SETTING:
      case MenuScreen::L_CHANNEL:
      case MenuScreen::R_CHANNEL:
        LCDMoveHandleGenericSettings(Amount);
        break;
    }
  }
  else {
    SAFE_EDIT_VAL(*CurrentSelectedItem, Amount);
  }
  return true;
}


void LCDSelectHandleStatus() {
  CurrentMenu = MenuScreen::MAIN_MENU;
  CursorPosition = 0;
}

void LCDSelectHandleChannelSel() {
  switch (CursorPosition) {
    case 0:
      CurrentMenu = MenuScreen::MAIN_MENU;
      CursorPosition = 0;
      break;
    case 1:
      CurrentMenu = MenuScreen::MASTER_SETTING;
      CursorPosition = 0;
      break;
    case 2:
      CurrentMenu = MenuScreen::L_CHANNEL;
      CursorPosition = 0;
      break;
    case 3:
      CurrentMenu = MenuScreen::R_CHANNEL;
      CursorPosition = 0;
      break;
  }

}

void LCDSelectHandleMasterSettings() {
  switch (CursorPosition) {
    case 0:
      CurrentMenu = MenuScreen::CHANNEL_SEL;
      CursorPosition = 0;
      break;
    case 1:
      CurrentSelectedItem = &MasterConfig.Volume;
      break;
    case 2:
      CurrentSelectedItem = &MasterConfig.Bass;
      break;
    case 3:
      CurrentSelectedItem = &MasterConfig.Treble;
      break;
  }
}

void LCDSelectHandleLeftSettings() {
  switch (CursorPosition) {
    case 0:
      CurrentMenu = MenuScreen::CHANNEL_SEL;
      CursorPosition = 0;
      break;
    case 1:
      CurrentSelectedItem = &LeftChannelConfig.Volume;
      break;
    case 2:
      CurrentSelectedItem = &LeftChannelConfig.Bass;
      break;
    case 3:
      CurrentSelectedItem = &LeftChannelConfig.Treble;
      break;
  }
}

void LCDSelectHandleRightSettings() {
  switch (CursorPosition) {
    case 0:
      CurrentMenu = MenuScreen::CHANNEL_SEL;
      CursorPosition = 0;
      break;
    case 1:
      CurrentSelectedItem = &RightChannelConfig.Volume;
      break;
    case 2:
      CurrentSelectedItem = &RightChannelConfig.Bass;
      break;
    case 3:
      CurrentSelectedItem = &RightChannelConfig.Treble;
      break;
  }
}

void LCDSelectHandleMainMenu() {
  switch (CursorPosition) {
    case 0:
      CurrentMenu = MenuScreen::STATUS;
      CursorPosition = 0;
      break;
    case 1:
      CurrentMenu = MenuScreen::CHANNEL_SEL;
      CursorPosition = 0;
      break;
    case 2:
      CurrentMenu = MenuScreen::CHANGE_UNIT;
      CursorPosition = 0;
      break;
  }
}

void LCDSelectHandleChangeUnit() {
  switch (CursorPosition) {
    case 0:
      CurrentMenu = MenuScreen::MAIN_MENU;
      CursorPosition = 0;
      break;
    default:
      DigipotUnit = (DigipotUnit_t) (CursorPosition - 1);
      break;
  }
}

bool LCDSelect() {
  if (CurrentSelectedItem == NULL) {
    switch (CurrentMenu) {
      case MenuScreen::INIT:
        break;

      case MenuScreen::STATUS:
        LCDSelectHandleStatus();
        break;

      case MenuScreen::MAIN_MENU:
        LCDSelectHandleMainMenu();
        break;

      case MenuScreen::CHANGE_UNIT:
        LCDSelectHandleChangeUnit();
        break;

      case MenuScreen::CHANNEL_SEL:
        LCDSelectHandleChannelSel();
        break;

      case MenuScreen::MASTER_SETTING:
        LCDSelectHandleMasterSettings();
        break;
      case MenuScreen::L_CHANNEL:
        LCDSelectHandleLeftSettings();
        break;
      case MenuScreen::R_CHANNEL:
        LCDSelectHandleRightSettings();
        break;
    }
    LCDMove(0);
  }
  else {
    CurrentSelectedItem = NULL;
  }
  return true;
}