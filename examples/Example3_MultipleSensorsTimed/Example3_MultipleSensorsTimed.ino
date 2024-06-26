/*
Based on example code by SparkFun in their MPR library.
Kundan Panta | kundan.panta123@gmail.com | 2024-06-26
*/

#include <Honeywell_MPR_Multiple.h>

// Create arrays to hold info for each sensor
#define NUM_PRESSURE 3  // Number of pressure sensors
uint8_t mpr_i2c_addr[NUM_PRESSURE] = {0x1B, 0x1C, 0x1F}; // I2C addresses
uint32_t mpr_p[NUM_PRESSURE];  // To hold (raw) pressure values
float mpr_p_units[NUM_PRESSURE];  // Pressure in specified units
SparkFun_MicroPressure* mpr = (SparkFun_MicroPressure*)malloc(sizeof(SparkFun_MicroPressure) * NUM_PRESSURE);  // To call constructor with non-default parameters

// For timing
#define SENSOR_RATE_HZ 50.
const int sensor_period_ms = 1000. / SENSOR_RATE_HZ;
uint32_t T_s = 0;
bool mpr_requested = false;  // Flag to tell if the sensors have been sent a request command

void setup() {
  Serial.begin(115200);
  Wire.begin();
  // Wire.setClock(400000);

  // Initialize sensors
  bool mpr_begin_all = true;
  for (uint8_t i = 0; i < NUM_PRESSURE; i++) {
    // Call constructor with desired parameters: EOC_PIN, RST_PIN, MIN_PSI, MAX_PSI
    mpr[i] = SparkFun_MicroPressure(-1, -1, 0, 1);
    // Check that sensors are available
    bool mpr_i_begin = mpr[i].begin(mpr_i2c_addr[i]);
    if (!mpr_i_begin) {
      Serial.print("Cannot connect to MPR ");
      Serial.print(i + 1);
      Serial.println(".");
    }
    mpr_begin_all = mpr_begin_all && mpr_i_begin;
  }
  while (!mpr_begin_all);
}

void loop() {
  // Run sensors at desired frequency, alternating between requests and reading
  uint32_t T_now = millis();
  if (!mpr_requested && T_now - T_s >= sensor_period_ms) {
    T_s += sensor_period_ms;  // Update last sample time
    mpr_requested = true;
    
    // Request pressure
    for (uint8_t i = 0; i < NUM_PRESSURE; i++) {
      mpr[i].requestPressure();
    }
  }
  else if (mpr_requested) {
    // Check if enough time has elapsed since request for all sensors
    bool mpr_ready_all = true;
    for (uint8_t i = 0; i < NUM_PRESSURE; i++) {
      mpr_ready_all = mpr_ready_all && mpr[i].sensorReady();
    }
    
    if (mpr_ready_all) {  // If all sensors are ready
      mpr_requested = false;

      // Read sensors
      for (uint8_t i = 0; i < NUM_PRESSURE; i++) {
        mpr_p[i] = mpr[i].readPressureRaw();
        mpr_p_units[i] = mpr[i].convertToUnits(PA);  // If wanted, get readings in desired units
      }
      
      // Print readings
      Serial.print(T_now);
      Serial.print(',');
      for (uint8_t i = 0; i < NUM_PRESSURE; i++) {
        Serial.print(mpr_p[i]);  // Print raw reading
        // Serial.print(mpr_p_units[i]);  // Or print in requested units
        (i == NUM_PRESSURE - 1) ? Serial.println() : Serial.print(",");
      }
    }
  }
}
