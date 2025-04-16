//
// Created by Andrew Yaroshevych on 01.11.2024.
//
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "user-application.h"

/* *** Application configuration *** */
#define CALIBRATE_ANT 0                        // Set to 1 to calibrate antenna delay
#define SETUP_MODE 0                           // Set to 1 to print device address and exit
#define DIAGNOSTICS 1                          // Set to 1 to enable diagnostics
#define DEBUG 0                                // Set to 1 for debug mode (unused for now)
#define ERROR_LOG 1                            // Set to 1 to log errors

/* *** UWB configuration *** */
#define CIR_SAMPLE_CNT 136                                  // Number of samples for CIR data
#define LE_OFFSET 8                                         // Number of samples prior to the leading edge of the first path
#define RX_FP_OFFSET 16                                     // Number of samples prior to the first path index accumulated on RX
#define RX_CIR_SAMPLE_CNT (RX_FP_OFFSET + CIR_SAMPLE_CNT)   // Number of samples accumulated on RX

#define RADIO_CFG 10    // Radio configuration

#if RADIO_CFG == 6
#define ANT_DELAY 14222 // Increase the antenna delay to account for larger preamble
#else
#define ANT_DELAY 13914
#endif

#define TX_POWER 120

typedef int16_t cir_sample_t;
#define CIR_SAMPLE_BYTES (2 * sizeof(cir_sample_t)) // Each CIR sample is 4 bytes (real + imaginary part)

SetConfigurationCmd config;

/* Role of the device in the ranging transaction */
typedef enum {
    Twr_Role_Anchor = 0,
    Twr_Role_Tag = 1
} role_t;

/* *** TDMA configuration *** */
/*
 * Superframe structure:
 *|| - | ------- | ------- | ... | ------- ||
 * ^   ^         ^               ^
 * ^   Slot 0    Slot 1          Slot N
 * Guard time
 *
 * The superframe consists of N slots, each of which can be assigned to one of the anchors.
 * The guard time at the start of the superframe ensures there is no inconsistency in the
 * superframe scheduling due to ADC readout time, timer delays, etc.
 */
#define NUM_ANCHORS 4

/* Best of what current implementation can achieve */
#define SLOT_PERIOD_US 3500
#define GUARD_TIME_US 500

#define SF_PERIOD_US (SLOT_PERIOD_US * NUM_ANCHORS + GUARD_TIME_US)

uint16_t anchorAddresses[NUM_ANCHORS] = {
    0xB4CA,
    0xB538,
    0xB557,
    0xB52E
};
// uint16_t anchorAntDelays[NUM_ANCHORS] = {};   // Must be determined using CALIBRATE_ANT

/* Set to 1 to enable LED indication.
 * If enabled, the anchors and tag will simultaneously blink LEDs */
#define LED_INDICATION 1

/* Set to 1 to enable NLoS detection.
 * If enabled, the anchors will read the NLoS sensor and include the information in the Final message */
#define NLOS_DETECTION 0

#define LOS_LED_PIN D7
#define NLOS_SENSOR_PIN D8

/* *** TWR settings *** */
#define MAX_DIST_M 30     // Maximum distance in meters
#define DIST_OFFSET_CM 30 // Distance offset in centimeters (will be subtracted from the calculated distance)

#define ANCHOR_MAX_FAULTS 50
#define TAG_MAX_FAULTS 250

#define RESP_CIR 0 // Set to 1 to use Response packet CIR instead of the Final packet CIR

#if RESP_CIR
#define FINAL_SCHEDULE_DELAY_US 550 // TODO!: Should be reviewed to account for CIR length
#else
#define FINAL_SCHEDULE_DELAY_US 150
#endif

/* *** Helper macros *** */
uint32_t interruptCounter;

#define APPLY_XTAL_CORRECTION(ppb, target) ((target) * (1.0 - (ppb) / 1000000000))
#define CHECK_PPB_RANGE(ppb) ((ppb) >= -256000 && (ppb) <= 256000)

#define TWR_ENTER_CRITICAL() interruptCounter = noInterrupts()
#define TWR_EXIT_CRITICAL() interrupts(interruptCounter)

/* *** Events *** */
#define WAKEUP_SF_START 0x04

/* *** Auxiliary *** */
#define SMALL_DELTA_MAX 63
#define SMALL_DELTA_MIN (-64)
#define MSB_FLAG 0x80

