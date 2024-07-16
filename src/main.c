#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <usart.h>
#include <util/delay.h>
#include <led.h>
#include <display.h>
#include <buttons.h>
#include <buzzer.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>  


#define LED_PORT PORTB
#define BUTTON_PORT PORTC
#define BUTTON_PIN PINC
#define LED_DDR DDRB
#define BUTTON_DDR DDRC
#define BUTTON1 PC1
#define BUTTON2 PC2
#define BUTTON3 PC3
#define LED1 PB2
#define LED2 PB3


typedef struct {
    char category[5];
    char word[5];
    int attempts;
    int time;
} PUZZLE;

PUZZLE currentPuzzle;

char category [4] [5] = {"ANML", "HMAN", "CTRY", "THNG"};
char localCategory [4];
char answer [5];
int isCategorySelected = 0;


char animal[20][5] = {
    "_U_K", "_O_F", "_O_T", "_RA_",
    "_IS_", "_R_G", "_E_R", "_EA_",
    "_EA_", "_I_D", "_A_B", "_O_S",
    "DO__", "NE__", "_I_N", "_I_S",
    "_O__", "_OL_", "_OA_", "_UN_"
};

char correctAnimal[20][5] = {
    "DUCK", "WOLF", "GOAT", "CRAB",
    "FISH", "FROG", "DEER", "BEAR",
    "SEAL", "BIRD", "LAMB", "COWS",
    "DOGS", "NEWT", "LION", "PIGS",
    "TOAD", "MOLE", "BOAR", "TUNA"
};

char human[20][5] = {
    "_E_K", "_O_E", "_HI_", "_AI_",
    "_A_D", "_OU_", "_EA_", "_A_E",
    "_N_E", "_EE_", "HE__", "_I_S",
    "_O_T", "_A_T", "_IB_", "_U__",
    "EY__", "_I_S", "_A_M", "_A_K"
};
char correctHuman[20][5] = {
    "NECK", "NOSE", "CHIN", "HAIR",
    "HAND", "MOUT", "HEAD", "FACE",
    "KNEE", "FEET", "HERT", "LIPS",
    "FOOT", "CART", "RIBS", "LUNG",
    "EYES", "RIBS", "PALM", "BACK"
};

char country[20][5] = {
  "IR__", "IT__", "_A_N", "_E_U",
  "_H_D", "_N_A", "_U_A", "_HN_",
  "_IJ_", "_GY_", "_NG_", "_O_O",
  "JR__", "_A_S", "_R_Q", "_M_N",
  "NI__", "_A_I", "_S_A", "_R_Z"
};
char correctCountry[20][5] = {
    "IRAN", "ITAL", "JAPN", "PERU",
    "CHAD", "INDA", "CUBA", "CHNA",
    "FIJI", "EGYP", "CNGO", "TOGO",
    "JRDN", "LAOS", "IRAQ", "OMAN",
    "NIUE", "MALI", "RSSA", "BRAZ"
};

char thing[20][5] = {
    "RO__", "_A_T", "_E_L", "_O_N",
    "_IN_", "_E_R", "GO__", "_O_E",
    "_IF_", "_U_P", "VA__", "_A_P",
    "_A_E", "_O_R", "SA__", "_OS_",
    "LI__", "_I_T", "_I_E", "ZO__"
};

char correctThing[20][5] = {
    "ROPE", "DART", "BELL", "MOON",
    "WIND", "PEAR", "GOLD", "HOPE",
    "LIFT", "JUMP", "VASE", "LAMP",
    "FACE", "DOOR", "SAND", "ROSE",
    "LINE", "MINT", "LIME", "ZOOM"
};

volatile uint32_t timer_seconds = 0;
volatile uint32_t last_button_press_seconds = 0; 
int isGameStarted = 1;
int current_category = 0;
int clickCount = 1;


void initTimer1() {
    TCCR1B |= (1 << WGM12);  
    TCCR1B |= (1 << CS12) | (1 << CS10); 
    TIMSK1 |= (1 << OCIE1A); // Enable timer compare interrupt
    OCR1A = 15624; 
    TCCR1B |= (1 << CS12) | (1 << CS10); 
}

ISR(TIMER1_COMPA_vect) {
    timer_seconds++;

    if (timer_seconds - last_button_press_seconds >= 60) { // 1 minute has passed since the last button press
          currentPuzzle.attempts++; 
          last_button_press_seconds = timer_seconds; //that "resets" the timer in last button press
    }
}



