#include "config.h"

/* ---------------- WiFi / MQTT ---------------- */
const char* WIFI_SSID     = "PTIT.HCM_CanBo";
const char* WIFI_PASSWORD = "";
const char* MQTT_HOST     = "10.252.6.156";
const int   MQTT_PORT     = 1883;
const char* TOPIC_CAM_CMD = "cam/cmd";
const char* TOPIC_DET     = "smartcane/detections";

/* ---------------- HTTP Flask Server ---------------- */
const char* LOCAL_SERVER_IP        = "10.252.6.156";
const int   LOCAL_SERVER_PORT      = 8080;
const char* LOCAL_SERVER_SAVE_PATH = "/save_location";
const char* DEVICE_ID              = "SmartCane01";
const unsigned long SERVER_UPDATE_INTERVAL_MS = 30000;

/* ---------------- Tham số hành vi ---------------- */
const char* MQTT_CLIENT_ID = "SmartCaneMaster-ESP32-01";
const char* MQTT_USERNAME  = "";
const char* MQTT_PASSWORD2 = "";
const float   DETECT_THRESHOLD = 0.50f;
const uint8_t DF_VOLUME        = 27;
const long    SERVER_UPDATE_INTERVAL = 30000;
const unsigned long REPEAT_MS  = 2500;

// HC-SR04 dưới (phía trước thấp)
const long  HC_SR04_CHECK_INTERVAL = 300;
const float OBSTACLE_CM_MIN = 40.0f;
const float OBSTACLE_CM_MAX = 60.0f;

// HC-SR04 trên (vật cản tầm đầu/ngực)
const float OVERHEAD_CM_MIN = 35.0f;
const float OVERHEAD_CM_MAX = 120.0f;
const long  HC_SR04_TOP_CHECK_INTERVAL = 300;

/* ===== HC-SR04 & detect range ===== */
const unsigned long HC_INTERVAL_MS = 180;
const float RANGE_MIN = 2.0f, RANGE_MAX = 400.0f;
const float DETECT_IN_MIN = 40.0f;
const float DETECT_IN_MAX = 120.0f;
const float EXIT_BELOW_CM = 35.0f;
const float EXIT_ABOVE_CM = 130.0f;

/* ===== Buckets (±5 cm) ===== */
const int BUCKETS[] = {30,40,50,60,70,80,90,100};
const int NB = sizeof(BUCKETS)/sizeof(BUCKETS[0]);
const int BUCKET_HALF_WIDTH = 5;

/* ===== Debounce / cooldown ===== */
const unsigned long CAM_CMD_COOLDOWN_MS = 300;
const unsigned long BUCKET_COOLDOWN_MS  = 1200;
const unsigned long DETECT_COOLDOWN_MS  = 1000;
const unsigned long LABEL_PRIORITY_WINDOW_MS = 1500;

/* ===== Vibration (SW-420) by pulseIn ===== */
const unsigned long VIBR_PULSE_THRESH_US     = 70000UL;
const unsigned long VIBR_PULSE_TIMEOUT_US    = 120000UL;
const unsigned long VIBR_MEASURE_INTERVAL_MS = 120;
const unsigned long FALL_CALL_COOLDOWN_MS    = 15000UL;

/* ===== Guardian Phone ===== */
const String GUARDIAN_PHONE_NUMBER = "+84373148007";