/* *** TWR packet structures *** */
/*
 * Header structure:
 * Parameter, Size (octets), Description
 * fCode      1              This octet identifies the type of the message.
 * addr16     2              The 16-bit address of the device that sent the message.
 *
 * 1. Poll message:
 * Parameter, Size (octets), Description
 * header     3              Unified header structure.
 * rNum       1              Range sequence number. After each range attempt,
 *                           this number is incremented (by modulo 256).
 *
 * 2. Response message:
 * Parameter,     Size (octets), Description
 * header         3              Unified header structure.
 * rNum           1              This is a range sequence number,
                                 corresponding to the range number as sent in the Poll.
 * slotCorrection 4              Slot correction value. Time difference between the actual
 *                               and expected time of arrival of the Poll message determined
 *                               by the Tag (in microseconds).
 *
 * 3. Final message:
 * Parameter,  Size (octets), Description
 * header      3              Unified header structure.
 * rNum        1              This is a range sequence number,
                              corresponding to the range number as sent in the Poll.
 * pollTxTime  8              Time of transmission of the Poll message (in ticks).
 * respRxTime  8              Time of reception of the Response message (in ticks).
 * finalTxTime 8              Time of transmission of the Final message (in ticks).
 * respFpIndex 2              Index of the first path in the Response message. Used for diagnostics.
 */

/* Each packet is required to have a unified header structure */
typedef struct {
    uint8_t fCode;
    uint16_t addr16;
} __attribute__((packed)) header_t;

/* Poll message structure */
typedef struct {
    header_t header;
    uint8_t rNum;
} __attribute__((packed)) poll_t;

/* Response message structure */
typedef struct {
    header_t header;
    uint8_t rNum;
#if LED_INDICATION
    uint8_t sfNum;
#endif
    int32_t slotCorrection;
} __attribute__((packed)) resp_t;

/* Final message structure */
typedef struct {
    header_t header;
    uint8_t rNum;
#if LED_INDICATION
    uint8_t sfNum;
#endif
    uint64_t pollTxTime;
    uint64_t respRxTime;
    uint64_t finalTxTime;
    uint16_t respFpi;
#if NLOS_DETECTION
    uint8_t losStatus;
#endif
#if RESP_CIR
    uint16_t cirBytesCount;
    uint8_t cirData[CIR_SAMPLE_CNT * CIR_SAMPLE_BYTES];
#endif
} __attribute__((packed)) final_t;

/* Function codes for messages */
typedef enum {
    Twr_Fcode_Poll = 0x84, // Initiator Poll message
    Twr_Fcode_Resp = 0x72, // Responder Response message
    Twr_Fcode_Final = 0x73 // Initiator Final message
} fcode_e;

/* *** TWR state information *** */
/* Transmission state */
typedef enum {
    Twr_Poll_Sent = 0x10,
    Twr_Resp_Sent = 0x11,
    Twr_Final_Sent = 0x12,
    Twr_Tx_Error = 0xBE
} tx_state_t;

/* TWR result */
typedef enum {
    Twr_OK = 0,
    Twr_Incorrect_Range = 1,
    Twr_Incorrect_Timestamping = 2,
} twr_result_t;

/* TWR transaction information */
RxTimestampNtf rxResult;
TxTimestampNtf txResult;
PayloadNtf rxPayload;
FirstPathInfoNtf rxFpi;
CirNtf rxCirBuf;

uint64_t pollTxTime;
uint64_t respRxTime;
uint64_t finalTxTime;
uint64_t pollRxTime;
uint64_t respTxTime;
uint64_t finalRxTime;

uint16_t pollFpi;
uint16_t respFpi;
uint16_t finalFpi;

uint8_t rangeNumber;
tx_state_t txState;
twr_result_t twrResult;
uint16_t currentAnchorAddr16;

/* *** Device state information *** */
// The role of the device in the ranging transaction. Will be determined during the setup
role_t role;
// The 16-bit address of the device. Will be determined during the setup
uint16_t addr16;
// The number of the current superframe (modulo 256)
uint8_t superframeNum;

uint16_t faultyTransactions;

/* For diagnostics. Currently unused */
int32_t xtalPpb;
int32_t lastSlotCorrection;
int32_t transactionTime;
int32_t guardTime;
volatile uint32_t SFStartTime_prev;
/* End of Currently unused */

// Flag indicating the reception of the Poll message during the last superframe
bool pollReceived = false;
// Flag indicating the reception of the Response message during the last superframe
bool respReceived = false;

volatile uint32_t SFStartTime; // Updated on ISR level
uint32_t nextWakeUpTime;

/* RX Interrupt buffer. Used to transfer data from the ISR level to the event handler */
#define RX_BUF_SIZE 2

typedef struct {
    uint64_t rxTimestamp;
    uint16_t payloadLength;

    // Diagnostics
    FirstPathInfoNtf fpi;
    CirNtf cirBuf;

    union {
        poll_t poll;
        resp_t resp;
        final_t final;
    } payload;

    uint8_t rxStatus;
} rx_packet_t;

struct {
    rx_packet_t buf[RX_BUF_SIZE];
    uint8_t head;
    uint8_t tail;
} rxBuffer;

