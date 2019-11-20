#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "exceptions.h"
#include "gpioPins.h"
#include "files.h"

/*
* 64 * 4 = 256 pinos -> uint64_t[4]
*/

//uint64_t GPIOSET[4];

void getGPIOBase(int pin, char* retorno){
	sprintf(retorno, "/sys/class/gpio/gpio%d/", pin);
}

void getGPIOStringProp(int pin, const char* propriedade, char* retorno){
    getGPIOBase(pin, retorno);
    strncat(retorno, propriedade, strlen(propriedade));
}


void gpio_setup(){
    // Limpa bits
    /*
    int i=0;
    for(;i<4;i++) GPIOSET[i] = 0;
    */
}

void gpio_free(){
    /* Vou pensar se é útil... */
}

void pinMode(uint32_t pin, GPIO_DIRECTION dir){
    #ifndef NO_GPIO
    if(pin >= MAX_GPIOS){
        FUNCTION_ERROR(-2, "PIN EXCEEDS RANGE")
    }

    char pinStr[200];
    sprintf(pinStr, "%d", pin);
    escreve_arquivo("/sys/class/gpio/export", pinStr);

    char retPath[200];
    getGPIOStringProp(pin, "direction", retPath);
    escreve_arquivo(retPath, (dir ? "in" : "out"));
    #endif
}

void digitalWrite(uint32_t pin, GPIO_VALUE val){
    #ifndef NO_GPIO
    if(pin >= MAX_GPIOS){
        FUNCTION_ERROR(-2, "PIN EXCEEDS RANGE")
    }

    char retPath[200];
    getGPIOStringProp(pin, "value", retPath);
    escreve_arquivo(retPath, (val ? "1" : "0") );
    #endif
}

GPIO_VALUE digitalRead(uint32_t pin){
    #ifndef NO_GPIO
    if(pin >= MAX_GPIOS){
        FUNCTION_ERROR(-2, "PIN EXCEEDS RANGE")
    }
    
    char retPath[200];
    char ret[200];
    getGPIOStringProp(pin, "value", retPath);
    
    #ifndef DEBUG
    le_arquivo(retPath, ret);
    #else
    size_t sizeOut = le_arquivo(retPath, ret);
    printf("digitalRead> sizeRet: %d bytes\n", sizeOut);
    #endif
    
    return (GPIO_VALUE) (ret[0] == '1');
    #else
    return LOW;
    #endif
}