ISR(PCINT1_vect)
{
    if (bit_is_clear(BUTTON_PIN, BUTTON1))
    {
       
          _delay_us(1000);
          if (bit_is_clear(BUTTON_PIN, BUTTON1))
          {
            if(clickCount == 1){
              clickCount++;
            } else if(clickCount > 1 && clickCount < 5){
              clickCount++;
              current_category++; 
            } else if(clickCount == 5){
              clickCount = 2;
              current_category = 0;
            }
          }
    }

    if (bit_is_clear(BUTTON_PIN, BUTTON2))
    {
        _delay_us(1000);
        if (bit_is_clear(BUTTON_PIN, BUTTON2))
        {
          if(strcmp(currentPuzzle.category, "") == 0){
            for (int k = 0; k < 4; k++)
            {
                currentPuzzle.category[k] = category[current_category][k];
            }
            currentPuzzle.category[4] = '\0';
            strcpy(localCategory, currentPuzzle.category);
            isCategorySelected = 1;
            _delay_ms(500);
          }
        }
    }

    if (bit_is_clear(BUTTON_PIN, BUTTON3))
    {
        _delay_us(1000);
        if (bit_is_clear(BUTTON_PIN, BUTTON3))
        {
          if(strcmp(currentPuzzle.category, "") == 0){
            printf("I dont use interrupts");
          }
        }
    }

}


void startingGame(){
  sei();
  while(1){
      writeString("CAT?");
      if(buttonPushed(0)){
        break;
      }
  }
  while(1){
      writeString(category[current_category]);
      if(isCategorySelected == 1){
        break;
      }
  }
}

