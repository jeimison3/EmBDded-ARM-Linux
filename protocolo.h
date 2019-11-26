#ifndef __PROTOCOLO_EmBDded
#define __PROTOCOLO_EmBDded

#include "http.h"

typedef enum{
    MESSAGE_UNKNOWN = (char) 0,
    MESSAGE_CLIENT_NAME = (char) 1,
    MESSAGE_CLIENT_RECV_SELF_ESTADO = (char) 5,
    MESSAGE_CLIENT_PUB_NEW_ATRIBUTO = (char) 6,
    MESSAGE_CLIENT_SET_ESTADO = (char) 7,
    MESSAGE_CLIENT_SET_EXPORT = (char) 8,
    MESSAGE_CONNECTION_CLOSE = (char) 255,
    MESSAGE_ENDL = (char) 13,
    MESSAGE_NEXTPARAM = (char) 14,
    MESSAGE_RESPONSE_ERROR = (char) 16
} message_type;

typedef enum{
    ATRIB_TYPE_BOOL = (char) 1,
    ATRIB_TYPE_INT = (char) 2,
    ATRIB_TYPE_STR = (char) 3,
    ATRIB_TYPE_FLOAT = (char) 4,
    ATRIB_TYPE_VOID = (char) 5,
} atrib_type;

typedef struct{
    message_type mHeader;
    int needExport;
    atrib_type atrType;
    char paramsStr[3][40];
} embdded_message;

int message_read(embdded_message * msg, char * inp, int len);


#endif