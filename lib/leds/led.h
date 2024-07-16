#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__  
#include <util/delay.h> 

void enableOneLed ( int lednumber ) ;
void enableMultipleLeds(uint8_t leds );
void enableAllLeds ();
void lightUpOneLed ( int lednumber );
void lightUpMultipleLeds (uint8_t leds);
void lightUpAllLeds ();
void lightDownOneLed ( int lednumber );
void lightDownMultipleLeds (uint8_t leds);
void lightDownAllLeds ();
void lightToggleOneLed(int leds);
void lightToggleMultipleLed(uint8_t leds);

void dimLed(int ledNum, int percent, int duration);
void fadeInLed(int lednumber, int duration);  
void fadeOutLed(int lednumber, int duration);

void flashingLight(int lednumber, int numberOfFlashes);