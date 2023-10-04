/*
Based on example code by SparkFun in their MPR library.
Kundan Panta | kundan.panta123@gmail.com | 2023-10-03
*/

#include <Honeywell_MPR_Multiple.h>

// Initialize sensor variables
SparkFun_MicroPressure mpr1(-1, -1, 0, 1);
SparkFun_MicroPressure mpr2(-1, -1, 0, 1);

// I2C addresses
#define I2C_MPR1 0x18
#define I2C_MPR2 0x17

// To hold (raw) pressure values
uint32_t p1 = 0;
uint32_t p2 = 0;


void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Check if sensors are functioning
  bool mpr1_begin = mpr1.begin(I2C_MPR1);
  bool mpr2_begin = mpr2.begin(I2C_MPR2);

  if (!mpr1_begin) {
    Serial.println("Cannot connect to MPR 1.");
  }
  if (!mpr2_begin) {
    Serial.println("Cannot connect to MPR 2.");
  }

  // Stop if any sensor is not working
  while (!mpr1_begin || !mpr2_begin) {
    delay(1000);
  }
}

void loop() {
  // Request pressure
  mpr1.requestPressure();
  mpr2.requestPressure();
  
  // Wait till sensors are ready
  while (!mpr1.sensorReady());
  while (!mpr2.sensorReady());

  // Read sensors (raw)
  p1 = mpr1.readPressureRaw();
  p2 = mpr2.readPressureRaw();

  // Print readings
  Serial.print(p1);
  Serial.print(",");
  Serial.print(p2);
  Serial.println();
}