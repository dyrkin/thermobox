#include "Adafruit_MCP9808.h"
#include "Adafruit_SHT31.h"
#include "movingAvg.h"

class Temp
{
public:
    Temp(int avgInterval);
    void begin();
    void update();
    float getMcpTemp();
    float getShtTemp();
    float getAvgTemp();
    float getShtHumidity();

private:
    float _mcpTemp = 0;
    float _shtTemp = 0;
    float _shtHumidity = 0;
    Adafruit_MCP9808 _tempSensorMcp;
    Adafruit_SHT31 _tempSensorSht;
    movingAvg _twoSensorAvgTemp;
};