#include "Digipots.h"
#include "digitalWriteFast.h"
#include "SPI.h"
#include "ChannelStore.h"

uint16_t DigiPotSwapRaw(DigipotChannel_t ThisPot, uint16_t Command) {
  uint16_t RVal = 0;
  digitalWriteFast(ThisPot.CSPin, LOW);
  delayMicroseconds(1);
  RVal = SPI.transfer16(Command);
  delayMicroseconds(1);
  digitalWriteFast(ThisPot.CSPin, HIGH);
  return RVal;
}

void DigipotWriteWiper(DigipotChannel_t ThisPot, uint8_t Val) {
  DigiPotSwapRaw(ThisPot, ThisPot.WiperMask | MASK_WRITE | Val);
}

uint8_t DigipotReadWiper(DigipotChannel_t ThisPot) {
  uint16_t ReadData = DigiPotSwapRaw(ThisPot, ThisPot.WiperMask | MASK_READ);
  return (uint8_t) (ReadData & 0b11111111);
}

void DigipotWriteTCON(DigipotChannel_t ThisPot, uint8_t NewTCON) {
  DigiPotSwapRaw(ThisPot, MASK_REG_TCON | MASK_WRITE | NewTCON);
}

uint8_t DigipotReadTCON(DigipotChannel_t ThisPot) {
  uint16_t ReadData = DigiPotSwapRaw(ThisPot, MASK_REG_TCON | MASK_READ);
  return (uint8_t) (ReadData & 0b11111111);
}

uint8_t DigipotReadStatus(DigipotChannel_t ThisPot) {
  uint16_t ReadData = DigiPotSwapRaw(ThisPot, MASK_REG_STATUS | MASK_READ);
  return (uint8_t) (ReadData & 0b11111111);
}

void CheckDigipots(uint32_t *ErrorCode) {
  *ErrorCode |= ((DigipotReadStatus(DIGIPOT_CH_1_0) != 0b11100000) << 0);
  *ErrorCode |= ((DigipotReadStatus(DIGIPOT_CH_1_1) != 0b11100000) << 1);
  *ErrorCode |= ((DigipotReadStatus(DIGIPOT_CH_1_2) != 0b11100000) << 2);
  *ErrorCode |= ((DigipotReadStatus(DIGIPOT_CH_1_3) != 0b11100000) << 3);
  *ErrorCode |= ((DigipotReadStatus(DIGIPOT_CH_2_0) != 0b11100000) << 4);
  *ErrorCode |= ((DigipotReadStatus(DIGIPOT_CH_2_1) != 0b11100000) << 5);
  *ErrorCode |= ((DigipotReadStatus(DIGIPOT_CH_2_2) != 0b11100000) << 6);
  *ErrorCode |= ((DigipotReadStatus(DIGIPOT_CH_2_3) != 0b11100000) << 7);
}

void DigipotInit(uint32_t *ErrorCode) {
  Serial.println("Starting Up Digipots");

  pinMode(CH_1_DIG1_CS_PIN, OUTPUT);
  pinMode(CH_1_DIG2_CS_PIN, OUTPUT);
  pinMode(CH_2_DIG1_CS_PIN, OUTPUT);
  pinMode(CH_2_DIG2_CS_PIN, OUTPUT);
  digitalWriteFast(CH_1_DIG1_CS_PIN, HIGH);
  digitalWriteFast(CH_1_DIG2_CS_PIN, HIGH);
  digitalWriteFast(CH_2_DIG1_CS_PIN, HIGH);
  digitalWriteFast(CH_2_DIG2_CS_PIN, HIGH);

  SPI.begin();

  CheckDigipots(ErrorCode);

  Serial.print("Digipots Started");
}

void UpdateDigipots() {
  ChannelConfig_t LOverall = CalculateOverall(&MasterConfig, &LeftChannelConfig);
  ChannelConfig_t ROverall = CalculateOverall(&MasterConfig, &RightChannelConfig);

  DigipotWriteWiper(DIGIPOT_L_CHANNEL_VOL, LOverall.Volume);
  DigipotWriteWiper(DIGIPOT_R_CHANNEL_VOL, ROverall.Volume);
  DigipotWriteWiper(DIGIPOT_L_CHANNEL_BASS, LOverall.Bass);
  DigipotWriteWiper(DIGIPOT_R_CHANNEL_BASS, ROverall.Bass);
  DigipotWriteWiper(DIGIPOT_L_CHANNEL_TREBLE, LOverall.Treble);
  DigipotWriteWiper(DIGIPOT_R_CHANNEL_TREBLE, ROverall.Treble);

}