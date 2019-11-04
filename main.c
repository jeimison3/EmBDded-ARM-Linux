#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "gpioPins.h"

#define GPIO(X,Y) ((X*32)+Y)

#define delayMicrosseconds(US, CLK_TIME) \
        CLK_TIME = clock(); \
		while( clock() - CLK_TIME <= US) { }

#define delay(MS, CLK_TIME) \
        delayMicrosseconds(MS*1000, CLK_TIME)

int main(){
    clock_t timerClk;
    pinMode(GPIO(1,21), OUTPUT);
    pinMode(GPIO(1,22), OUTPUT);
    pinMode(GPIO(1,23), OUTPUT);
    pinMode(GPIO(1,24), OUTPUT);

    while(1){
        digitalWrite(GPIO(1,21), !digitalRead(GPIO(1,21)) );
        delay(1000, timerClk);
    }
    
    return 0;
}