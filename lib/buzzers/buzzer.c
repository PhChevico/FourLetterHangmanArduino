#include "buzzer.h"
#include <avr/io.h>
#include <usart.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>


float frequenciesHappySong[] = {
        261.63, // C4
        293.66, // D4
        329.63, // E4
        349.23, // F4
        392.00, // G4
        440.00, // A4
        493.88, // B4
        523.25,  // C5
        261.63, // C4
        293.66, // D4
        329.63, // E4
        349.23, // F4
        392.00, // G4
        440.00, // A4
        493.88, // B4
        523.25  // C5
};

float frequenciesLossSong[] = {
        196.00, // G3
        220.00, // A3
        233.08, // A#3/Bb3
        261.63, // C4
        293.66, // D4
        329.63, // E4
        349.23, // F4
        392.00, // G4
        196.00, // G3
        220.00, // A3
        233.08, // A#3/Bb3
        261.63, // C4
        293.66, // D4
        329.63, // E4
        349.23, // F4
        392.00  // G4
};

int lengthWin = sizeof(frequenciesHappySong) / sizeof(frequenciesHappySong[0]);
int lengthLoss = sizeof(frequenciesLossSong) / sizeof(frequenciesLossSong[0]);


void enableBuzzer(){
    DDRD |= ( 1 << PD3 ); //Buzzer is connected to PD3
}


void playTone( float frequency, uint32_t duration )
{
   uint32_t periodInMicro = ( uint32_t ) ( 1000000 / frequency ); //Calculate the period in microsecs from the freq
   uint32_t durationInMicro = duration * 1000; //We express duration in microsecs
   for ( uint32_t time = 0; time < durationInMicro; time += periodInMicro ) 
    {
    PORTD &= ~( 1 << PD3 ); //turn the buzzer on
    _delay_us( periodInMicro / 2 ); //Wait for the half of the period
    PORTD |= ( 1 << PD3 ); //Turn the buzzer off
    _delay_us( periodInMicro / 2 ); //Wait again for half of the period
    }
}


void disableBuzzer() {
    DDRD &= ~(1 << PD3);
}

void playMusicWin(){
    for (int note = 0; note < lengthWin; note++) {
      for(int i = 0; i < 500; i ++){
        writeString("YES!");
      }

      if((note % 7 == 0 || note % 15 == 0) && note != 0){
        playTone(frequenciesHappySong[note], 333.33);
      }
      playTone(frequenciesHappySong[note], 225);
      _delay_ms(100);
      if(note == lengthWin - 1){
        break;
      }
    }
}


void playMusicLoss(){
    for (int note = 0; note < lengthLoss; note++) {
      for(int i = 0; i < 500; i ++){
        writeString("YES!");
      }

      if((note % 7 == 0 || note % 15 == 0) && note != 0){
        playTone(frequenciesLossSong[note], 333.33);
      }
      playTone(frequenciesLossSong[note], 225);
      _delay_ms(100);
      if(note == lengthLoss - 1){
        break;
      }
    }
}