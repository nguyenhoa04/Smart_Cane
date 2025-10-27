#ifndef GPS_H
#define GPS_H
#include <Arduino.h>

void gpsSetup();
void gpsProcess();
bool gpsHasFix();
bool gpsHaveLast();
float gpsLat();
float gpsLon();
unsigned long gpsLastUpdateMs();

#endif // GPS_H