/* *** Logging structures *** */
/*
 * UWB logging message structure:
 * Parameter, Size (octets),            Description
 * prefix     4                         A 4-byte string that identifies the start of the message.
 * data       Variable                  The data structure that contains the ranging data. This structure is shared
                                        with the receiver code and is defined in the data_types.h header file.
 * checksum   1                         Checksum of the data structure to ensure data integrity.
                                        Calculated as 0x55 ^ each byte of data.
 * cirData    sizeof(cir_sample_t) *    The CIR data structure that contains the channel impulse response.
              2 * CIR_SAMPLE_CNT
 * suffix     3                         A 3-byte string that identifies the end of the message.
 */
#include "data_types.h" // Include the shared data types

#define BAUD_RATE 2400000
#define SEND_CIR 1     // Set to 1 to send CIR data
#define USE_ENCODING 1 // Set to 1 to use delta encoding for CIR data

static const char PREFIX[3] = "UWB";
static const char SUFFIX[3] = "END";

typedef struct __attribute__((packed)) {
    uwbLogMessageData_t data;
    uint8_t checksum; // Data checksum
#if SEND_CIR
    uint16_t cirBytesCount; // Number of bytes encoded in the CIR data
    uint8_t cirData[CIR_SAMPLE_CNT * CIR_SAMPLE_BYTES]; // CIR data
#endif
} uwbLogMessage_t;

#define LOG_MESSAGE_DATA_SIZE (sizeof(uwbLogMessageData_t) + sizeof(uint8_t))

/* *** Function prototypes *** */
void twr_fault_callback();
void twr_tx_callback();
void anchor_rx_callback(const rx_packet_t *packet);
void tag_rx_callback(const rx_packet_t *packet);
void anchor_send_poll();
void tag_received_final(const rx_packet_t *finalPacket);
void sf_start_callback();

/* *** Interrupt service routines *** */
/* RX ISR. Called when the UWB module receives a packet */
void rx_isr() {
    const int head = rxBuffer.head;
    const int tail = rxBuffer.tail;

    if ((head + 1) % RX_BUF_SIZE == tail) {
        // Overflow (packet drop)
#if ERROR_LOG
        Serial.print("RX buffer overflow\n");
#endif
        return;
    }

    rx_packet_t *packet = &rxBuffer.buf[head];

    packet->rxTimestamp = rxResult.rxTimestamp;
    packet->payloadLength = rxPayload.payloadLength;

    packet->fpi = rxFpi;
    packet->cirBuf = rxCirBuf;

    packet->rxStatus = rxResult.uciStatus | rxPayload.uciStatus | rxFpi.uciStatus | rxCirBuf.uciStatus;

    // Copy the payload
    memcpy(&packet->payload, rxPayload.payloadData, rxPayload.payloadLength);

    rxBuffer.head = (head + 1) % RX_BUF_SIZE;

    triggerEvent(RX_DONE);
}

/* Superframe start ISR. Called by Timer at the start of each superframe */
void sf_start_isr() {
    // SFStartTime_prev = SFStartTime;
    SFStartTime = micros();

    if (role == Twr_Role_Tag) {
        /* In general, this is a bad practice to perform such operations in the ISR.
         * However, as this is the highest priority ISR, we can afford to do this here
         * and start the RX as soon as possible, without waiting for the event loop to
         * catch up. This helps shorten the guard time and thus superframe duration */
        UWB.stopAction();
        const UciStatus ret = UWB.startRx(ASAP);
        if (ret != SUCCESS) {
#if ERROR_LOG
            Serial.print("Poll RX error: %d\r\n", ret);
#endif
        }
    }

    triggerEvent(WAKEUP_SF_START);
}

/* *** Helper functions *** */
/* Encodes the CIR data using delta encoding */
size_t delta_encode(const void* cirBuffer, uint8_t* encodedData) {
    const cir_sample_t* cirData = (cir_sample_t*)cirBuffer;
    size_t encodeIndex = 0;

    encodedData[encodeIndex++] = cirData[0] >> 8;
    encodedData[encodeIndex++] = cirData[0] & 0xFF;
    encodedData[encodeIndex++] = cirData[1] >> 8;
    encodedData[encodeIndex++] = cirData[1] & 0xFF;

    for (size_t i = 2; i < CIR_SAMPLE_CNT * 2; i += 1) {
        const int16_t delta = cirData[i] - cirData[i - 2];

        if (delta >= SMALL_DELTA_MIN && delta <= SMALL_DELTA_MAX) {
            encodedData[encodeIndex++] = delta & 0x7F;
        } else {
            encodedData[encodeIndex++] = MSB_FLAG | delta >> 8 & 0x7F;
            encodedData[encodeIndex++] = delta & 0xFF;
        }
    }

    return encodeIndex;
}

