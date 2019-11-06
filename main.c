#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "files.h"
#include <string.h>

#define MAX_GPIOS 256

#include "gpioPins.h"

#define GPIO(X,Y) ((X*32)+Y)

#define delayMicrosseconds(US, CLK_TIME) \
        CLK_TIME = clock(); \
        while( clock() - CLK_TIME <= US) { }

#define delay(MS, CLK_TIME) \
        delayMicrosseconds(MS*1000, CLK_TIME)

int main(){
    #ifdef DEBUG
    printf("Compilado com -DDEBUG\n");
    #endif
    clock_t timerClk;

    pinMode(GPIO(0,2), OUTPUT);
    pinMode(GPIO(0,4), OUTPUT);

    while(1){
        digitalWrite(GPIO(0,2), !digitalRead(GPIO(0,2)) );
        delay(1000, timerClk);
    }

    /*

    char input[200];
    while(1){
        le_arquivo("/dev/ttyO0", input);
        if(strlen(input) > 0){
            printf("%s", input);
        }
    }

    exit(0);

    

    pinMode(GPIO(1,21), OUTPUT);
    pinMode(GPIO(1,22), OUTPUT);
    pinMode(GPIO(1,23), OUTPUT);
    pinMode(GPIO(1,24), OUTPUT);

    while(1){
        digitalWrite(GPIO(1,21), !digitalRead(GPIO(1,21)) );
        delay(1000, timerClk);
    }
    */
    
    
    return 0;
}