void gamePlay(){
    
    startingGame();
    currentPuzzle.time = 0;
    currentPuzzle.attempts = 0;
    initTimer1(); // Initialize Timer1

    srand(TCNT1); 
    while (1)
    {
      
      int randomIndex = rand() % 20; 
      

      //this is working, had to use a local variable to make it work
      char localAnswer [4];

      if(strcmp(currentPuzzle.category, "ANML") == 0){
        strcpy(currentPuzzle.word, animal[randomIndex]);
        currentPuzzle.word[4] = '\0';
        
        strcpy(answer, correctAnimal[randomIndex]);
        answer[4] = '\0';

        //this is working, had to use a local variable to make it work
        strcpy(localAnswer, answer); 

        while(1){
          if(strcmp(localAnswer, currentPuzzle.word) == 0){
            currentPuzzle.attempts++;
            for(int i = 0; i < 10000; i ++){
              writeString(currentPuzzle.word);
            }
            break;
          }else{
            writeString(currentPuzzle.word);
            if(buttonPushed(0)){
              if(animal[randomIndex][0] == '_'){
                if (currentPuzzle.word[0] < 'Z') { 
                  currentPuzzle.word[0]++;  
                } else {
                  currentPuzzle.word[0] = 'A';
                }
                _delay_ms(500);
              }
            }
            else if(buttonPushed(1)){
              if(animal[randomIndex][2] == '_'){
                if (currentPuzzle.word[2] < 'Z') { 
                  currentPuzzle.word[2]++;  
                } else {
                  currentPuzzle.word[2] = 'A';
                }
                _delay_ms(500);
              }
            }else if(buttonPushed(2)){
              if(animal[randomIndex][3] == '_'){
              if (currentPuzzle.word[3] < 'Z') { 
                currentPuzzle.word[3]++;  
              } else {
                currentPuzzle.word[3] = 'A';
              }
              _delay_ms(500);
              }
            }
          }
        }
      }
      else if(strcmp(currentPuzzle.category, "HMAN") == 0){
        strcpy(currentPuzzle.word, human[randomIndex]);
        currentPuzzle.word[4] = '\0';

        strcpy(answer, correctHuman[randomIndex]);
        answer[4] = '\0';

        //this is working, had to use a local variable to make it work
        strcpy(localAnswer, answer); 

        while(1){
          if(strcmp(localAnswer, currentPuzzle.word) == 0){
            currentPuzzle.attempts++;
            for(int i = 0; i < 10000; i ++){
              writeString(currentPuzzle.word);
            }
            break;
          } else{
            writeString(currentPuzzle.word);
            if(buttonPushed(0)){
              if(human[randomIndex][0] == '_'){
                if (currentPuzzle.word[0] < 'Z') { 
                  currentPuzzle.word[0]++;  
                } else {
                  currentPuzzle.word[0] = 'A';
                }
                _delay_ms(500);
              }
            }
            else if(buttonPushed(1)){
              if(human[randomIndex][2] == '_'){
                if (currentPuzzle.word[2] < 'Z') { 
                  currentPuzzle.word[2]++;  
                } else {
                  currentPuzzle.word[2] = 'A';
                }
                _delay_ms(500);
              }
            }else if(buttonPushed(2)){
              if(human[randomIndex][3] == '_'){
              if (currentPuzzle.word[3] < 'Z') { 
                currentPuzzle.word[3]++;  
              } else {
                currentPuzzle.word[3] = 'A';
              }
              _delay_ms(500);
              }
            }
          }
        }
      }
      else if(strcmp(currentPuzzle.category, "CTRY") == 0){
        strcpy(currentPuzzle.word, country[randomIndex]);
        currentPuzzle.word[4] = '\0';

        strcpy(answer, correctCountry[randomIndex]);
        answer[4] = '\0';

        //this is working, had to use a local variable to make it work
        strcpy(localAnswer, answer); 

        while(1){
          if(strcmp(localAnswer, currentPuzzle.word) == 0){
            currentPuzzle.attempts++;
            for(int i = 0; i < 10000; i ++){
              writeString(currentPuzzle.word);
            }
            break;
          } else{
            writeString(currentPuzzle.word);
            if(buttonPushed(0)){
              if(country[randomIndex][0] == '_'){
                if (currentPuzzle.word[0] < 'Z') { 
                  currentPuzzle.word[0]++;  
                } else {
                  currentPuzzle.word[0] = 'A';
                }
                _delay_ms(500);
              }
            }
            else if(buttonPushed(1)){
              if(country[randomIndex][2] == '_'){
                if (currentPuzzle.word[2] < 'Z') { 
                  currentPuzzle.word[2]++;  
                } else {
                  currentPuzzle.word[2] = 'A';
                }
                _delay_ms(500);
              }
            }else if(buttonPushed(2)){
              if(country[randomIndex][3] == '_'){
              if (currentPuzzle.word[3] < 'Z') { 
                currentPuzzle.word[3]++;  
              } else {
                currentPuzzle.word[3] = 'A';
              }
              _delay_ms(500);
              }
            }
          }
        }
      }
      else if(strcmp(currentPuzzle.category, "THNG") == 0){
        strcpy(currentPuzzle.word, thing[randomIndex]);
        currentPuzzle.word[4] = '\0';

        strcpy(answer, correctThing[randomIndex]);
        answer[4] = '\0';

        //this is working, had to use a local variable to make it work
        strcpy(localAnswer, answer); 

        while(1){
          if(strcmp(localAnswer, currentPuzzle.word) == 0){
            currentPuzzle.attempts++;
            for(int i = 0; i < 10000; i ++){
              writeString(currentPuzzle.word);
            }
            break;
          } else{
            writeString(currentPuzzle.word);
            if(buttonPushed(0)){
              if(thing[randomIndex][0] == '_'){
                if (currentPuzzle.word[0] < 'Z') { 
                  currentPuzzle.word[0]++;  
                } else {
                  currentPuzzle.word[0] = 'A';
                }
                _delay_ms(500);
              }
            }
            else if(buttonPushed(1)){
              if(thing[randomIndex][2] == '_'){
                if (currentPuzzle.word[2] < 'Z') { 
                  currentPuzzle.word[2]++;  
                } else {
                  currentPuzzle.word[2] = 'A';
                }
                _delay_ms(500);
              }
            }else if(buttonPushed(2)){
              if(thing[randomIndex][3] == '_'){
              if (currentPuzzle.word[3] < 'Z') { 
                currentPuzzle.word[3]++;  
              } else {
                currentPuzzle.word[3] = 'A';
              }
              _delay_ms(500);
              }
            }
          }
        }
      }
      break;
    }

    // Stop the timer
    cli();
    currentPuzzle.time = timer_seconds; // save time until guess

    // show result
    initUSART();
    printf("\nIn category %.4s the word %s was guessed in %d attempts and took %d seconds.", localCategory, answer, currentPuzzle.attempts, currentPuzzle.time);
    enableBuzzer();
    playMusicWin();
    disableBuzzer();


    printf("\nTo restart, press button 1!");
    while(1){
      writeString("YES!");
      if(buttonPushed(0)){
        isCategorySelected = 0;
        current_category = 0;
        strcpy(currentPuzzle.category, "");
        break;
      }
    }
    _delay_ms(500);
    gamePlay();
}

int main()
{
    LED_DDR |= _BV(LED1) | _BV(LED2);   // we'll use led1 and led2
    LED_PORT |= _BV(LED1) | _BV(LED2);  // turn 2 leds off
    BUTTON_DDR &= ~_BV(BUTTON1);        // we'll use button1
    BUTTON_PORT |= _BV(BUTTON1);        // enable pull-up
    PCICR |= _BV(PCIE1);                // activate interrupt for port C
    PCMSK1 |= _BV(BUTTON1) | _BV(BUTTON2) | _BV(BUTTON3); // enable pin change interrupt for BUTTON1 and BUTTON2
    initUSART();
    initDisplay();
    printf("\n\nChoose a category: animal (ANML), human (HMAN), country (CTRY) or thing (THNG). You are going to be presented with a riddle: a 4-letter word; letters are masked with an underscore (_). The aim is to guess the right word in as few attempts as possible.\n");
    printf("Press Button 1 to start playing!\n\n");
    printf("Pressing button 1 changes the displayed category.\n");
    printf("With Button 2 you select your category.\n");

    for (int i = 1; i < 4; i++)
    {
        enableButton(i);
    }

    gamePlay();


    return 0;
}