/* Turn the LED on the given pin to the given state. Used to indicate the device status */
void turn_led(const int pin, const bool state) {
    digitalWrite(pin, state);
}

/* Sends the UWB logging data to the PC in binary format. */
void send_bin_to_pc(uwbLogMessage_t const *msg) {
    Serial.write((uint8_t *)PREFIX, sizeof(PREFIX));

    const uint8_t *ptr = (uint8_t *)msg;
    uint16_t len = LOG_MESSAGE_DATA_SIZE;
#if SEND_CIR
    len += sizeof(msg->cirBytesCount) + msg->cirBytesCount;
#endif
    while (len > 0) {
        const uint16_t chunk = len > 256 ? 256 : len;
        Serial.write(ptr, chunk);
        ptr += chunk;
        len -= chunk;
    }

    Serial.write((uint8_t *)SUFFIX, sizeof(SUFFIX));
}

/* Returns the index of the anchor in the list of
 * anchors if it is present, -1 otherwise */
int16_t get_anchor_index(const uint16_t addr16) {
    for (uint16_t i = 0; i < NUM_ANCHORS; i++) {
        if (anchorAddresses[i] == addr16) {
            return i;
        }
    }
    return -1;
}

/* Returns the slot time for the given Anchor address
 * relative to the start of the superframe */
uint16_t get_slot_time_us(const uint16_t addr16) {
    return get_anchor_index(addr16) * SLOT_PERIOD_US + GUARD_TIME_US;
}

/* Returns first pending RX packet */
rx_packet_t *get_rx_packet() {
    TWR_ENTER_CRITICAL();
    const int head = rxBuffer.head;
    const int tail = rxBuffer.tail;
    TWR_EXIT_CRITICAL();

    if (head == tail) {
        return NULL;
    }

    return &rxBuffer.buf[tail];
}

/* Removes the oldest RX packet from the buffer */
void remove_rx_packet() {
    TWR_ENTER_CRITICAL();
    const int tail = rxBuffer.tail;
    if (rxBuffer.head != tail) {
        rxBuffer.tail = (tail + 1) % RX_BUF_SIZE;
    }
    TWR_EXIT_CRITICAL();
}

/* *** Application setup *** */
void setup() {
    // Start the XTAL and UWB circuitry
    UWB.begin();

    SetConfiguration_setDefaults(&config);
    config.channelImpulseResponseStart = -RX_FP_OFFSET;
    config.channelImpulseResponseLength = RX_CIR_SAMPLE_CNT;
    config.rxRadioCfg = RADIO_CFG;
    config.txRadioCfg = RADIO_CFG;
    config.txPower = TX_POWER;
    UWB.setConfiguration(&config);

#if CALIBRATE_ANT
#define LOOPBACK_READY 0x123
#define LOOPBACK_COUNT 100

    Serial.begin(115200);

    while (1) {
        LoopbackTimestampNtf observed;
        UWB.onLoopbackUpdate(LOOPBACK_READY, &observed);

        uint32_t sum = 0;

        for (uint8_t i = 0; i < LOOPBACK_COUNT; i++) {
            UWB.startLoopback(ASAP);
            ASSERT(waitForEvent(2), LOOPBACK_READY);
            uint32_t d = observed.rxTimestamp - observed.txTimestamp;
            sum += d;
        }

        Serial.print("Antenna delay = %u\n", sum / LOOPBACK_COUNT);
        delay(3000);
    }
#endif

    // Retrieve the Anchor's address from device info
    GetDeviceInfoRsp info;
    UWB.getDeviceInfo(&info);
    // We will use 16 bytes from unique device identifier of type hex32
    addr16 = info.serialNumber & 0xFFFF;

#if SETUP_MODE
    Serial.begin(115200);

    Serial.print("===== TWR info =====\r\n");
    Serial.print("Short address: %04X\r\n", addr16);
    Serial.print("Role: %s\r\n", role == Twr_Role_Tag ? "Tag" : "Anchor");
    Serial.print("===== Dev info =====\r\n");
    Serial.print("Serial number: %08X\r\n", info.serialNumber);
    Serial.print("PHY version: %d.%d\r\n", info.phyMajorVersion, info.phyMinorVersion);
    Serial.print("Chip version: %d.%d\r\n", info.chipMajorVersion, info.chipMinorVersion);
    Serial.print("DSP version: %d.%d\r\n", info.dspMajorVersion, info.dspMinorVersion);
    Serial.print("ROM version: %d.%d\r\n", info.romMajorVersion, info.romMinorVersion);
    Serial.print("Baseband version: %d.%d\r\n", info.basebandMajorVersion, info.basebandMinorVersion);
    Serial.print("App version: %d.%d\r\n", info.appMajorVersion, info.appMinorVersion);
    Serial.print("Hardware id: %d\r\n", getHardwareId());
    delay(3000);
    reset();
#endif

    // Determine the role of the device in the ranging transaction
    if (get_anchor_index(addr16) == -1) {
        role = Twr_Role_Tag;
    } else {
        role = Twr_Role_Anchor;
    }

    // Set up the callbacks
    UWB.onRxUpdate(rx_isr, &rxResult, &rxPayload, sizeof(final_t), &rxFpi, &rxCirBuf);
    UWB.onTxUpdate((void (*)())TX_DONE, &txResult);

#if LED_INDICATION
    pinMode(PIN_LED_R | PIN_LED_G, OUTPUT_HIGH);
#endif
#if NLOS_DETECTION
    pinMode(NLOS_SENSOR_PIN, INPUT);
    pinMode(LOS_LED_PIN, OUTPUT_LOW);
#endif

    // Initialize the UART
    Serial.begin(BAUD_RATE);

    // Start the application by triggering the first superframe
    sf_start_isr();
}

