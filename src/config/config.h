#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>

/* ===== WiFi & MQTT ===== */
extern const char* WIFI_SSID;
extern const char* WIFI_PASSWORD;
extern const char* MQTT_HOST;
extern const int   MQTT_PORT;
extern const char* TOPIC_CAM_CMD;
extern const char* TOPIC_DET;

/* ===== Flask local server ===== */
extern const char* LOCAL_SERVER_IP;
extern const int   LOCAL_SERVER_PORT;
extern const char* LOCAL_SERVER_SAVE_PATH;
extern const char* DEVICE_ID;
extern const unsigned long SERVER_UPDATE_INTERVAL_MS;

/* ===== Pins ===== */
#define HC_TRIG   13
#define HC_ECHO   34
#define DF_RX     22
#define DF_TX     21
#define BUTTON_PIN   27
#define VIBR_PIN     14
#define SIM_RX_PIN   18
#define SIM_TX_PIN   19

/* ===== Tracks /MP3/000N.mp3 ===== */
#define TRK_TABLE   1
#define TRK_CHAIR   2
#define TRK_FRIDGE  3
#define TRK_DOOR    4
#define TRK_PERSON  5
#define TRK_UNKNOWN 6

/* ===== HC-SR04 & detect range ===== */
extern const unsigned long HC_INTERVAL_MS;
extern const float RANGE_MIN, RANGE_MAX;
extern const float DETECT_IN_MIN;
extern const float DETECT_IN_MAX;
extern const float EXIT_BELOW_CM;
extern const float EXIT_ABOVE_CM;

/* ===== Buckets (±5 cm) ===== */
extern const int BUCKETS[];
extern const int NB;
extern const int BUCKET_HALF_WIDTH;

/* ===== Debounce / cooldown ===== */
extern const unsigned long CAM_CMD_COOLDOWN_MS;
extern const unsigned long BUCKET_COOLDOWN_MS;
extern const unsigned long DETECT_COOLDOWN_MS;
extern const unsigned long LABEL_PRIORITY_WINDOW_MS;

/* ===== Vibration (SW-420) by pulseIn ===== */
extern const unsigned long VIBR_PULSE_THRESH_US;
extern const unsigned long VIBR_PULSE_TIMEOUT_US;
extern const unsigned long VIBR_MEASURE_INTERVAL_MS;
extern const unsigned long FALL_CALL_COOLDOWN_MS;

/* ===== Guardian Phone ===== */
extern const String GUARDIAN_PHONE_NUMBER;

#endif // CONFIG_H