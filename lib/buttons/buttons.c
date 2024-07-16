
#define __DELAY_BACKWARD_COMPATIBLE__  
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>



void enableButton( int button ){
    DDRC &= ~( 1 << PC1 + button );
    PORTC |= ( 1 << PC1 + button);
}

int buttonPushed( int button ){
    if((PINC & ( 1 << PC1 + button )) == 0){
        return 1;
    } else{
        return 0;
    }
}

int buttonReleased( int button ){
    if(buttonPushed(button == 0)){
        return 1;
    } else{
        return 0;
    }
}