/* *** Callbacks *** */
/* Should be called when TWR transaction fails for some reason */
void twr_fault_callback() {
    faultyTransactions++;

    if (faultyTransactions > (role == Twr_Role_Tag ? TAG_MAX_FAULTS : ANCHOR_MAX_FAULTS)) {
        reset();
    }
}

/* Callback for the transmission of the UWB messages. Role independent */
void twr_tx_callback() {
    switch (txState) {
        /* Depending on the current TX state, save the corresponding timestamp and
         * immediately enable the receiver to listen for the response */
        case Twr_Poll_Sent:
            pollTxTime = txResult.txTimestamp;

            UWB.stopAction();
            UciStatus ret = UWB.startRx(ASAP);
            if (ret != SUCCESS) {
#if ERROR_LOG
                Serial.print("Resp RX error: %d\r\n", ret);
#endif
            }
        break;
        case Twr_Resp_Sent:
            respTxTime = txResult.txTimestamp;

            UWB.stopAction();
            ret = UWB.startRx(ASAP);
            if (ret != SUCCESS) {
#if ERROR_LOG
                Serial.print("Final RX error: %d\r\n", ret);
#endif
            }
        break;
        case Twr_Final_Sent:
            /* The Final message finishes the ranging transaction
             * and is already timestamped. Do nothing */
            break;
        case Twr_Tx_Error:
#if ERROR_LOG
            Serial.print("Unexpected TX_ERROR state\r\n");
#endif
            twr_fault_callback();
            break;
    }
}

/* Anchor reception callback. On successful reception of the Response message,
 * corrects the next wakeup time and schedules the transmission of the Final message */
void anchor_rx_callback(const rx_packet_t *packet) {
    if (packet == NULL) {
        // Buffer underflow
        UWB.startRx(ASAP);
        return;
    }

    if (packet->rxStatus != SUCCESS) {
#if ERROR_LOG
        Serial.print("Error: packet status: %d\r\n", packet->rxStatus);
#endif
        UWB.startRx(ASAP);
        return;
    }

    if (packet->payload.resp.header.fCode != Twr_Fcode_Resp ||
        packet->payloadLength != sizeof(resp_t)) {
#if ERROR_LOG
        Serial.print("Invalid payload. Length: %d, fCode\r\n",
            packet->payloadLength, packet->payload.resp.header.fCode);
#endif
        // Invalid packet, or just not for us. In most cases, it is not an error
        return;
    }

    // Check if the Response message is intended for this Anchor. Not an error, just ignore
    if (packet->payload.resp.header.addr16 != addr16) {
        UWB.startRx(ASAP);
        return;
    }

    const resp_t *resp = &packet->payload.resp;
    respRxTime = packet->rxTimestamp;

    // We received a response, so timestamp the reception time and unpack the payload
    respFpi = packet->fpi.firstPathIndex;
    respReceived = true;
    faultyTransactions = 0;

    // According to the slot correction, calculate the time of the next Superframe start
    TWR_ENTER_CRITICAL();
    if (abs(resp->slotCorrection) < SF_PERIOD_US) {
        int64_t nextWakeUpPeriod = SF_PERIOD_US - (int64_t)resp->slotCorrection;
        if (nextWakeUpPeriod < SF_PERIOD_US / 2) {
            nextWakeUpPeriod += SF_PERIOD_US;
        }

        nextWakeUpTime = SFStartTime + nextWakeUpPeriod;
        Timer.clearOneShots();
        const ErrnoVal ret = Timer.atMicroseconds(nextWakeUpTime, sf_start_isr);
        if (ret != SUCCESS) {
            reset();
        }
    }
    TWR_EXIT_CRITICAL();

#if NLOS_DETECTION
    const uint8_t losSensorValue = digitalRead(NLOS_SENSOR_PIN);

    turn_led(LOS_LED_PIN, !losSensorValue);
#endif

    // Prepare the final message and schedule its transmission
    SetPayloadCmd reply;

    StartTxCmd send = {.time = *ticks() + MICROS_TO_TICKS(FINAL_SCHEDULE_DELAY_US)};

    finalTxTime = send.time;

    final_t *final = reply.payloadData;

    final->header.fCode = Twr_Fcode_Final;
    final->header.addr16 = addr16;
    final->rNum = resp->rNum;
#if LED_INDICATION
    final->sfNum = resp->sfNum;
#endif
    final->pollTxTime = pollTxTime;
    final->respRxTime = respRxTime;
    final->finalTxTime = finalTxTime;
    final->respFpi = respFpi;
#if NLOS_DETECTION
    final->losStatus = losSensorValue;
#endif

    /* If demanded, include the Response packet's CIR data in the Final message */
    size_t cirBytesCount = 0;

#if RESP_CIR
    const int16_t startIdx = LE_OFFSET + (packet->fpi.firstPathIndex - packet->fpi.edgeIdx);
    const int16_t *cirData = packet->cirBuf.cirData + (RX_FP_OFFSET - startIdx) * 2;
    cirBytesCount = delta_encode(cirData, final->cirData);
    final->cirBytesCount = cirBytesCount;
#endif

    size_t finalSize = sizeof(final_t);
    #if RESP_CIR
        finalSize -= sizeof(final->cirData) - cirBytesCount;
    #endif

    reply.payloadLength = finalSize;

    UWB.stopAction();

    // Send the Final message
    UWB.setPayload(&reply);

    txState = Twr_Final_Sent;

    const UciStatus ret = UWB.startTx(&send);
    if (ret != SUCCESS) {
#if ERROR_LOG
        Serial.print("Final TX error: %d\r\n", ret);
#endif
        txState = Twr_Tx_Error;
    } else {
#if LED_INDICATION
        if (resp->sfNum % 16 == 0) {
            turn_led(PIN_LED_R, resp->sfNum % 32 == 0);
        }
#endif
    }
}

