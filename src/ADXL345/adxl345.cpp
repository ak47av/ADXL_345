#include "adxl345.h"
#include <iostream>
#include <cstdio>
#include <math.h>

using namespace std;

ADXL345::ADXL345(unsigned int bus, unsigned int device) : EE513::I2CDevice(bus,device) 
{
    this->setPowerMode();
}

void ADXL345::setPowerMode()
{
    this->writeRegister(REG_POWER_CTL, MODE_MEASUREMENT);
}

void ADXL345::setOffsets(int8_t x, int8_t y, int8_t z)
{
    this->writeRegister(REG_OFSX, x);
    this->writeRegister(REG_OFSY, y);
    this->writeRegister(REG_OFSZ, z);
}

void ADXL345::calibrateSensor()
{
    acc_xyz values;
    int32_t sumx = 0, sumy = 0, sumz = 0;
    int num_readings = 1000;
    for(int i=0; i<num_readings; i++)
    {
        values = this->readXYZ();
        sumx += values.acc_x;
        sumy += values.acc_y;
        sumz += values.acc_z;
        sleep(0.1);
    }
    int8_t avgx = sumx/num_readings;
    int8_t avgy = sumy/num_readings;
    int8_t avgz = sumz/num_readings;

    int8_t off_x = 0 - round((double) avgx);
    int8_t off_y = 0 - round((double) avgy);
    int8_t off_z = 0 - round((double) (avgz+256));

    cout << "Offsets(x,y,z): " << (int)off_x << "," << (int)off_y << "," << (int)off_z << "," << endl;
    this->setOffsets(avgx, avgy, avgz);
}

acc_xyz ADXL345::readXYZ()
{
    acc_xyz values;
    uint8_t *val = this->readRegisters(6, REG_DATAX0);
    values.acc_x = ((val[1] << 8) | val[0]);
    values.acc_y = ((val[3] << 8) | val[2]);
    values.acc_z = ((val[5] << 8) | val[4]);
    return values;
}