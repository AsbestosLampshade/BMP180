#ifndef BMP180_H_INCLUDED
#define BMP180_H_INCLUDED
//A4(SDA), A5(SCL)
#include <Wire.h>
#include <math.h>  //For pow
#include <Arduino.h>  //For Serial Prints etc.

#define BMP180_WR_ADDR 0x77
#define BMP180_RD_ADDR 0xF7

#define EEPROM_ADDR 0xAA
#define MSB_ADDR 0xF6
#define LSB_ADDR 0xF7
#define XLSB_ADDR 0xF8

#define BMP180_CALIB_EEPROM_SIZEW 11
#define BMP_180_CALIB_EEPROM_SIZEB 22

enum calib_reg_t{CALIB_AC1=0,CALIB_AC2,CALIB_AC3,CALIB_AC4,CALIB_AC5,CALIB_AC6,CALIB_B1,CALIB_B2,CALIB_MB,CALIB_MC,CALIB_MD};

struct C_reg{
  int AC1,AC2,AC3;
  unsigned int AC4,AC5,AC6;
  int C_B1,C_B2,MB,MC,MD;
};

union eeprom_t{
  word calibW[BMP180_CALIB_EEPROM_SIZEW];
  byte calibB[BMP_180_CALIB_EEPROM_SIZEB];
  struct C_reg calibReg;
};

class BMP_t{
  private:
    eeprom_t calibParam;
    double B5;  
  public:
    BMP_t();
    double getPressure();
    double getTemperature();
};

void BEtoLEConvert(word&);

#endif
