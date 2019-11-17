#ifndef __HTTP_REQ_DEF
#define __HTTP_REQ_DEF

#define RETURN_SIZE 4096

int httppost(char* host, char* httpPath, char* content, char retorno[RETURN_SIZE]);

#endif