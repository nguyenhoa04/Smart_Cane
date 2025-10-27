#include <Arduino.h>
#include "mqtt_mod.h"
#include "../../config/config.h"

WiFiClient   g_net;
PubSubClient g_mqtt(g_net);

static void ensureWiFi() {
  if (WiFi.status()==WL_CONNECTED) return;
  WiFi.mode(WIFI_STA); WiFi.setSleep(false);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("WiFi connecting");
  while (WiFi.status()!=WL_CONNECTED){ delay(300); Serial.print("."); }
  Serial.printf("\nWiFi OK: %s\n", WiFi.localIP().toString().c_str());
}

static void mqttReconnect() {
  while(!g_mqtt.connected()){
    Serial.print("MQTT connecting...");
    if (g_mqtt.connect("ESP32-MASTER")){
      Serial.println("OK");
      g_mqtt.subscribe(TOPIC_DET, 0);
      // đảm bảo cam stop khi boot — logic bật/tắt nằm ở master
      g_mqtt.publish(TOPIC_CAM_CMD, "stop", false);
    } else {
      Serial.printf("rc=%d retry\n", g_mqtt.state()); delay(1000);
    }
  }
}

void wifiMqttSetup(void (*onMsg)(char*, uint8_t*, unsigned int)) {
  ensureWiFi();
  g_mqtt.setServer(MQTT_HOST, MQTT_PORT);
  g_mqtt.setCallback(onMsg);
  g_mqtt.setBufferSize(1024);
  g_mqtt.setKeepAlive(60);
  g_mqtt.setSocketTimeout(30);
  mqttReconnect();
}

void wifiMqttLoop() {
  if (WiFi.status()!=WL_CONNECTED) ensureWiFi();
  if (!g_mqtt.connected()) mqttReconnect();
  g_mqtt.loop();
}

void camSendCmd(const char* cmd){
  g_mqtt.publish(TOPIC_CAM_CMD, cmd, false);
  Serial.printf("[CAM] cmd=%s\n", cmd);
}
