#include <Arduino.h>
#include "button.h"
#include "../../config/config.h"
#include "../gps/gps.h"
#include "../sim_module/sim_module.h"

// Vibration (pulse width) state
static unsigned long lastVibrMeasureMs = 0;
static unsigned long lastFallAction    = 0;

// SOS state
static enum { SOS_IDLE, SOS_PRESSED, SOS_WAIT_FOR_CALL, SOS_CALLING } sosState = SOS_IDLE;
static unsigned long sosStateTimer = 0;

static unsigned long TP_init() {
  pinMode(VIBR_PIN, INPUT);
  unsigned long w = pulseIn(VIBR_PIN, HIGH, VIBR_PULSE_TIMEOUT_US);
  return w;
}

static void checkVibration() {
  unsigned long now = millis();
  if (now - lastVibrMeasureMs < VIBR_MEASURE_INTERVAL_MS) return;
  lastVibrMeasureMs = now;

  unsigned long pw = TP_init();
  if (pw > VIBR_PULSE_THRESH_US) {
    if (now - lastFallAction > FALL_CALL_COOLDOWN_MS) {
      Serial.printf("[FALL] pulse=%lu us -> Auto call\n", pw);
      simMakeEmergencyCall(GUARDIAN_PHONE_NUMBER);
      lastFallAction = now;
    }
  }
}

void buttonSetup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(VIBR_PIN, INPUT);
}

void buttonProcess() {
  // --- SOS Button ---
  bool isPressed = (digitalRead(BUTTON_PIN) == LOW);
  switch (sosState) {
    case SOS_IDLE:
      if (isPressed) { sosState = SOS_PRESSED; sosStateTimer = millis(); }
      break;
    case SOS_PRESSED:
      if (!isPressed) { sosState = SOS_IDLE; break; }
      if (millis() - sosStateTimer >= 100) {
        // Send SMS
        {
          bool fix = gpsHasFix(), haveLast = gpsHaveLast();
          float lat = fix ? gpsLat() : (haveLast ? gpsLat() : 0);
          float lon = fix ? gpsLon() : (haveLast ? gpsLon() : 0);
          unsigned long age = haveLast ? (millis()-gpsLastUpdateMs())/1000 : 0;
          simSendEmergencySMS(GUARDIAN_PHONE_NUMBER, lat, lon, fix, haveLast, age);
        }
        sosState = SOS_WAIT_FOR_CALL;
        sosStateTimer = millis();
      }
      break;
    case SOS_WAIT_FOR_CALL:
      if (millis() - sosStateTimer >= 3000) {
        simMakeEmergencyCall(GUARDIAN_PHONE_NUMBER);
        sosState = SOS_CALLING;
        sosStateTimer = millis();
      }
      break;
    case SOS_CALLING:
      if (millis() - sosStateTimer >= 5000) { sosState = SOS_IDLE; }
      break;
  }

  // --- Vibration trigger ---
  if (sosState == SOS_IDLE) checkVibration();
}
