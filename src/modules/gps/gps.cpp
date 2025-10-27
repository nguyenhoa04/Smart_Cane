#include <Arduino.h>
#include "gps.h"
#include "../../config/config.h"
#include <TinyGPS++.h>
#include <HardwareSerial.h>

static HardwareSerial GPSSerial(2);
static TinyGPSPlus gps;

static float  g_lat = 0.0f, g_lon = 0.0f;
static bool   g_fix = false, g_haveLast = false;
static unsigned long g_lastMs = 0;

void gpsSetup() {
  // RX2=16 (ESP32) <- GPS TX, TX2=17 -> GPS RX
  GPSSerial.begin(9600, SERIAL_8N1, 16, 17);
}

void gpsProcess() {
  while (GPSSerial.available() > 0) {
    if (gps.encode(GPSSerial.read())) {
      if (gps.location.isValid() && gps.location.isUpdated()) {
        g_lat = gps.location.lat();
        g_lon = gps.location.lng();
        g_fix = true;
        g_haveLast = true;
        g_lastMs = millis();
      }
    }
  }
}

bool gpsHasFix() { return g_fix; }
bool gpsHaveLast(){ return g_haveLast; }
float gpsLat()    { return g_lat; }
float gpsLon()    { return g_lon; }
unsigned long gpsLastUpdateMs(){ return g_lastMs; }