/* Tag reception callback. On successful reception of:
 * - Poll: Calculate the slot correction and send the Response message
 * - Final: Calculate the TOF and report the result to the PC */
void tag_rx_callback(const rx_packet_t *packet) {
    if (packet == NULL) {
        // Buffer underflow
        UWB.startRx(ASAP);
        return;
    }

    if (packet->rxStatus != SUCCESS) {
#if ERROR_LOG
        Serial.print("Error: packet status: %d\r\n", packet->rxStatus);
#endif
        currentAnchorAddr16 = 0;

        UWB.startRx(ASAP);
        return;
    }

    if (packet->payloadLength != sizeof(poll_t)) {
        size_t finalSize = sizeof(final_t);
#if RESP_CIR
            /* The actual size of the Final message is known only at runtime.
             * Check if that size is at least expected */
            finalSize -= sizeof(packet->payload.final.cirData) - packet->payload.final.cirBytesCount;
#endif

        if (packet->payloadLength != finalSize) {
#if ERROR_LOG
            Serial.print("Invalid payload. Length: %d, fCode: %d\r\n",
                packet->payloadLength, packet->payload.final.header.fCode);
#endif
            // Invalid packet, or just not for us. In most cases, it is not an error
            return;
        }
    }

    const uint8_t f_code = packet->payload.poll.header.fCode;

    switch (f_code) {
        case Twr_Fcode_Poll:
            const poll_t *poll = &packet->payload.poll;
            pollRxTime = packet->rxTimestamp;

            pollFpi = packet->fpi.firstPathIndex;
            pollReceived = true;
            faultyTransactions = 0;

            currentAnchorAddr16 = poll->header.addr16;

            // Prepare the Response message and transmit immediately
            SetPayloadCmd reply = {.payloadLength = sizeof(resp_t)};

            /* Calculate the slot correction based on the time of the Superframe start
             * and the time of the Poll message reception:
             * - calculate the time passed since the start of the Superframe
             * - subtract the time of expected start of the slot corresponding to the Poll message sender */
            const int32_t slotCorrection = TICKS_TO_MICROS(pollRxTime) -
                SFStartTime - get_slot_time_us(poll->header.addr16);

            lastSlotCorrection = slotCorrection;
            rangeNumber = poll->rNum;

            resp_t *resp = reply.payloadData;

            resp->header.fCode = Twr_Fcode_Resp;
            resp->header.addr16 = poll->header.addr16;
            resp->rNum = poll->rNum;
#if LED_INDICATION
            resp->sfNum = superframeNum;
#endif
            resp->slotCorrection = slotCorrection;

            UWB.stopAction();

            // Send a Response message
            UWB.setPayload(&reply);

            txState = Twr_Resp_Sent;

            ErrnoVal ret = UWB.startTx(ASAP);
            if (ret != SUCCESS) {
#if ERROR_LOG
                Serial.print("Resp TX error: %d\r\n", ret);
#endif
                txState = Twr_Tx_Error;

                currentAnchorAddr16 = 0;
                UWB.startRx(ASAP);
            }
            break;
        case Twr_Fcode_Final:
            const final_t *final = &packet->payload.final;
            finalRxTime = packet->rxTimestamp;

            if (currentAnchorAddr16 != final->header.addr16 || final->rNum != rangeNumber) {
                UWB.startRx(ASAP);
                return;
            }

            finalFpi = packet->fpi.firstPathIndex;
            twrResult = Twr_OK;

            // Call the final callback to calculate the distance and report the result
            tag_received_final(packet);

#if LED_INDICATION
        if (final->sfNum % 16 == 0) {
            turn_led(PIN_LED_G, final->sfNum % 32 == 0);
        }
#endif

            UWB.stopAction();

            ret = UWB.startRx(ASAP);
            if (ret != SUCCESS) {
#if ERROR_LOG
                Serial.print("Poll RX error (mid): %d\r\n", ret);
#endif
            }
            break;
        default:
            currentAnchorAddr16 = 0;
            /* If the Anchor is performing a TX, then the receiver will be enabled
             * automatically by the twr_tx_callback() function.
             * Otherwise, we re-enable the receiver manually */
            UWB.startRx(ASAP);
            break;
    }
}

