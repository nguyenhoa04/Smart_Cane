#ifndef SIM_MODULE_H
#define SIM_MODULE_H
#include <Arduino.h>

void simSetup();
bool simReady();
bool simRegistered();
void simSendEmergencySMS(const String& phone, float lat, float lon,
                         bool hasFix, bool haveLast, unsigned long lastAgeSec);
void simMakeEmergencyCall(const String& phone);

#endif // SIM_MODULE_H
