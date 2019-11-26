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

void dump_chars(char* linha, int maxLen){
    int i = 0;
    while(i < maxLen && linha[i] != '\0'){
        printf("[%d]",linha[i++]);
    }
}

//sprintf(teste, "%c%s%c%s%c", MESSAGE_CLIENT_PUB_NEW_ATRIBUTO, "Teste1", MESSAGE_NEXTPARAM, "Teste2", MESSAGE_ENDL );
int message_read(embdded_message * msg, char * inp, int len){
    if(len == 0) return -1;
    
    int msgLen;
    if((msgLen = hasNext(inp, len))){
        char inpMsg[msgLen];
        inpMsg[msgLen-1] = '\0';
        strncpy(inpMsg, inp, msgLen-1);
        memmove(inp, &inp[msgLen], len-msgLen);
        //inp = &inp[msgLen];

        #ifdef DEBUG
        printf("RLine: %s | SIZE: %d\n", inpMsg, msgLen);
        #endif


        msg->mHeader = (message_type) inpMsg[0];
        #ifdef DEBUG
        printf("Type: %d\n", msg->mHeader);
        #endif

        int initPArgs = 1; // idx do inicio dos atributos

        switch(msg->mHeader){
            case MESSAGE_CLIENT_PUB_NEW_ATRIBUTO: // Não deveria vir, mas vai que
            case MESSAGE_CLIENT_SET_ESTADO:
                msg->atrType = (atrib_type) inpMsg[1];
                msg->needExport = inpMsg[2] -1;
                #ifdef DEBUG
                printf("Export: %d\n", msg->needExport);
                printf("AtribType: %d\n", msg->atrType);
                #endif
                initPArgs = 3; // Inicia do [3]
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
            printf("Arg[%d]: ", q);
            dump_chars(msg->paramsStr[q], 40);
            printf("\n");
            #endif
            

            lch = pch; // Último válido
            pch = strchr(pch+1, MESSAGE_NEXTPARAM); // Próximo
            q++; //i++
        }

        // Preenche com ultimo válido
        strncpy(msg->paramsStr[q], lch+1, msgLen - (inpMsg-lch) -1 );
        msg->paramsStr[q][msgLen - (inpMsg-lch)-1] = '\0';
        
        #ifdef DEBUG
        printf("Arg[%d]: %s\n", q, msg->paramsStr[q]);
        printf("Arg[%d]: ", q);
        dump_chars(msg->paramsStr[q], 40);
        printf("\n");
        #endif
        
        q++;
        #ifdef DEBUG
        printf("ArgQ: %d\n\n", q);
        #endif
        //if(len == msgLen) return -1;
        return len-msgLen;
    }
    return -1;
}