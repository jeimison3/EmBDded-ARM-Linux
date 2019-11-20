#ifndef __HTTP_REQ_DEF
#define __HTTP_REQ_DEF

#define MESSAGE_MXSIZE 4096

int httppost(char* host, char* httpPath, char* content, char retorno[RETURN_SIZE]);

typedef enum{
    MT_Safe=0, // Stop receiving data for this socket. If further data arrives, reject it.
    AS_Safe, // Stop trying to transmit data from this socket. Discard any data waiting to be sent. Stop looking for acknowledgement of data already sent; don’t retransmit it if it is lost.
    AC_Safe, // Stop both reception and transmission.
} POSIXSafety;

int web_socket_create(char* host, int port_no);
int web_socket_write(int socket, char* msg);
int web_socket_read(int socket, char retorno[RETURN_SIZE]);
int web_socket_close(int socket, POSIXSafety option);

#endif