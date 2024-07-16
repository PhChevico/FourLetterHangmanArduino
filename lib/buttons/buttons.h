#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__  
#include <util/delay.h> 



void enableButton( int button );
int buttonPushed( int button );
int buttonReleased( int button );