/*
* Biblioteca desenvolvida por Jeimison Moreno.
* Controle de GPIO baseado em arquivos, por meio de device drivers.
*/
#ifndef __GPIOController
#define __GPIOController
    #include <stdint.h>

    #ifdef BOARD_BEAGLEBONE
        #define MAX_GPIOS 128
    #elif defined BOARD_RASPI3
        #define MAX_GPIOS 27
    #elif defined NOBOARD
        #define NO_GPIO
    #endif

    #ifndef MAX_GPIOS
        #define MAX_GPIOS 0
    #endif

    typedef enum{
        OUTPUT = 0,
        INPUT
    } GPIO_DIRECTION;

    typedef enum{
        LOW = 0,
        HIGH
    } GPIO_VALUE;

    /*
    Define direção da porta GPIO.
    @param pin: 0..MAX_GPIOS
    @param dir: INPUT|OUTPUT
    */
    int pinMode(uint32_t pin, GPIO_DIRECTION dir);

    /*
    Escrita digital no pino especificado.
    @param pin: unsigned 0..MAX_GPIOS
    @param val: HIGH|LOW
    */
    void digitalWrite(uint32_t pin, GPIO_VALUE val);


    /*
    Leitura digital do pino especificado.
    @param pin: unsigned 0..MAX_GPIOS
    @return HIGH|LOW
    */
    GPIO_VALUE digitalRead(uint32_t pin);
    
#endif