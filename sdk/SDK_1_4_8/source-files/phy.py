
# see: documentation/UCI-for-Host.html

DEVICE_CMD = 0x20
DEVICE_RSP = 0x40
DEVICE_NTF = 0x60

DEVICE_RESET_DEVICE = 0x00
DEVICE_RESET_DEVICE_SIZE = 0

DEVICE_DEVICE_STATUS_NTF = 0x01
DEVICE_DEVICE_STATUS_NTF_SIZE = 0

DEVICE_GET_DEVICE_INFO = 0x02
DEVICE_GET_DEVICE_INFO_SIZE = 0

DEVICE_ENTER_POWER_MODE = 0x21
DEVICE_ENTER_POWER_MODE_SIZE = 9

DEVICE_SET_PERSISTENT_REG = 0x23
DEVICE_SET_PERSISTENT_REG_SIZE = 4

DEVICE_GET_PERSISTENT_REG = 0x24
DEVICE_GET_PERSISTENT_REG_SIZE = 0

DEVICE_OTA_UPDATE_RECEIVE = 0x31
DEVICE_OTA_UPDATE_RECEIVE_SIZE = 12

DEVICE_OTA_UPDATE_TRANSMIT = 0x32
DEVICE_OTA_UPDATE_TRANSMIT_SIZE = 13

CONFIGURATION_CMD = 0x21
CONFIGURATION_RSP = 0x41
CONFIGURATION_NTF = 0x61

CONFIGURATION_SET_CONFIGURATION = 0x04
CONFIGURATION_SET_CONFIGURATION_SIZE = 50

CONFIGURATION_GET_CONFIGURATION = 0x05
CONFIGURATION_GET_CONFIGURATION_SIZE = 0

CONFIGURATION_STORE_RX_RADIO_CONFIG = 0x25
CONFIGURATION_STORE_RX_RADIO_CONFIG_SIZE = 513

CONFIGURATION_STORE_TX_RADIO_CONFIG = 0x26
CONFIGURATION_STORE_TX_RADIO_CONFIG_SIZE = 257

CONFIGURATION_IMPORT_KEY = 0x22
CONFIGURATION_IMPORT_KEY_SIZE = 18

CONFIGURATION_GET_TICKS = 0x02
CONFIGURATION_GET_TICKS_SIZE = 0

CONFIGURATION_SET_PAYLOAD = 0x03
CONFIGURATION_SET_PAYLOAD_SIZE = 1022

ACTIONS_CMD = 0x22
ACTIONS_RSP = 0x42
ACTIONS_NTF = 0x62

ACTIONS_BEGIN = 0x00
ACTIONS_BEGIN_SIZE = 0

ACTIONS_START_TX = 0x04
ACTIONS_START_TX_SIZE = 8

ACTIONS_START_RX = 0x05
ACTIONS_START_RX_SIZE = 8

ACTIONS_START_CONTINUOUS_RX = 0x06
ACTIONS_START_CONTINUOUS_RX_SIZE = 8

ACTIONS_START_LOOPBACK = 0x07
ACTIONS_START_LOOPBACK_SIZE = 8

ACTIONS_RECALC_RX = 0x09
ACTIONS_RECALC_RX_SIZE = 1

ACTIONS_STOP_ACTION = 0x08
ACTIONS_STOP_ACTION_SIZE = 0

ACTIONS_GET_CIR = 0x17
ACTIONS_GET_CIR_SIZE = 3

ACTIONS_TX_TIMESTAMP_NTF = 0x21
ACTIONS_TX_TIMESTAMP_NTF_SIZE = 0

ACTIONS_RX_TIMESTAMP_NTF = 0x22
ACTIONS_RX_TIMESTAMP_NTF_SIZE = 0

ACTIONS_LOOPBACK_TIMESTAMP_NTF = 0x23
ACTIONS_LOOPBACK_TIMESTAMP_NTF_SIZE = 0

ACTIONS_PAYLOAD_NTF = 0x25
ACTIONS_PAYLOAD_NTF_SIZE = 0

ACTIONS_FIRST_PATH_INFO_NTF = 0x26
ACTIONS_FIRST_PATH_INFO_NTF_SIZE = 0

ACTIONS_CIR_NTF = 0x27
ACTIONS_CIR_NTF_SIZE = 0

ACTIONS_END = 0x01
ACTIONS_END_SIZE = 0


byte1_device = dict([
  (DEVICE_RESET_DEVICE, "reset device"),
  (DEVICE_DEVICE_STATUS_NTF, "device status ntf"),
  (DEVICE_GET_DEVICE_INFO, "get device info"),
  (DEVICE_ENTER_POWER_MODE, "enter power mode"),
  (DEVICE_SET_PERSISTENT_REG, "set persistent reg"),
  (DEVICE_GET_PERSISTENT_REG, "get persistent reg"),
  (DEVICE_OTA_UPDATE_RECEIVE, "ota update receive"),
  (DEVICE_OTA_UPDATE_TRANSMIT, "ota update transmit"),
])

