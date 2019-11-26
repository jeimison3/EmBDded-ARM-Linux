#ifndef __ATRIB_CONTROLL
#define __ATRIB_CONTROLL

#include "gpioPins.h"

typedef enum{
    GPIO_REAL,
    I2C_DS3231_VIRTUAL
} AttribClass;

typedef struct{
    GPIO_DIRECTION dir; // INPUT -> Exportar | OUTPUT -> Receber
    char nome[40];
    int indexDimension; // ID útil
    int isVirtual; // 0-> GPIO | 1-> I2C/SPI/...
    AttribClass classe;
    char valor[40];
} atributo;

typedef struct{
    atributo atributos[20];
    int i;
} controlador_atributos;

void inicia_controlador_atributos(controlador_atributos * ca);
void add_controlador_atributos(controlador_atributos * ca, atributo atr);
void libera_id_controlador_atributos(controlador_atributos * ca, int id);

void preform_newRead_atributo(controlador_atributos * ca, int id);
void publish_controlador_atributos(controlador_atributos * ca, int id, char* ret);


/*
typedef struct{
    atributo* atrib;
    listaEncd_atributos* proximo;
} listaEncd_atributos;

typedef struct{
    listaEncd_atributos* inicio;
    listaEncd_atributos* fim;
    int tam;
} lista_atributos;*/


#endif