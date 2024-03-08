#ifndef ADXL345_H_
#define ADXL345_H_

#include "../I2C/I2CDevice.h"
#include <unistd.h>
#include <cstdint>

#define REG_DEVICE_ID           0x00
#define REG_THRESH_TAP          0x1D
#define REG_OFSX                0x1E
#define REG_OFSY                0x1F
#define REG_OFSZ                0x20
#define REG_TAP_DURATION        0x21
#define REG_TAP_LATENCY         0x22
#define REG_TAP_WINDOW          0x23
#define REG_THRESH_ACT          0x24
#define REG_THRESH_INACT        0x25
#define REG_TIME_INACT          0x26
#define REG_ACT_INACT_CTL       0x27
#define REG_THRESH_FF           0x28
#define REG_TIME_FF             0x29
#define REG_TAP_AXES            0x2A
#define REG_ACT_TAP_STATUS      0x2B
#define REG_BW_RATE             0x2C
#define REG_POWER_CTL           0x2D
#define REG_INT_ENABLE          0x2E 
#define REG_INT_MAP             0x2F
#define REG_INT_SOURCE          0x30
#define REG_DATA_FORMAT         0x31
#define REG_DATAX0              0x32
#define REG_DATAX0              0x32
#define REG_DATAX1              0x33
#define REG_DATAY0              0x34
#define REG_DATAY1              0x35
#define REG_DATAZ0              0x36
#define REG_DATAZ1              0x37
#define REG_FIFO_CTL            0x38
#define REG_FIFO_STATUS         0x39

#define MODE_MEASUREMENT 0x8

typedef struct acc_xyz {
    int16_t acc_x;
    int16_t acc_y;
    int16_t acc_z;
} acc_xyz;

class ADXL345: public EE513::I2CDevice{
    private:

    public:
        ADXL345(unsigned int bus, unsigned int device);
        void setPowerMode();
        void calibrateSensor();
        void setOffsets(int8_t x, int8_t y, int8_t z);
        acc_xyz readXYZ();
};

#endif