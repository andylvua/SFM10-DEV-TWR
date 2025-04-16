
/* (c) SynchronicIT B.V. 2021. All rights reserved.                                   24/02/2025
             _____                  _               ______         _             
            / ____|                | |             |  ____|       (_)             TM
           | (___  _   _ _ __   ___| |__  _ __ ___ | |__ _   _ ___ _  ___  _ __  
            \___ \| | | | '_ \ / __| '_ \| '__/ _ \|  __| | | / __| |/ _ \| '_ \ 
            ____) | |_| | | | | (__| | | | | | (_) | |  | |_| \__ \ | (_) | | | |
           |_____/ \__, |_| |_|\___|_| |_|_|  \___/|_|   \__,_|___/_|\___/|_| |_|
                    __/ |                                                        
                   |___/                                 http://www.synchronicit.nl/ 

  This software is confidential and proprietary of SynchronicIT and is subject to the terms and 
  conditions defined in file 'LICENSE.txt', which is part of this source code package. You shall 
  not disclose such Confidential Information and shall use it only in accordance with the terms 
  of the license agreement.

*/

#include "user-application.h"

#define DISP_CLK_PIN  (D5)
#define DISP_DATA_PIN (D7)

#define DISP_CLK_HIGH() { PORTD |= DISP_CLK_PIN; }
#define DISP_CLK_LOW() { PORTD &= ~DISP_CLK_PIN; }
#define DISP_DATA_HIGH() { PORTD |= DISP_DATA_PIN; }
#define DISP_DATA_LOW() { PORTD &= ~DISP_DATA_PIN; }


// bit-bang macro:
#define matrix_send_fast(data, bits) { \
  for (uint8_t i = 0; i < bits; i++) { \
    PORTD = data_clk; \
    if (data & (1ul<<i)) { \
      PORTD = DATA_clk; \
      PORTD = DATA_CLK; \
    } else { \
      PORTD = data_clk; \
      PORTD = data_CLK; \
    } \
  } \
}


void matrix_init(void) {
  pinMode(DISP_DATA_PIN | DISP_CLK_PIN, OUTPUT_HIGH);
}

// show single 64 bit value with 3-bit intensity:
void matrix_refresh(uint64_t* data, uint8_t intensity_3bit) {
  uint32_t data_clk = PORTD & ~(DISP_CLK_PIN | DISP_DATA_PIN);
  uint32_t data_CLK = data_clk | DISP_CLK_PIN;
  uint32_t DATA_clk = data_clk | DISP_DATA_PIN;
  uint32_t DATA_CLK = data_clk | DISP_CLK_PIN | DISP_DATA_PIN;
  PORTD = data_CLK; 
  matrix_send_fast(0x44, 8);
  PORTD = DATA_CLK;
  PORTD = data_CLK; 
  matrix_send_fast(0xC0, 8);
  uint32_t d1 = ((uint32_t*)data)[0];
  uint32_t d2 = ((uint32_t*)data)[1];
  matrix_send_fast(d1, 32);
  matrix_send_fast(d2, 32);
  PORTD = DATA_CLK;
  PORTD = data_CLK;
  PORTD = data_clk;
  PORTD = data_CLK;
  PORTD = DATA_CLK;
  PORTD = data_CLK; 
  uint8_t intensity_cmd = 0x88|(intensity_3bit);
  matrix_send_fast(intensity_cmd, 8);
  PORTD = DATA_CLK; 
}


// iterate every pixel
void show_animation() {
  for (int i = 0 ; i < 64; i++) {
    uint64_t data = 1ull << (i % 64);
    matrix_refresh(&data, 7);
    delay(10);
  }
}

uint64_t sprite_heart = 0x1824428181996600;

void setup() {
    matrix_init();
    // show heart sprite for 5 seconds
    matrix_refresh(&sprite_heart, 7);
    delay(5000);
}

// play animation
void loop() {
  show_animation();
}

