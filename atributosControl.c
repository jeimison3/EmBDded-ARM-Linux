#include "atributosControl.h"
#include "protocolo.h"
#include "gpioPins.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void inicia_controlador_atributos(controlador_atributos * ca){
    ca->i = -1;
}

void add_controlador_atributos(controlador_atributos * ca, atributo atr){
    /*ca->i++;
    atributo* atrb = (atributo*) malloc(sizeof(atributo));
    atrb->classe = atr.classe;
    atrb->dir = atr.dir;
    atrb->indexDimension = atr.indexDimension;
    atrb->isVirtual = atr.isVirtual;
    strcpy(atrb->valor, atr.valor);
    strcpy(atrb->nome, atr.nome);*/
    ca->i++;
    ca->atributos[ca->i] = atr;
}

void libera_id_controlador_atributos(controlador_atributos * ca, int id){
    int itrs =  ca->i - id;
    while(itrs-- > 0){
        ca->atributos[id++] = ca->atributos[id];
    }
    //free(& ca->atributos[id] );
    ca->i--;
}

void preform_newRead_atributo(controlador_atributos * ca, int id){
    atributo a = ca->atributos[id];
    switch (a.classe) {
    case GPIO_REAL:
        if(!a.isVirtual){
            char valor = digitalRead(a.indexDimension) + '0';
            sprintf(ca->atributos[id].valor, "%c%c", valor, '\0');
        }
        break;
    
    default:
        break;
    }
    
}

void publish_controlador_atributos(controlador_atributos * ca, int id, char* ret){
    atributo a = ca->atributos[id];
    sprintf(ret, "%c%s%c%s%c", MESSAGE_CLIENT_PUB_ESTADO, a.nome, MESSAGE_NEXTPARAM, a.valor, MESSAGE_ENDL);
}


/*
void insert_new_atributo(lista_atributos* lista, atributo a){
    atributo* atrb = (atributo*) malloc(sizeof(atributo));
    atrb->classe = a.classe;
    atrb->dir = a.dir;
    atrb->indexDimension = a.indexDimension;
    atrb->isVirtual = a.isVirtual;
    strcpy(atrb->valor, a.valor);
    strcpy(atrb->nome, a.nome);


    listaEncd_atributos* LEnc = (listaEncd_atributos*) malloc(sizeof(listaEncd_atributos));
    LEnc->atrib = atrb;
    LEnc->proximo = NULL;


    listaEncd_atributos* ultimo = lista->fim;
    if(ultimo == NULL){
        lista->inicio = LEnc;
    }else{
        ultimo->proximo = LEnc;
    }
    lista->fim = LEnc;
    lista->tam++;
}

void liberar_atributo_lista(lista_atributos* lista, int id){
    listaEncd_atributos * LEnc = lista->inicio->proximo;
    listaEncd_atributos * LEncAnterior = lista->inicio;
    int i = 1;
    while(i++ < id){
        LEncAnterior = LEnc;
        LEnc = LEnc->proximo;
    }


}*/
