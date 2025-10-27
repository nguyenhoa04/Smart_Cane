#ifndef MQTT_MOD_H
#define MQTT_MOD_H
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

void wifiMqttSetup(void (*onMsg)(char*, uint8_t*, unsigned int));
void wifiMqttLoop();
void camSendCmd(const char* cmd);

extern WiFiClient   g_net;
extern PubSubClient g_mqtt;

#endif // MQTT_MOD_H
