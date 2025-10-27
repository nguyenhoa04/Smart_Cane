#ifndef DFPLAYER_H
#define DFPLAYER_H
#include <Arduino.h>

void dfSetup();
void dfPoll();
bool dfIsBusy();
void dfPlayTrack(uint8_t n);
void dfPlayNumber(int cm);

#endif // DFPLAYER_H