/* Polling task of the Anchor. Callback for the start of the Anchor's superframe. */
void anchor_send_poll() {
    // Prepare Poll message payload
    SetPayloadCmd payloadCmd = {.payloadLength = sizeof(poll_t)};

    poll_t *poll = payloadCmd.payloadData;

    poll->header.fCode = Twr_Fcode_Poll;
    poll->header.addr16 = addr16;
    poll->rNum = rangeNumber;

    UWB.stopAction();

    // Transmit Poll message
    UWB.setPayload(&payloadCmd);

    txState = Twr_Poll_Sent;
    rangeNumber++;

    const UciStatus ret = UWB.startTx(ASAP);
    if (ret != SUCCESS) {
#if ERROR_LOG
        Serial.print("Poll TX error: %d. rn: %d\r\n", ret, rangeNumber);
#endif
        txState = Twr_Tx_Error;
    }
}

/* Callback for the reception of the Final message. Calculates the time
 * of flight, infers the distance, and sends the data to the PC */
void tag_received_final(const rx_packet_t *finalPacket) {
    // const uint16_t anchorAntDelay = anchorAntDelays[getAnchorIndex(final->header.addr16)];
    // TODO: Check the correctness of the anchorAntDelay and use it in the distance calculation
    const final_t *final = &finalPacket->payload.final;

    const uint64_t round1 = final->respRxTime - final->pollTxTime - ANT_DELAY;
    const uint64_t round2 = finalRxTime - respTxTime - ANT_DELAY;
    const uint64_t reply1 = respTxTime - pollRxTime + ANT_DELAY;
    const uint64_t reply2 = final->finalTxTime - final->respRxTime + ANT_DELAY;

    const float r1r2rp1rp2 = (float)round1 * (float)round2 - (float)reply1 * (float)reply2;
    const float r1r2rp1rp2_sum = (float)round1 + (float)round2 + (float)reply1 + (float)reply2;

    const int64_t tProp = r1r2rp1rp2 / r1r2rp1rp2_sum;
    const int64_t distance = ((307389 * tProp) / 655360) - DIST_OFFSET_CM; // In cm

    /* Filter out obviously wrong distances */
    if (distance > MAX_DIST_M * 100 || distance < 0) {
        twrResult = Twr_Incorrect_Range;
    }

    /* Now, filter out distances that are the result of the incorrect timestamping.
     *
     * The root cause of this remains unknown, but we know how to detect it:
     * All the transactions that contain the incorrect timestamping have abnormally
     * small detected index of a first path. It is statistically determined that the
     * percentage of such transactions is around 0.1%. We, therefore, filter out
     * all the transactions that have the first path index less than 0.0005 quantile
     * of the first path index distribution. This approach leads to the sensitivity
     * of 100% and the specificity of 99.8%. The 0.0005 quantile is 530.
     */
    if (pollFpi < 530
        || final->respFpi < 530
        || finalFpi < 530) {
        twrResult = Twr_Incorrect_Timestamping;
    }

    uwbLogMessage_t msg = {
        .data = {
            .anchor = final->header.addr16,
            .timestamp = micros(),
            .distance = distance,
            .rangeNumber = final->rNum,
            .status = twrResult,
#if NLOS_DETECTION
            .losStatus = final->losStatus,
#endif

            // Can be used for diagnostics
            // .transactionTime = transactionTime,
            // .lastSlotCorrection = lastSlotCorrection
        }
    };

    uint8_t checksum = 0x55;
    for (int i = 0; i < sizeof(uwbLogMessageData_t); i++) {
        checksum ^= ((uint8_t *)&msg.data)[i];
    }
    msg.checksum = checksum;

#if SEND_CIR
    /* CIR readout.
     * Depending on the configuration, the CIR data is either
     * included in the Final message or read from the RX buffer */
    #if RESP_CIR
        msg.cirBytesCount = final->cirBytesCount;
        memcpy(msg.cirData, final->cirData, final->cirBytesCount);
    #else
        int16_t startIdx = LE_OFFSET + (finalPacket->fpi.firstPathIndex - finalPacket->fpi.edgeIdx);
        const int16_t *cirData = finalPacket->cirBuf.cirData + (RX_FP_OFFSET - startIdx) * 2;
        #if USE_ENCODING
            msg.cirBytesCount = delta_encode(cirData, msg.cirData);
        #else
            msg.cirBytesCount = CIR_SAMPLE_CNT * CIR_SAMPLE_BYTES;
            memcpy(msg.cirData, cirData, CIR_SAMPLE_CNT * CIR_SAMPLE_BYTES);
        #endif
    #endif
#endif

    send_bin_to_pc(&msg);
    // transactionTime = micros() - TICKS_TO_MICROS(pollRxTime); // Can be used to adjust slot duration
}

