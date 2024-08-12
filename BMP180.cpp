#include "BMP180.h"

byte UP[]={0xF4,0x34};

BMP_t::BMP_t(){
  Wire.begin(); //Initialization Delay
  delay(100);
  byte ePromAddr = EEPROM_ADDR;
  //Reading 22 bytes from the EEPROM
  for(int i=0;i<22;i++){
    Wire.beginTransmission(BMP180_WR_ADDR);
    Wire.write(ePromAddr);
    Wire.endTransmission(true);
    Wire.requestFrom(BMP180_RD_ADDR,1);
    calibParam.calibB[i]=Wire.read();
    ePromAddr++;
  }
  for(int i=0;i<11;i++){
    BEtoLEConvert(calibParam.calibW[i]);
  }
}
#define BMP180_UP_REG  0xF4
#define BMP180_OSS 0
#define BMP180_UP_DATA (0x34+(BMP180_OSS<<6))
double BMP_t::getPressure(){
  Wire.beginTransmission(BMP180_WR_ADDR);
  Wire.write(BMP180_UP_REG);
  Wire.write(BMP180_UP_DATA);
  Wire.endTransmission(true);
  delay(50);

  Wire.beginTransmission(BMP180_WR_ADDR);
  Wire.write(MSB_ADDR);
  Wire.endTransmission(true);
  Wire.requestFrom(BMP180_RD_ADDR,1);
  byte PressMSB= Wire.read();

  Wire.beginTransmission(BMP180_WR_ADDR);
  Wire.write(LSB_ADDR);
  Wire.endTransmission(true);
  Wire.requestFrom(BMP180_RD_ADDR,1);
  byte PressLSB= Wire.read();
  
  Wire.beginTransmission(BMP180_WR_ADDR);
  Wire.write(XLSB_ADDR);
  Wire.endTransmission(true);
  Wire.requestFrom(BMP180_RD_ADDR,1);
  byte PressXLSB= Wire.read();

  long UPval = PressMSB;
  UPval=UPval<<8;
  UPval=UPval|PressLSB;
  UPval=UPval<<8;
  UPval=UPval|PressXLSB;
  UPval=UPval>>(8-BMP180_OSS);
  
  double UPval_f = UPval;
  getTemperature();
  double B6=B5-(double)4000;
  double X1=(calibParam.calibReg.C_B2*(B6*B6)/pow(2,12))/pow(2,11);
  double X2=calibParam.calibReg.AC2*B6/pow(2,11);
  double X3=X1+X2;
  double B3=((((calibParam.calibReg.AC1*4)+X3)*pow(2,BMP180_OSS))+(double)2)/(double)4;
  X1 = calibParam.calibReg.AC3*B6/pow(2,13);
  X2 = (calibParam.calibReg.C_B1*(B6*B6/pow(2,12)))/pow(2,16);
  X3 = ((X1+X2)+(double)2)/(double)4;
  double B4= calibParam.calibReg.AC4*(unsigned long)(X3+(double)32768)/pow(2,15);
  double B7=((unsigned long)(UPval_f-B3))*(50000>>BMP180_OSS);
  double p;
  if(B7<0x80000000){
    p= (B7*2.0)/B4;
  }
  else{
    p=(B7/B4)*2;
  }
  X1=(p/pow(2,8))*(p/pow(2,8));
  X1=(X1*3038.0)/pow(2,16);
  X2=(-7357.0*p)/pow(2,16);
  p=p+(X1+X2+3791.0)/pow(2,4);
  return p;
}

#define BMP180_UT_REG  0xF4
#define BMP180_UT_DATA 0x2E
double BMP_t::getTemperature(){
  Wire.beginTransmission(BMP180_WR_ADDR);
  Wire.write(BMP180_UT_REG);
  Wire.write(BMP180_UT_DATA);
  Wire.endTransmission(true);
  delay(50);

  Wire.beginTransmission(BMP180_WR_ADDR);
  Wire.write(MSB_ADDR);
  Wire.endTransmission(true);
  Wire.requestFrom(BMP180_RD_ADDR,1);
  byte tempMSB= Wire.read();

  Wire.beginTransmission(BMP180_WR_ADDR);
  Wire.write(LSB_ADDR);
  Wire.endTransmission(true);
  Wire.requestFrom(BMP180_RD_ADDR,1);
  byte tempLSB= Wire.read();

  uint16_t UTval=tempMSB;
  UTval=UTval<<8;
  UTval=UTval|tempLSB;


  double X1=((double)UTval-calibParam.calibReg.AC6)*calibParam.calibReg.AC5/pow(2,15);
  double X2=calibParam.calibReg.MC*pow(2,11)/(X1+(double)calibParam.calibReg.MD);

  B5 = X1+ X2;
  double trueTemp=(B5+8)/pow(2,4);

  return trueTemp;
}

void BEtoLEConvert(word& wBE){
  word temp=wBE;
  word wLE = wBE<<8;
  wLE= wLE | (temp>>8);
  wBE=wLE;
}
