#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "files.h" // API de arquivos
#include <string.h>
#include "gpioPins.h" // GPIO API
#include "http.h" // Conexão web

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

    char retrn[MESSAGE_MXSIZE] = {0};

    int socket = web_socket_create("localhost", 8002);
    if(socket != -1){
        size_t sz = web_socket_read(socket, retrn);
        if(sz>0) printf("R: %s\n", retrn);
    }
    char DADOS[] = {13, 0};
    web_socket_write(socket, DADOS);

    web_socket_close(socket, AC_Safe);

    

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