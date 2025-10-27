#include <Arduino.h>
#include "smart_cane_master.h"
#include "../../config/config.h"

#include "../dfplayer/dfplayer.h"
#include "../ultrasonic/ultrasonic.h"
#include "../gps/gps.h"
#include "../sim_module/sim_module.h"
#include "../button/button.h"
#include "../flask/flask.h"
#include "../mqtt/mqtt_mod.h"

#include <ArduinoJson.h>

/* ===== Trạng thái phát âm / detect ===== */
static bool camActive=false;
static unsigned long lastCamCmdMs=0;
static unsigned long lastMeasureMs=0;

static int lastBucketForUnknown = -1;
static int lastBucketAnnounced  = -1;
static unsigned long lastBucketSayMs = 0;

static String lastDetectKey="";
static unsigned long lastDetectSayMs=0;

static unsigned long lastMappedLabelMs = 0;

/* ===== Helpers ===== */
static uint8_t trackFromLabel(String s){
  s.trim(); s.toLowerCase();
  if (s=="table") return TRK_TABLE;
  if (s=="chair") return TRK_CHAIR;
  if (s=="refrigerator") return TRK_FRIDGE;
  if (s=="door") return TRK_DOOR;
  if (s=="person") return TRK_PERSON;
  return 0;
}

/* ===== MQTT callback ===== */
static void onMqtt(char* topic, uint8_t* payload, unsigned int len){
  if (String(topic) != TOPIC_DET) return;
  String s; s.reserve(len);
  for (unsigned i=0;i<len;i++) s += (char)payload[i];

  StaticJsonDocument<256> doc;
  String label="unknown";
  if (!deserializeJson(doc, s)){
    if (doc.containsKey("label")) label = String((const char*)doc["label"]);
  }

  uint8_t tr = trackFromLabel(label);
  if (tr){ // mapped label -> ƯU TIÊN
    if (!dfIsBusy()) dfPlayTrack(tr);
    lastDetectKey = label; lastDetectKey.toLowerCase();
    lastDetectSayMs = millis();
    lastMappedLabelMs = millis();          // cửa sổ ưu tiên
    Serial.printf("[DET] %s -> track %u (PRIORITY)\n", lastDetectKey.c_str(), tr);
  } else {
    // unknown -> 0006 + số cm gần nhất (nếu có)
    if (!dfIsBusy()) dfPlayTrack(TRK_UNKNOWN);
    if (lastBucketForUnknown>0){
      // xếp hàng số cm ngay khi 0006 xong (dfPoll sẽ tự nhả busy)
      // cơ chế xếp hàng: ở đây đơn giản; để nối tiếp, master sẽ phát số khi DF rảnh trong nhịp kế tiếp
      // (Giữ logic gốc: unknown + đọc số)
      // Thực thi ở tick loop bên dưới.
    }
    lastDetectKey = "unknown";
    lastDetectSayMs = millis();
    Serial.printf("[DET] unknown -> 0006 + %s\n",
                  (lastBucketForUnknown>0? String(lastBucketForUnknown).c_str():"no-cm"));
  }
}

/* ====== SETUP / LOOP ====== */
void smartCaneSetup() {
  Serial.begin(115200); delay(200);

  dfSetup();
  ultrasonicSetup();
  gpsSetup();
  simSetup();
  buttonSetup();

  wifiMqttSetup(onMqtt);
  Serial.printf("[FLASK] http://%s:%d%s  deviceId=%s\n",
                LOCAL_SERVER_IP, LOCAL_SERVER_PORT, LOCAL_SERVER_SAVE_PATH, DEVICE_ID);

  Serial.println("READY");
}

void smartCaneLoop() {
  wifiMqttLoop();
  gpsProcess();
  buttonProcess();         // SOS + rung
  flaskMaybeSendPosition();// Flask push theo chu kỳ
  dfPoll();

  unsigned long now = millis();
  if (now - lastMeasureMs >= HC_INTERVAL_MS) {
    lastMeasureMs = now;

    float cmf = measureCm();
    if (cmf > 0) {
      int cm = (int)roundf(cmf);

      // VÙNG detect (40–120): ưu tiên label
      bool inRange = (cm >= DETECT_IN_MIN && cm <= DETECT_IN_MAX);
      bool outRangeHys = (cm <= EXIT_BELOW_CM || cm >= EXIT_ABOVE_CM);

      if (inRange && !camActive) {
        if (now - lastCamCmdMs >= CAM_CMD_COOLDOWN_MS) {
          camActive = true; camSendCmd("start"); lastCamCmdMs = now;
        }
      } else if (camActive && outRangeHys) {
        if (now - lastCamCmdMs >= CAM_CMD_COOLDOWN_MS) {
          camActive = false; camSendCmd("stop"); lastCamCmdMs = now;
        }
      }

      // cm gần nhất để dùng khi unknown (trong vùng)
      lastBucketForUnknown = bucketOf(cm);

      // NGOÀI vùng 40–120: chỉ đọc số cm (folder /01)
      // nhưng nếu vừa có label map trong cửa sổ ưu tiên thì bỏ qua số
      if (!inRange) {
        bool suppressNumber = ((now - lastMappedLabelMs) < LABEL_PRIORITY_WINDOW_MS);
        if (!suppressNumber) {
          int b = bucketOf(cm);
          if (b>0 && b!=lastBucketAnnounced && (now-lastBucketSayMs>BUCKET_COOLDOWN_MS)) {
            if (!dfIsBusy()) dfPlayNumber(b);
            lastBucketAnnounced = b;
            lastBucketSayMs = now;
            Serial.printf("[RANGE] say %d cm\n", b);
          }
          if (b<0) lastBucketAnnounced = -1;
        }
      }
    }
  }
}
