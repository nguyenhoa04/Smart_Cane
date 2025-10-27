#include <Arduino.h>
#include "flask.h"
#include "../../config/config.h"
#include "../gps/gps.h"
#include <WiFi.h>
#include <HTTPClient.h>

static unsigned long lastServerUpdateTime = 0;

void flaskMaybeSendPosition() {
  if (WiFi.status()!=WL_CONNECTED) return;
  if (!gpsHasFix() && !gpsHaveLast()) return;
  unsigned long now = millis();
  if (now - lastServerUpdateTime < SERVER_UPDATE_INTERVAL_MS) return;

  float lat = gpsHasFix() ? gpsLat() : gpsLat();
  float lon = gpsHasFix() ? gpsLon() : gpsLon();

  HTTPClient http;
  String url = String("http://") + LOCAL_SERVER_IP + ":" + LOCAL_SERVER_PORT + LOCAL_SERVER_SAVE_PATH +
               "?deviceId=" + DEVICE_ID +
               "&latitude="  + String(lat, 6) +
               "&longitude=" + String(lon, 6);
  http.begin(url);
  http.setConnectTimeout(8000);
  http.setTimeout(8000);
  int code = http.GET();
  Serial.printf("[HTTP] %s -> code=%d\n", url.c_str(), code);
  http.end();

  lastServerUpdateTime = now;
}
