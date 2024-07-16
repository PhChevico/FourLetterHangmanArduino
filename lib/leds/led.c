
#define __DELAY_BACKWARD_COMPATIBLE__  
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

#define NUMBER_OF_LEDS 4 

void enableOneLed ( int lednumber )
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    DDRB |= ( 1 << ( PB2 + lednumber ));    
}

void enableMultipleLeds(uint8_t leds )
{
    DDRB = (leds << 2); 
}


void enableAllLeds ()
{
    enableMultipleLeds(0b1111);
}


void lightUpOneLed ( int lednumber )    
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    PORTB &= ~( 1 << ( PB2 + lednumber ));  
}


void lightUpMultipleLeds (uint8_t leds)
{
    PORTB &= ~(leds << 2); 
}


void lightUpAllLeds (){
    lightUpMultipleLeds(0b1111);
} 


void lightDownOneLed (int lednumber )
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    PORTB |= ( 1 << ( PB2 + lednumber )); 
}


void lightDownMultipleLeds (uint8_t leds){
    PORTB |= (leds << 2);
}

void lightDownAllLeds() {
    lightDownMultipleLeds(0b1111);
}


void lightToggleOneLed(int lednumber){
    PORTB  ^= (0b10 << lednumber);
}


void lightToggleMultipleLed(uint8_t leds){
    PORTB ^= (leds<<2);
}


void dimLed(int lednumber, int percentage, int duration){
    if (lednumber < 0 || lednumber >= NUMBER_OF_LEDS) return; 

    DDRB |= (1 << (PB2 + lednumber));

    int on_time = (percentage * 10) / 100;
    int off_time = 10 - on_time;

    int cycles = duration / 10;

    for (int i = 0; i < cycles; i++) {
        PORTB &= ~(1 << (PB2 + lednumber));
        _delay_ms(on_time);

        PORTB |= (1 << (PB2 + lednumber));
        _delay_ms(off_time);
    }

    PORTB |= (1 << (PB2 + lednumber));
}



void fadeInLed(int lednumber, int duration) {
   if (lednumber < 0 || lednumber > NUMBER_OF_LEDS-1) return;
   int x = 0;
   DDRB |= (1 << (PB2 + lednumber));
   int time = duration;
   while (duration > 0) {
      PORTB &= ~(1 << (PB2 + lednumber)); // on
      _delay_ms(x / (time / 100));
      PORTB |= (1 << (PB2 + lednumber)); // off
      _delay_ms(10 - x / (time / 100));
      x++;
      duration -= 10;
   }
   PORTB |= (1 << (PB2 + lednumber));
}

void fadeOutLed(int lednumber, int duration) {
   if (lednumber < 0 || lednumber > NUMBER_OF_LEDS-1) return;
   int x = 0;
   DDRB |= (1 << (PB2 + lednumber));
   int time = duration;
   while (duration > 0) {
      PORTB &= ~(1 << (PB2 + lednumber));
      _delay_ms(10 - x / (time / 100));
      PORTB |= (1 << (PB2 + lednumber));
      _delay_ms(x / (time / 100));
      x++;
      duration -= 10;
   }
   PORTB |= (1 << (PB2 + lednumber));
}




void flashingLight(int lednumber, int numberOfFlashes){
    enableOneLed(lednumber);
    for(int i = 0 ; i < numberOfFlashes ; i ++){
        lightUpOneLed(lednumber);
        _delay_ms(250);
        lightDownOneLed(lednumber);
        _delay_ms(250);    
    }
}








