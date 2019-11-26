#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "files.h" // API de arquivos
#include <string.h>
#include "gpioPins.h" // GPIO API
#include "http.h" // Conexão web
#include "protocolo.h" // Operações byte-a-byte
#include "atributosControl.h"

typedef enum {
    atrBOOL = 1,
    atrINT = 2,
    atrSTR = 3,
    atrFLOAT = 4,
    atrVOID = 5,
} TypeAtrib;

#define GPIO(X,Y) ((X*32)+Y)

#define delayMicrosseconds(US, CLK_TIME) \
        CLK_TIME = clock(); \
        while( clock() - CLK_TIME <= US) { }

#define delay(MS, CLK_TIME) \
        delayMicrosseconds(MS*1000, CLK_TIME)

#ifndef CLIENT_NAME
    #define CLIENT_NAME "GUESTBOARD"
#endif


char retrn[MESSAGE_MXSIZE] = {0};
controlador_atributos atributos;


int publishPort(char name[100], TypeAtrib type, GPIO_DIRECTION dir, int sockt){
    char msg[200];
    sprintf(msg, "%c%c%c%s%c", MESSAGE_CLIENT_PUB_NEW_ATRIBUTO, (char)type, (char) (dir ? 2 : 1) , name, MESSAGE_ENDL);
    return web_socket_write(sockt, msg);
}

int require_estado(int socket, int gpio){
    char name[40 + 2];
    sprintf(name, "%cGPIO%d%c", MESSAGE_CLIENT_RECV_SELF_ESTADO, gpio, MESSAGE_ENDL);
    return web_socket_write(socket, name);
}

void retrive_mensagens(int socket){
    int sz = web_socket_read(socket, retrn);
    if(sz <= 0) return;

    printf("RECEBIDO + %d BYTES\n",sz);

    embdded_message msg;
    while( (sz = message_read(&msg, retrn, sz)) >=0 ){
        switch (msg.mHeader) {
        case MESSAGE_CLIENT_SET_ESTADO:{
            
            char porta[40];
            int num;
            sscanf( msg.paramsStr[0], "%4s%d", porta, &num );
            if(! (msg.paramsStr[1][0] == MESSAGE_RESPONSE_ERROR) ){
                if(msg.atrType == ATRIB_TYPE_BOOL){
                    if(msg.needExport){
                        atributo atr;
                        atr.dir = INPUT;
                        atr.indexDimension = num;
                        atr.classe = GPIO_REAL;
                        atr.isVirtual = 0;
                        strcpy(atr.nome, msg.paramsStr[0]);
                        strcpy(atr.valor, msg.paramsStr[1]);

                        add_controlador_atributos(&atributos, atr);

                        pinMode(num, INPUT);

                    } else {
                        int i=0;
                        for(;i<=atributos.i;i++){
                            if(atributos.atributos[i].indexDimension == num){
                                libera_id_controlador_atributos(&atributos, i);
                            }
                        }
                        pinMode(num, OUTPUT);
                        digitalWrite( num, (GPIO_VALUE) (msg.paramsStr[1][0] == '1') );
                    }
                }
            }
            break;
        }
        case MESSAGE_CLIENT_SET_EXPORT:{
            char porta[40];
            int num;
            sscanf( msg.paramsStr[0], "%4s%d", porta, &num );
            if(msg.atrType == ATRIB_TYPE_BOOL){
                atributo atr;
                atr.dir = INPUT;
                atr.indexDimension = num;
                atr.classe = GPIO_REAL;
                atr.isVirtual = 0;
                strcpy(atr.nome, msg.paramsStr[0]);
                strcpy(atr.valor, msg.paramsStr[1]);

                add_controlador_atributos(&atributos, atr);
                
                pinMode(num, INPUT);
            }
            break;
        }
        
        default:
            printf("FALHA HEADER %d\n",msg.mHeader);
            break;
        }

    }

}

int main(){
    #ifdef DEBUG
    printf("Compilado com -DDEBUG\n");
    #endif
    clock_t timerClk;

    inicia_controlador_atributos(&atributos);

    printf("Sz: %d\n", atributos.i+1);


    int socket = web_socket_create("localhost", 1000, 8000);
    if(socket != -1){
        
        char myname[300];
        sprintf(myname, "%c%s%c", (char) 1, CLIENT_NAME, MESSAGE_ENDL);
        web_socket_write(socket, myname);

        //int sz = web_socket_read(socket, retrn);
        //if(sz>0) printf("R: %s (%d bytes)\n", retrn, sz);

        char name[40];

        int L,C;
        #ifdef BOARD_BEAGLEBONE
        for(L=0; L < 4; L++ ){
            for(C = 0; C < 32; C++){
                sprintf(name, "GPIO%d", GPIO(L,C));
                publishPort(name, atrBOOL, OUTPUT, socket);
                require_estado(socket, GPIO(L,C));
                //printf("%s> %s\n", name, (ret? "OK" : "FAILED"));
            }
        }
        #elif defined NOBOARD
        for(L=0; L < 1; L++ ){
            for(C = 0; C < 32; C++){
                sprintf(name, "GPIO%d", GPIO(L,C));
                publishPort(name, atrBOOL, OUTPUT, socket);
                require_estado(socket, GPIO(L,C));
            }
        }
        #endif

        while(1){
            timerClk = clock(); 
            while( clock() - timerClk <= 500){
                retrive_mensagens(socket);
            }
            //printf("Atributos pub: %d\n", atributos.i+1);
            int i;
            for(i=0; i <= atributos.i; i++){
                preform_newRead_atributo(&atributos, i);
                char pca[100];
                publish_controlador_atributos(&atributos, i, pca);
                #ifdef DEBUG
                //printf("MSG: ");
                //dump_chars(pca, 100);
                //printf("\n");
                #endif
                web_socket_write(socket, pca);
            }

            //delay(300, timerClk);
        }
        delay(1000, timerClk);
        sprintf(myname, "%c%c", (char) 255, (char) 13);
        int sent = web_socket_write(socket, myname); // Close
    }

    web_socket_close(socket, MY_SHUT_RDWR);
    
    return 0;
}