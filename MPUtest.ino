#include <Wire.h>

#define MPU_ADDR 0x68

int16_t ax, ay, az, gx, gy, gz;

long ax_offset, ay_offset, az_offset;
long gx_offset, gy_offset, gz_offset;

void calibrateMPU(){

  Serial.println("Calibrating sensor...");
  Serial.println("Keep sensor perfectly still");

  long ax_sum=0, ay_sum=0, az_sum=0;
  long gx_sum=0, gy_sum=0, gz_sum=0;

  for(int i=0;i<2000;i++){

    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR,14,true);

    ax = Wire.read()<<8 | Wire.read();
    ay = Wire.read()<<8 | Wire.read();
    az = Wire.read()<<8 | Wire.read();

    Wire.read(); Wire.read(); // skip temp

    gx = Wire.read()<<8 | Wire.read();
    gy = Wire.read()<<8 | Wire.read();
    gz = Wire.read()<<8 | Wire.read();

    ax_sum += ax;
    ay_sum += ay;
    az_sum += az;

    gx_sum += gx;
    gy_sum += gy;
    gz_sum += gz;

    delay(2);
  }

  ax_offset = ax_sum/2000;
  ay_offset = ay_sum/2000;
  az_offset = (az_sum/2000) - 16384;

  gx_offset = gx_sum/2000;
  gy_offset = gy_sum/2000;
  gz_offset = gz_sum/2000;

  Serial.println("Calibration Done");
}

void setup(){

  Serial.begin(115200);
  Wire.begin(21,22);

  // Wake MPU6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  delay(200);

  calibrateMPU();
}

void loop(){

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR,14,true);

  ax = Wire.read()<<8 | Wire.read();
  ay = Wire.read()<<8 | Wire.read();
  az = Wire.read()<<8 | Wire.read();

  Wire.read(); Wire.read();

  gx = Wire.read()<<8 | Wire.read();
  gy = Wire.read()<<8 | Wire.read();
  gz = Wire.read()<<8 | Wire.read();

  // remove offsets
  ax -= ax_offset;
  ay -= ay_offset;
  az -= az_offset;

  gx -= gx_offset;
  gy -= gy_offset;
  gz -= gz_offset;

  Serial.print("AX: "); Serial.print(ax);
  Serial.print(" AY: "); Serial.print(ay);
  Serial.print(" AZ: "); Serial.println(az);

  Serial.print("GX: "); Serial.print(gx);
  Serial.print(" GY: "); Serial.print(gy);
  Serial.print(" GZ: "); Serial.println(gz);

  Serial.println();

  delay(500);
}