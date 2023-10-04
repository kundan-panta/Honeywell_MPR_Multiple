#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#define DEFAULT_ADDRESS 0x18
#define MAXIMUM_PSI     25
#define MINIMUM_PSI     0

#define BUSY_FLAG       0x20
#define INTEGRITY_FLAG  0x04
#define MATH_SAT_FLAG   0x01
#define POWER_FLAG 0x40

#define OUTPUT_MAX 0xE66666
#define OUTPUT_MIN 0x19999A

// Time (microseconds) needed after requesting pressure from sensor
#define REQUEST_WAIT_MUS 9000  // >=5000 according to datasheet

enum Pressure_Units {
  PSI,
  PA,
  KPA,
  TORR,
  INHG,
  ATM,
  BAR
};

class SparkFun_MicroPressure
{
  public:
    SparkFun_MicroPressure(int8_t eoc_pin=-1, int8_t rst_pin=-1, uint8_t minimumPSI=MINIMUM_PSI, uint8_t maximumPSI=MAXIMUM_PSI);
    bool begin(uint8_t deviceAddress=DEFAULT_ADDRESS, TwoWire &wirePort=Wire, bool returnFlags=false, uint16_t tDelay=REQUEST_WAIT_MUS);
    uint8_t readStatus(void);
    float readPressure(Pressure_Units units=PSI);

    // New functions to separate request and read commands
    // Needed for non-blocking approach to wait for sensor to be ready, instead of querying
    void requestPressure();
    uint32_t readPressureRaw();
    bool sensorReady();
    float convertToUnits(Pressure_Units units=PSI);
    
  private:
    int8_t _address;
	  int8_t _eoc, _rst;
	
    uint8_t _minPsi, _maxPsi;
    
    TwoWire *_i2cPort;

    // New variables
    uint32_t _pRaw;  // Raw pressure value
    uint32_t _tRequest;  // Time of last request
    uint16_t _tDelay;  // Time (microseconds) needed after requesting pressure from sensor
    bool _returnFlags; // Return error codes instead of pressure
};
