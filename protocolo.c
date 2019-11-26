#include "http.h"
#include "protocolo.h"
#include <string.h>

#ifdef DEBUG
#include "stdio.h"
#endif

int hasNext(char* inp, int len){
    int i=0;
    while(i < len){
        if(inp[i++] == MESSAGE_ENDL) return i;
    }
    return 0;
}

//sprintf(teste, "%c%s%c%s%c", MESSAGE_CLIENT_PUB_NEW_ATRIBUTO, "Teste1", MESSAGE_NEXTPARAM, "Teste2", MESSAGE_ENDL );
int message_read(embdded_message * msg, char * inp, int len){
    int msgLen;
    if((msgLen = hasNext(inp, len))){
        char inpMsg[msgLen];
        inpMsg[msgLen-1] = '\0';
        strncpy(inpMsg, inp, msgLen-1);
        memmove(inp, &inp[msgLen], len-msgLen);
        //inp = &inp[msgLen];


        msg->mHeader = (message_type) inpMsg[0];
        #ifdef DEBUG
        printf("Type: %d\n", msg->mHeader);
        #endif

        int initPArgs = 1; // idx do inicio dos atributos

        switch(msg->mHeader){
            case MESSAGE_CLIENT_PUB_NEW_ATRIBUTO: // Não deveria vir, mas vai que
            case MESSAGE_CLIENT_SET_ESTADO:
                msg->atrType = (atrib_type) inpMsg[1];
                #ifdef DEBUG
                printf("AtribType: %d\n", msg->atrType);
                #endif
                initPArgs = 2; // Inicia do [2]
                break;
            default:
                initPArgs=1;
        }

        int q = 0;
        char* lch = inpMsg+initPArgs; // char* atrInicio
        char* pch = strchr(inpMsg+initPArgs, MESSAGE_NEXTPARAM); // char* atrProximo
        while(pch!=NULL){

            strncpy(msg->paramsStr[q], lch, pch-lch );
            msg->paramsStr[q][pch-lch] = '\0';

            #ifdef DEBUG
            printf("Arg[%d]: %s\n", q, msg->paramsStr[q]);
            #endif
            

            lch = pch; // Último válido
            pch = strchr(pch+1, MESSAGE_NEXTPARAM); // Próximo
            q++; //i++
        }

        // Preenche com ultimo válido
        strncpy(msg->paramsStr[q], lch, msgLen - (inpMsg-lch) -1 );
        msg->paramsStr[q][msgLen - (inpMsg-lch)-1] = '\0';
        
        #ifdef DEBUG
        printf("Arg[%d]: %s\n", q, msg->paramsStr[q]);
        #endif
        q++;
        #ifdef DEBUG
        printf("ArgQ: %d\n", q);
        #endif
        return len-msgLen;
    }
    return 0;
}