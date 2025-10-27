#include <Arduino.h>
#include "ultrasonic.h"
#include "../../config/config.h"

void ultrasonicSetup() {
  pinMode(HC_TRIG, OUTPUT);
  pinMode(HC_ECHO, INPUT);
}

float measureCm() {
  digitalWrite(HC_TRIG, LOW); delayMicroseconds(2);
  digitalWrite(HC_TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(HC_TRIG, LOW);
  long dur = pulseIn(HC_ECHO, HIGH, 30000);
  float cm = dur * 0.034f / 2.0f;
  if (cm < RANGE_MIN || cm > RANGE_MAX) return -1.0f;
  return cm;
}

int bucketOf(int cm) {
  for (int i=0;i<NB;i++){
    if (cm >= BUCKETS[i]-BUCKET_HALF_WIDTH && cm <= BUCKETS[i]+BUCKET_HALF_WIDTH)
      return BUCKETS[i];
  }
  return -1;
}
