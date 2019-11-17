#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */

#include "exceptions.h"

#include "http.h"

int httppost(char* host, char* httpPath, char* content, char retorno[RETURN_SIZE]) {
    /* first what are we going to send and where are we going to send it? */
    int portno =        80;
    char *message_fmt = "POST /%s HTTP/1.0\r\nHost: %s\r\nContent-Length: %d\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n%s";

    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, received, total;
    char message[1024];


    /* fill in the parameters */
    sprintf(message, message_fmt, httpPath, host, strlen(content), content);
    //printf("Request:\n%s\n",message);

    /* create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        {FUNCTION_ERROR(-10, "Falha ao criar o socket")}
        

    /* lookup the ip address */
    server = gethostbyname(host);
    if (server == NULL) {FUNCTION_ERROR(-11, "Host não encontrado")}

    /* fill in the structure */
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

    /* connect the socket */
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        {FUNCTION_ERROR(-12, "Falha conectando")}

    /* send the request */
    total = strlen(message);
    sent = 0;
    do {
        bytes = write(sockfd,message+sent,total-sent);
        if (bytes < 0)
            {FUNCTION_ERROR(-13, "Falha escrevendo mensagem no socket")}
        if (bytes == 0)
            break;
        sent+=bytes;
    } while (sent < total);

    /* receive the response */
    memset(retorno,0,RETURN_SIZE);
    total = RETURN_SIZE-1;
    received = 0;
    do {
        bytes = read(sockfd,retorno+received,total-received);
        if (bytes < 0)
            {FUNCTION_ERROR(-14, "Falha lendo resposta do socket")}
        if (bytes == 0)
            break;
        received+=bytes;
    } while (received < total);

    if (received == total)
        {FUNCTION_ERROR(-15, "Falha armazenando resposta do socket")}

    /* close the socket */
    close(sockfd);

    /* process response */
    //printf("Response:\n%s\n",response);

    return 0;
}