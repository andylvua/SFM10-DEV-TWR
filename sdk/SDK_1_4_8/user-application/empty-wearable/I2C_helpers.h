
#ifndef I2C_HELPERS_H
#define I2C_HELPERS_H

#define BOUND_RANGE(var, lower, upper) \
    if (var < lower) { \
        var = lower; \
    } else if (var > upper) { \
        var = upper; \
    }

#define MAP_RANGE(var, start, step) \
    var = (var - start) / step;

#define BOUND_AND_MAP_RANGE(var, lower, upper, step) \
    BOUND_RANGE(var, lower, upper) \
    MAP_RANGE(var, lower, step)


#define B7 (1<<7)
#define B6 (1<<6)
#define B5 (1<<5)
#define B4 (1<<4)
#define B3 (1<<3)
#define B2 (1<<2)
#define B1 (1<<1)
#define B0 (1<<0)



#define CAT_I(a,b) a##b
#define CAT(a,b) CAT_I(a, b)


#define I2C_WRITE_FUN(name, i2c_address) \
int8_t CAT(name, _write)(uint8_t addr, uint8_t value) { \
    uint8_t buffer[2] = { addr, value }; \
    return I2C.write(i2c_address, buffer, 2, true); \
}


#define I2C_READ_FUN(name, i2c_address) \
uint8_t CAT(name, _read)(uint8_t addr) { \
    uint8_t buffer[1] = { addr }; \
    ASSERT_0(I2C.write(i2c_address, buffer, 1, false)); \
    ASSERT(I2C.read(i2c_address, buffer, 1, true), 1); \
    return buffer[0]; \
} 


#define I2C_READ_BYTES_FUN(name, i2c_address) \
void CAT(name, _readBytes)(uint8_t addr, uint8_t* buffer, uint16_t length) { \
    buffer[0] = addr; \
    ASSERT_0(I2C.write(i2c_address, buffer, 1, false)); \
    ASSERT(I2C.read(i2c_address, buffer, length, true), length); \
}


#define I2C_FUN(name, i2c_address) \
    I2C_WRITE_FUN(name, i2c_address) \
    I2C_READ_FUN(name, i2c_address) \
    I2C_READ_BYTES_FUN(name, i2c_address) \




#endif