#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "gpioPins.h"

#define GPIO(X,Y) ((X*32)+Y)

int main(){
    pinMode(GPIO(1,21), OUTPUT);
    pinMode(GPIO(1,22), OUTPUT);
    pinMode(GPIO(1,23), OUTPUT);
    pinMode(GPIO(1,24), OUTPUT);

    
    return 0;
}