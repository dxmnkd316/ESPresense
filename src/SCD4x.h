#pragma once
#ifdef SENSORS
#include <ArduinoJson.h>

namespace SCD4x {
void ConnectToWifi();
void SerialReport();
bool SendDiscovery();
void Setup();
void Loop();
}  // namespace SCD4x

#endif
