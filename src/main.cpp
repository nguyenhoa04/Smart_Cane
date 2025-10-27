#include <Arduino.h>
#include "config/config.h"
#include "modules/smart_cane_master/smart_cane_master.h"

void setup(){
   smartCaneSetup();
}
void loop(){
  smartCaneLoop();
}
