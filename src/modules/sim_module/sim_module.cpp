#include <Arduino.h>
#include "sim_module.h"
#include "../../config/config.h"
#include <HardwareSerial.h>

static HardwareSerial SIMSerial(1);

static bool simReadResp(String& out, unsigned long timeout=2000){
  out = "";
  unsigned long t0 = millis();
  while (millis()-t0 < timeout){
    while (SIMSerial.available()) out += (char)SIMSerial.read();
    if (out.indexOf("OK")!=-1 || out.indexOf("ERROR")!=-1 || out.indexOf(">")!=-1
        || out.indexOf("+CME ERROR")!=-1 || out.indexOf("+CMS ERROR")!=-1) break;
    delay(10);
  }
  out.trim();
  return out.length()>0;
}

void simSetup(){
  SIMSerial.begin(115200, SERIAL_8N1, SIM_RX_PIN, SIM_TX_PIN);
  Serial.println("[SIM] Probing...");
  String r;
  SIMSerial.println("AT");       simReadResp(r, 1000); Serial.println(r);
  SIMSerial.println("AT+CPIN?"); simReadResp(r, 1000); Serial.println(r);
  SIMSerial.println("AT+CREG?"); simReadResp(r, 1000); Serial.println(r);
  SIMSerial.println("AT+CSQ");   simReadResp(r, 1000); Serial.println(r);
}

bool simReady(){
  String r; unsigned long t0=millis();
  do { SIMSerial.println("AT"); simReadResp(r, 500);
       if (r.indexOf("OK")!=-1) return true; delay(200);
  } while (millis()-t0 < 8000);
  return false;
}
bool simRegistered(){
  String r; unsigned long t0=millis();
  do { SIMSerial.println("AT+CREG?"); if (simReadResp(r, 800)){
         if (r.indexOf(",1")!=-1 || r.indexOf(",5")!=-1) return true;
       } delay(500);
  } while (millis()-t0 < 20000);
  return false;
}

static bool simSetupSmsTextMode(){
  String r;
  SIMSerial.println("ATE0");               simReadResp(r, 800);
  SIMSerial.println("AT+CMEE=2");          simReadResp(r, 800);
  SIMSerial.println("AT+CMGF=1");          if (!simReadResp(r, 1200)) return false;
  SIMSerial.println("AT+CSCS=\"GSM\"");    simReadResp(r, 1200);
  SIMSerial.println("AT+CSMP=17,167,0,0"); simReadResp(r, 1200);
  SIMSerial.println("AT+CSCA?");           simReadResp(r, 1200);
  return true;
}

void simSendEmergencySMS(const String& phone, float lat, float lon,
                         bool hasFix, bool haveLast, unsigned long lastAgeSec) {
  if (!simReady())       { Serial.println("[SIM] AT not ready"); return; }
  if (!simRegistered())  { Serial.println("[SIM] Not registered to network"); return; }
  simSetupSmsTextMode();

  String msg;
  if (hasFix) {
    msg = "SOS! Lat=" + String(lat,6) + ", Lon=" + String(lon,6) +
          ". http://maps.google.com/maps?q=" + String(lat,6) + "," + String(lon,6);
  } else if (haveLast) {
    msg = "SOS! Gan nhat (" + String(lastAgeSec) + "s): Lat=" + String(lat,6) + ", Lon=" + String(lon,6) +
          ". http://maps.google.com/maps?q=" + String(lat,6) + "," + String(lon,6);
  } else {
    msg = "SOS! Vi tri chua xac dinh.";
  }

  String r;
  while (SIMSerial.available()) SIMSerial.read();
  SIMSerial.print("AT+CMGS=\""); SIMSerial.print(phone); SIMSerial.println("\"");
  if (!simReadResp(r, 5000) || r.indexOf(">")==-1){ Serial.println("[SIM] CMGS prompt failed"); return; }
  SIMSerial.print(msg); SIMSerial.write(0x1A);
  if (simReadResp(r, 20000) && (r.indexOf("+CMGS:")!=-1 || r.indexOf("OK")!=-1))
    Serial.println("[SIM] SMS sent");
  else { Serial.print("[SIM] SMS send fail: "); Serial.println(r); }
}

void simMakeEmergencyCall(const String& phone){
  if (!simReady() || !simRegistered()){ Serial.println("[SIM] Not ready/registered for call"); return; }
  String r; SIMSerial.print("ATD"); SIMSerial.print(phone); SIMSerial.println(";"); simReadResp(r, 5000);
  Serial.println("[SIM] Dialing...");
}