byte1_configuration = dict([
  (CONFIGURATION_SET_CONFIGURATION, "set configuration"),
  (CONFIGURATION_GET_CONFIGURATION, "get configuration"),
  (CONFIGURATION_STORE_RX_RADIO_CONFIG, "store rx radio config"),
  (CONFIGURATION_STORE_TX_RADIO_CONFIG, "store tx radio config"),
  (CONFIGURATION_IMPORT_KEY, "import key"),
  (CONFIGURATION_GET_TICKS, "get ticks"),
  (CONFIGURATION_SET_PAYLOAD, "set payload"),
])

byte1_actions = dict([
  (ACTIONS_BEGIN, "begin"),
  (ACTIONS_START_TX, "start tx"),
  (ACTIONS_START_RX, "start rx"),
  (ACTIONS_START_CONTINUOUS_RX, "start continuous rx"),
  (ACTIONS_START_LOOPBACK, "start loopback"),
  (ACTIONS_RECALC_RX, "recalc rx"),
  (ACTIONS_STOP_ACTION, "stop action"),
  (ACTIONS_GET_CIR, "get cir"),
  (ACTIONS_TX_TIMESTAMP_NTF, "tx timestamp ntf"),
  (ACTIONS_RX_TIMESTAMP_NTF, "rx timestamp ntf"),
  (ACTIONS_LOOPBACK_TIMESTAMP_NTF, "loopback timestamp ntf"),
  (ACTIONS_PAYLOAD_NTF, "payload ntf"),
  (ACTIONS_FIRST_PATH_INFO_NTF, "first path info ntf"),
  (ACTIONS_CIR_NTF, "cir ntf"),
  (ACTIONS_END, "end"),
])

byte1 = dict([
    (DEVICE_CMD, byte1_device),
    (DEVICE_RSP, byte1_device),
    (DEVICE_NTF, byte1_device),
    (CONFIGURATION_CMD, byte1_configuration),
    (CONFIGURATION_RSP, byte1_configuration),
    (CONFIGURATION_NTF, byte1_configuration),
    (ACTIONS_CMD, byte1_actions),
    (ACTIONS_RSP, byte1_actions),
    (ACTIONS_NTF, byte1_actions),
])

byte0 = dict([
    (DEVICE_CMD,"device command"),
    (DEVICE_RSP,"device response"),
    (DEVICE_NTF,"device notification"),
    (CONFIGURATION_CMD,"configuration command"),
    (CONFIGURATION_RSP,"configuration response"),
    (CONFIGURATION_NTF,"configuration notification"),
    (ACTIONS_CMD,"actions command"),
    (ACTIONS_RSP,"actions response"),
    (ACTIONS_NTF,"actions notification"),
])

