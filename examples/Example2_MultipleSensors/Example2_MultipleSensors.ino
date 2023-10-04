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

// For timing
#define SENSOR_RATE_HZ 100.
const int sensor_period_ms = 1000. / SENSOR_RATE_HZ;
uint32_t T_s = 0;
bool mpr_requested = false;  // Flag to tell if the sensors have been sent a request command


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
  // Run sensors at desired frequency, alternating between requests and reading
  if (!mpr_requested && millis() - T_s >= sensor_period_ms) {
    // Request pressure
    T_s += sensor_period_ms;  // Update last sample time
    mpr_requested = true;
    mpr1.requestPressure();
    mpr2.requestPressure();
  }
  else if (mpr_requested && mpr1.sensorReady() && mpr2.sensorReady()) {
    // Read sensors
    mpr_requested = false;
    p1 = mpr1.readPressureRaw();
    p2 = mpr2.readPressureRaw();

    // Print readings
    Serial.print(p1);
    Serial.print(",");
    Serial.print(p2);
    Serial.println();
  }
}
