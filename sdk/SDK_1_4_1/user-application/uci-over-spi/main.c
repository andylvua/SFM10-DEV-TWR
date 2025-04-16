// Optionally disable notifications here by setting ENABLE_XXX_NTF 0

// #define ENABLE_RX_TIMESTAMP_NTF 0
// #define ENABLE_PAYLOAD_NTF 0
#define ENABLE_CIR_NTF 0
#define ENABLE_FIRST_PATH_INFO_NTF 0

// Do above #defines before including phy-uci-interpreter.h
#include "user-application.h"
#include "phy-uci-interpreter.h"
#include <string.h>

// RX/TX buffer we use to store SPI transactions:

#define SPI_BUF_SIZE (1500)
uint8_t spi_buffer[SPI_BUF_SIZE];

int spi_buf_queued_tx_start_index = 0;
int spi_buf_queued_tx_byte_count = 0;

// States of our state machine, see 6-wire-SPI-flow.png image 
// with hardware flow control mapping:
typedef enum DeviceState_t {
  STATE_BOOTING = 0,
  STATE_READY_TO_RECEIVED_CMD = 1,
  STATE_REQUESTED_TO_RECEIVE_CMD = 2,
  STATE_REQUESTED_TO_INTERPRET_CMD = 4,
  STATE_BUSY_INTERPRETING_CMD = 5,
  STATE_REQUESTING_TO_TRANSMIT = 6,
  STATE_BUSY_TRANSMITTING_RSP = 7,
  STATE_DONE_TRANSMITTING = 8
} DeviceState_t;

DeviceState_t state = STATE_BOOTING;

// Events defined for triggerEvent:
#define BIT(x) (1 << x)
enum {
	EVENT_CS_ASSERT = BIT(0),
	EVENT_CS_DEASSERT = BIT(1),
	EVENT_NTF_READY = BIT(2),
};

void spi_assert_irq() {
  digitalWrite(PIN_INT_N, HIGH);
  triggerEvent(EVENT_CS_ASSERT);
}

void spi_deassert_irq() {
  triggerEvent(EVENT_CS_DEASSERT);
}

void spi_transfer_irq() {
  // nothing to do
}


// Function to push a message to our OUTPUT buffer
void to_spi_buf(uint8_t gid, uint8_t oid, uint8_t* ptr, uint16_t payloadLength) {

  uint16_t len = payloadLength + 4;
  if (spi_buf_queued_tx_byte_count + len > SPI_BUF_SIZE) {
    Serial.print("Buf Overflow\n");
    return;
  }
  UciHeader hdr;
  uint8_t* hdrPtr = (uint8_t*)&hdr;
  UCI_NTF_HEADER(hdrPtr, gid, oid, payloadLength); 

  uint16_t iStart = (spi_buf_queued_tx_start_index + spi_buf_queued_tx_byte_count) % SPI_BUF_SIZE;
  spi_buf_queued_tx_byte_count += len;

  for (uint16_t i = 0 ; i < len; i++) {
    uint16_t writeIdx = (i + iStart) % SPI_BUF_SIZE;
    
    uint8_t* writePtr = &spi_buffer[writeIdx];
    uint8_t* readPtr;
    if (i < 4) {
      readPtr = &hdrPtr[i];
    } else {
      readPtr = &ptr[i-4];
    }
    *writePtr = readPtr[0];
  }
  triggerEvent(EVENT_NTF_READY);
}


// Macros to connect UWB.onXXX and notification handlers:
ADD_NOTIFICATION_CONTAINERS;
ADD_NOTIFICATION_ISR