byte4 = dict([
    (0,"success"),
    (0x01,"rejected"),
    (0x02,"failed"),
    (0x03,"syntax error"),
    (0x04,"invalid param"),
    (0x05,"invalid range"),
    (0x06,"invalid message size"),
    (0x07,"unknown gid"),
    (0x08,"unknown oid"),
    (0x09,"read only"),
    (0x0A,"command retry"),
    (0x11,"error session not exist"),
    (0x12,"error session duplicate"),
    (0x13,"error session active"),
    (0x14,"error max sessions exceeded"),
    (0x15,"error session not configured"),
    (0x16,"error active sessions ongoing"),
    (0x17,"error multicast list full"),
    (0x18,"error address not found"),
    (0x19,"error address already present"),
    (0x1A,"slot len not supported"),
    (0x1B,"invalid slot per rr"),
    (0x1C,"sts not configured"),
    (0x20,"ranging tx failed"),
    (0x21,"ranging rx timeout"),
    (0x22,"ranging rx phy dec failed"),
    (0x23,"ranging rx phy toa failed"),
    (0x24,"ranging rx phy sts failed"),
    (0x25,"ranging rx mac dec failed"),
    (0x26,"ranging rx mac ie dec failed"),
    (0x27,"ranging rx mac ie missing"),
    (0x35,"param too high"),
    (0x36,"param too low"),
    (0x37,"payload too big"),
    (0x38,"scheduled too early"),
    (0x39,"scheduled too late"),
    (0x51,"general error"),
    (0x55,"bad unit number"),
    (0x56,"bad instance"),
    (0x57,"bad handle"),
    (0x58,"bad index"),
    (0x59,"bad parameter"),
    (0x5A,"no instances"),
    (0x5B,"no component"),
    (0x5C,"no resources"),
    (0x5D,"instance in use"),
    (0x5E,"resource owned"),
    (0x5F,"resource not owned"),
    (0x60,"inconsistent params"),
    (0x61,"not initialized"),
    (0x62,"not enabled"),
    (0x63,"not supported"),
    (0x64,"init failed"),
    (0x65,"busy"),
    (0x66,"not busy"),
    (0x67,"read"),
    (0x68,"write"),
    (0x69,"erase"),
    (0x6A,"lock"),
    (0x6B,"unlock"),
    (0x6C,"out of memory"),
    (0x6D,"bad virt address"),
    (0x6E,"bad phys address"),
    (0x6F,"timeout"),
    (0x70,"overflow"),
    (0x71,"full"),
    (0x72,"empty"),
    (0x73,"not started"),
    (0x74,"already started"),
    (0x75,"not stopped"),
    (0x76,"already stopped"),
    (0x77,"already setup"),
    (0x78,"null parameter"),
    (0x79,"null datainfunc"),
    (0x7A,"null dataoutfunc"),
    (0x7B,"null controlfunc"),
    (0x7C,"null completionfunc"),
    (0x7D,"null progressfunc"),
    (0x7E,"null errorfunc"),
    (0x7F,"null memallocfunc"),
    (0x80,"null memfreefunc"),
    (0x81,"null configfunc"),
    (0x82,"null parent"),
    (0x83,"null iodesc"),
    (0x84,"null ctrldesc"),
    (0x85,"unsupported dataclass"),
    (0x86,"unsupported datatype"),
    (0x87,"unsupported datasubtype"),
    (0x88,"format"),
    (0x89,"input desc flags"),
    (0x8A,"output desc flags"),
    (0x8B,"cap required"),
    (0x8C,"bad tmalfunc table"),
    (0x8D,"invalid channel id"),
    (0x8E,"invalid command"),
    (0x8F,"stream mode confusion"),
    (0x90,"underrun"),
    (0x91,"empty packet recvd"),
    (0x92,"other datainout err"),
    (0x93,"stop requested"),
    (0x94,"lpuart rx hw overrun"),
    (0x95,"lpuart noise error"),
    (0x96,"lpuart framing error"),
    (0x97,"lpuart parity error"),
    (0x98,"lpuart flag no manual clear"),
    (0x99,"assertion"),
    (0x9A,"highway bandwidth"),
    (0x9B,"hw reset failed"),
    (0x9D,"bad flags"),
    (0x9E,"bad priority"),
    (0x9F,"bad reference count"),
    (0xA0,"bad setup"),
    (0xA1,"bad stack size"),
    (0xA2,"idle"),
    (0xA3,"null packet"),
    (0xA4,"page flash invld page addr param"),
    (0xA5,"page flash cmd execution failed"),
    (0xA6,"page flash operation timeout"),
    (0xA7,"page flash write word cmd failed"),
    (0xA8,"page flash program page cmd failed"),
    (0xA9,"page flash erase page range failed"),
    (0xAA,"ecc"),
    (0xAB,"page flash margin check failed"),
    (0xAC,"page flash blank check failed"),
    (0xAD,"page flash page corrupted"),
    (0xAE,"page flash page programmed"),
    (0xAF,"page flash page erased"),
    (0xB0,"page flash checksum page cmd failed"),
    (0xB1,"page flash erase page failed"),
    (0xB2,"page flash invld word addr param"),
    (0xB3,"page flash set read mode error"),
    (0xB4,"page flash init failed"),
    (0xB5,"page repair"),
    (0xB6,"imagecheck failed"),
    (0xB7,"invalid state"),
    (0xB8,"operation not permitted"),
    (0xB9,"not clocked"),
    (0xBA,"invalid psdu len"),
    (0xBB,"invalid timeout time"),
    (0xBC,"concurrent access"),
    (0xBD,"store amu"),
    (0xBE,"remap amu"),
    (0xBF,"init amu"),
    (0xC0,"page margin verify"),
    (0xC1,"amu disabled"),
    (0xC2,"compatibility"),
    (0xC3,"trim with hardware defaults"),
    (0xC4,"crc"),
    (0xC5,"flash page copy"),
    (0xC6,"page flash report ecc log error"),
    (0xC7,"rs undecodable"),
    (0xC8,"rs illegal erasures num"),
    (0xC9,"rs illegal erasures location"),
    (0xCA,"rs illegal params"),
    (0xCB,"secded undecodable"),
    (0xCC,"toa undetected"),
    (0xCD,"verification failed"),
    (0xCE,"invalid cfg"),
    (0xCF,"no trim data err"),
    (0xD1,"authent fail"),
    (0xD2,"call from ISR not allowed"),
    (0xF8,"crc error"),
    (0xFE,"not implemented"),
])