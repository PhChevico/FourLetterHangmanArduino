#include <avr/io.h>

void enableBuzzer();
void playTone( float frequency, uint32_t duration );
void disableBuzzer();
void playMusicWin();
void playMusicLoss();