// Transition part of our state machine, called from our loop():
void move_to_state(DeviceState_t new_state) {
  // Comment out to see state changes over UART:
  // uint8_t debug[] = {'0' + new_state}; Serial.write(debug, 1);

  state = new_state;
  switch(new_state) {

    case STATE_READY_TO_RECEIVED_CMD: {
      SPI.transfer(NULL, spi_buffer, SPI_BUF_SIZE);
    } break;

    case STATE_REQUESTED_TO_RECEIVE_CMD: {
      digitalWrite(PIN_RDY_N, LOW);
    } break;

    case STATE_REQUESTED_TO_INTERPRET_CMD: {
      digitalWrite(PIN_RDY_N, HIGH);
      move_to_state(STATE_BUSY_INTERPRETING_CMD);
    } break;

    case STATE_BUSY_INTERPRETING_CMD: {
      int cnt = SPI.count();
      uint8_t* outptr = &spi_buffer[cnt];
      
      UCI_execute(&spi_buffer[0], outptr);
      spi_buf_queued_tx_start_index = cnt;
      spi_buf_queued_tx_byte_count = GET_UCI_LEN(outptr)+4;

      move_to_state(STATE_REQUESTING_TO_TRANSMIT);
    } break;

    case STATE_REQUESTING_TO_TRANSMIT: {
      SPI.write(&spi_buffer[spi_buf_queued_tx_start_index], spi_buf_queued_tx_byte_count);
      // give the SPI DMA some time to prepare for the transmission:
      delayMicroseconds(10);
      digitalWrite(PIN_INT_N, LOW);
    } break;

    case STATE_BUSY_TRANSMITTING_RSP: {
      // nothing to do here, wait for DEASSERT
    } break;

    case STATE_DONE_TRANSMITTING: {
      digitalWrite(PIN_INT_N, HIGH);

      spi_buf_queued_tx_byte_count = 0;
      move_to_state(STATE_READY_TO_RECEIVED_CMD);
    } break;
  }
}


void setup() {
    // We set high UART baud rate so adding debug (UART) output 
    // anywhere does not cause signigicat delays
    Serial.begin(2000000);
    Serial.print("%s\n", __FILE__);
    attachInterrupt(ERROR_EVENT, Serial.printError, 0);

    // For debugging, we write out spi_buffer with some watermark:
    memset(spi_buffer, 0xab, sizeof(spi_buffer));
    
    // Setup our 6-wire SPI communication
    pinMode(PIN_INT_N | PIN_RDY_N, OUTPUT_HIGH);
    
    SPI.begin();

    SPI.beginTransaction ( 
        SPISettings(SPI_SLAVE_MODE, MSBFIRST, SPI_MODE0 | SPI_CS1 )
    );

    attachInterrupt(SPI_TRANSFER_COMPLETE, (void*)spi_transfer_irq, 0);
    attachInterrupt(SPI_TRANSFER_ASSERT, (void*)spi_assert_irq, 0);
    attachInterrupt(SPI_TRANSFER_DEASSERT, (void*)spi_deassert_irq, 0);

    SET_NOTIFICATION_HANDLERS
    
    // Handling SPI and notification events:
    while(1) {
        uint32_t eventMask = waitForEvent(100);

        if (eventMask & EVENT_CS_ASSERT) {

            switch (state) {
              case STATE_READY_TO_RECEIVED_CMD: {
                move_to_state(STATE_REQUESTED_TO_RECEIVE_CMD);
              } break;
              case STATE_REQUESTING_TO_TRANSMIT: {
                move_to_state(STATE_BUSY_TRANSMITTING_RSP);
              } break;
              default: {
                Serial.print("Error %u, state = %u\n", __LINE__, state);
              }
            }

        }

        if (eventMask & EVENT_CS_DEASSERT) {
          switch (state) {
            case STATE_REQUESTED_TO_RECEIVE_CMD: {
              move_to_state(STATE_REQUESTED_TO_INTERPRET_CMD);
            } break;
            case STATE_BUSY_TRANSMITTING_RSP: {
              int16_t writeCount = SPI.count();

              if (writeCount <= spi_buf_queued_tx_byte_count) {
                while (spi_buf_queued_tx_byte_count > 0 && writeCount > 0) {
                  int16_t len = GET_UCI_LEN(&spi_buffer[spi_buf_queued_tx_start_index]) + 4;
                  spi_buf_queued_tx_byte_count -= len;
                  spi_buf_queued_tx_start_index += len;
                  writeCount -= len;
                }

              }
              if (spi_buf_queued_tx_byte_count < 0) {
                spi_buf_queued_tx_byte_count = 0;
              }
              if (spi_buf_queued_tx_byte_count > 0) {
                move_to_state(STATE_REQUESTING_TO_TRANSMIT);
              } else {
                move_to_state(STATE_DONE_TRANSMITTING);
              }

            } break;
            default: {
              Serial.print("Error line %u, state = %u\n", __LINE__, state);
            }
          }
        }

        if (eventMask & EVENT_NTF_READY) {
          if (spi_buf_queued_tx_byte_count > 0 && state != STATE_REQUESTING_TO_TRANSMIT) {
            move_to_state(STATE_REQUESTING_TO_TRANSMIT);

          }
        }

        if (state == STATE_BOOTING) {
           move_to_state(STATE_READY_TO_RECEIVED_CMD);
        }
    }
}
