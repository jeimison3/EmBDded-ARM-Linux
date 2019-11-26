#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "files.h" // API de arquivos
#include <string.h>
#include "gpioPins.h" // GPIO API
#include "http.h" // Conexão web
#include "protocolo.h"

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


int publishPort(char name[100], TypeAtrib type, GPIO_DIRECTION dir, int sockt){
    char msg[200];
    sprintf(msg, "%c%c%c%s%c", MESSAGE_CLIENT_PUB_NEW_ATRIBUTO, (char)type, ((char)dir)+1, name, MESSAGE_ENDL);
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

    embdded_message msg;
    while((sz = message_read(&msg, retrn, sz))){
        switch (msg.mHeader) {
        case MESSAGE_CLIENT_SET_ESTADO:{
            char porta[40];
            int num;
            char valor[40];
            sscanf( msg.paramsStr[0], "%4s%d", porta, &num );
            sscanf( msg.paramsStr[1], "%s", valor );
            if(! (valor[0] == MESSAGE_RESPONSE_ERROR) ){
                if(msg.atrType == ATRIB_TYPE_BOOL){
                    pinMode(num, OUTPUT);
                    digitalWrite( num, (GPIO_VALUE) (valor[0] == '1') );
                }
            }
            break;
            }
        
        default:
            break;
        }
    }
}

int main(){
    #ifdef DEBUG
    printf("Compilado com -DDEBUG\n");
    #endif
    clock_t timerClk;


    int socket = web_socket_create("localhost", 8000);
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
        printf("\n");

        while(1){
            retrive_mensagens(socket);
            delay(300, timerClk);
        }

        delay(1000, timerClk);
        sprintf(myname, "%c%c", (char) 255, (char) 13);
        int sent = web_socket_write(socket, myname); // Close
    }

    web_socket_close(socket, MY_SHUT_RDWR);

    

    //int res = socket_connect("localhost", 8000, "", retrn);//httppost("localhost", "embdded/socket.php", "a=123", retrn);
    //if(res==0) printf("%s\n", retrn);

    /*
    // Code 1
    pinMode(GPIO(0,2), OUTPUT);
    pinMode(GPIO(0,4), OUTPUT);

    while(1){
        digitalWrite(GPIO(0,2), !digitalRead(GPIO(0,2)) );
        delay(1000, timerClk);
    }
    */


    /*
    // Code 2
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