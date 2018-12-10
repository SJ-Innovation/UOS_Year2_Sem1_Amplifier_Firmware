#pragma once

#include "Arduino.h"
#include "pins.h"

#define MASK_WRITE      (uint16_t)0b0000000000000000
#define MASK_READ       (uint16_t)0b0000110000000000

#define MASK_REG_WIPER0 (uint16_t)0b0000000000000000
#define MASK_REG_WIPER1 (uint16_t)0b0001000000000000
#define MASK_REG_TCON   (uint16_t)0b0100000000000000

#define MASK_REG_STATUS (uint16_t)0b0101110000000000

typedef struct {
  int CSPin;
  uint16_t WiperMask;
} DigipotChannel_t;


#define DIGIPOT_CH_1_0 (DigipotChannel_t){CH_1_DIG1_CS_PIN, MASK_REG_WIPER0}
#define DIGIPOT_CH_1_1 (DigipotChannel_t){CH_1_DIG1_CS_PIN, MASK_REG_WIPER1}
#define DIGIPOT_CH_1_2 (DigipotChannel_t){CH_1_DIG2_CS_PIN, MASK_REG_WIPER0}
#define DIGIPOT_CH_1_3 (DigipotChannel_t){CH_1_DIG2_CS_PIN, MASK_REG_WIPER1}
#define DIGIPOT_CH_2_0 (DigipotChannel_t){CH_2_DIG1_CS_PIN, MASK_REG_WIPER0}
#define DIGIPOT_CH_2_1 (DigipotChannel_t){CH_2_DIG1_CS_PIN, MASK_REG_WIPER1}
#define DIGIPOT_CH_2_2 (DigipotChannel_t){CH_2_DIG2_CS_PIN, MASK_REG_WIPER0}
#define DIGIPOT_CH_2_3 (DigipotChannel_t){CH_2_DIG2_CS_PIN, MASK_REG_WIPER1}

#define DIGIPOT_L_CHANNEL_VOL DIGIPOT_CH_1_2
#define DIGIPOT_R_CHANNEL_VOL DIGIPOT_CH_1_3

#define DIGIPOT_L_CHANNEL_BASS DIGIPOT_CH_1_0
#define DIGIPOT_L_CHANNEL_TREBLE DIGIPOT_CH_1_1

#define DIGIPOT_R_CHANNEL_BASS DIGIPOT_CH_2_1
#define DIGIPOT_R_CHANNEL_TREBLE DIGIPOT_CH_2_0

void DigipotInit(uint32_t *ErrorCode);

void CheckDigipots(uint32_t *ErrorCode);

uint16_t DigiPotSwapRaw(DigipotChannel_t ThisPot, uint16_t Command);

void DigipotWriteWiper(DigipotChannel_t ThisPot, uint8_t Val);

uint8_t DigipotReadWiper(DigipotChannel_t ThisPot);

void DigipotWriteTCON(DigipotChannel_t ThisPot, uint8_t NewTCON);

uint8_t DigipotReadTCON(DigipotChannel_t ThisPot);

uint8_t DigipotReadStatus(DigipotChannel_t ThisPot);

void UpdateDigipots();