/* General callback for the start of the superframe. Used in both Anchor and Tag roles */
void sf_start_callback() {
    // Schedule the next superframe start
    nextWakeUpTime = SFStartTime + SF_PERIOD_US;
    Timer.atMicroseconds(nextWakeUpTime, sf_start_isr);

    // Check if the device received something during the last Superframe
    bool *reception_flag = role == Twr_Role_Tag ? &pollReceived : &respReceived;
    if (!*reception_flag) {
        twr_fault_callback();
    } else {
        *reception_flag = false;
    }

    superframeNum++;
}

/* *** Main event loop *** */
void loop() {
    /* Wait until any triggerEvent() is called. We set the timeout
     * to twice the superframe period in order to detect any system
     * failures, potentially caused by missed wake-up events */
    uint32_t event = waitForEvent(SF_PERIOD_US * 2 / 1000);

    if (event == EVENT_TIMEOUT) {
        /* This should never happen, but if it does, the device
         * is in unrecoverable state and should be reset */
#if ERROR_LOG
        Serial.print("Hard timeout. Resetting...\r\n");
#endif
        reset();
    }

    if (event & WAKEUP_SF_START) {
        sf_start_callback();
        if (role == Twr_Role_Anchor) {
            anchor_send_poll();
        }

        event &= ~WAKEUP_SF_START;
    }

    if (event & TX_DONE) {
        twr_tx_callback();
        event &= ~TX_DONE;
    }

    if (event & RX_DONE) {
        const rx_packet_t *packet;

        while ((packet = get_rx_packet()) != NULL) {
            switch (role) {
                case Twr_Role_Anchor:
                    anchor_rx_callback(packet);
                    break;
                case Twr_Role_Tag:
                    tag_rx_callback(packet);
                    break;
                default: ;
            }

            remove_rx_packet();
        }

        event &= ~RX_DONE;
    }

    if (event != 0) {
#if ERROR_LOG
        Serial.print("Unexpected event: %d\r\n", event);
#endif
    }
}

/* *** Notes *** */
/* Double-sided (DS) Two-way ranging (TWR) algorithm.
 *
 * The DS-TWR algorithm is used to calculate the time of flight (TOF) between two devices.
 * The algorithm consists of three messages (UWB packets):
 * 1. Poll message: The initiator sends a Poll message to the responder and timestamps the transmission time.
 * 2. Response message: The responder receives the Poll message, timestamps the reception time,
 * and sends the Response message, again timestamping the transmission time.
 * 3. Final message: The initiator receives the Response message, timestamps the reception time,
 * and schedules the transmission of the Final message. In the payload of the Final message,
 * the initiator includes its local timestamps: Poll TX time, Response RX time, and Final TX time.
 * 4. The responder receives the Final message, timestamps the reception time, and calculates the TOF:
 *
 * The TOF is calculated as follows (taking into account device's antenna delay):
 * TRound1 = Response RX time - Poll TX time (initiator time domain)
 * TRound2 = Final RX time - Response TX time (responder time domain)
 * TReply1 = Response TX time - Poll RX time (responder time domain)
 * TReply2 = Final TX time - Response RX time (initiator time domain)
 *
 * TOF = (TRound1 * TRound2 - TReply1 * TReply2) / (TRound1 + TRound2 + TReply1 + TReply2)
 *
 * The distance is inferred from the TOF using the speed of light in air and known clock resolution.
 */
