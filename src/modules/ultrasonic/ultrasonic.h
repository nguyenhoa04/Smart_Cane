#ifndef ULTRASONIC_H
#define ULTRASONIC_H
#include <Arduino.h>

void  ultrasonicSetup();
float measureCm();
int   bucketOf(int cm);

#endif // ULTRASONIC_H
