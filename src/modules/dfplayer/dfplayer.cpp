#include <Arduino.h>
#include "dfplayer.h"
#include "../../config/config.h"
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

static SoftwareSerial dfSS(DF_RX, DF_TX);
static DFRobotDFPlayerMini df;
static volatile bool g_busy = false;

void dfSetup() {
  dfSS.begin(9600);
  if (!df.begin(dfSS, true, false)) {
    Serial.println("DFPlayer not found");
  } else {
    df.outputDevice(DFPLAYER_DEVICE_SD);
    df.volume(28);
    df.setTimeOut(500);
    Serial.println("DFPlayer ready");
  }
}

bool dfIsBusy() { return g_busy; }

void dfPlayTrack(uint8_t n) {
  if (!g_busy) { df.playMp3Folder(n); g_busy = true; }
}

void dfPlayNumber(int cm) {
  if (!g_busy) { df.playFolder(1, constrain(cm,1,255)); g_busy = true; }
}

void dfPoll() {
  if (df.available()) {
    uint8_t t = df.readType();
    if (t == DFPlayerPlayFinished || t == DFPlayerError) {
      g_busy = false;
    }
  }
}
