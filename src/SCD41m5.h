#pragma once
#ifdef SENSORS
#include <ArduinoJson.h>

namespace SCD41m5 {
void ConnectToWifi();
void SerialReport();
bool SendDiscovery();
void Setup();
void Loop();
}  // namespace SCD41m5

#endif
