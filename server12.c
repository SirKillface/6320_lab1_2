//Paul Dalton pld0016@auburn.edu
//Ginny Genge vgg0003@auburn.edu
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>

#define PORT 10020
#define MAXLINE 4096
#define LISTENQ 8

int main(int argc, char *argv){

    int sockfd, newSockfd, n;
    int a, b, result, valid;
    int flag = 0;
    char c;
    char buf[MAXLINE];

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0){
        perror("error opening socket");
        exit(1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if( bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        perror("error binding");
        exit(2);
    }


    listen(sockfd, LISTENQ);
    printf("%s\n", "Server running...waiting for connections");

    clilen = sizeof(cli_addr);
    while(1){
        newSockfd = accept(sockfd, (struct sockaddr *) &cli_addr, & clilen);

        if(newSockfd < 0){
            perror("accept error");
            exit(3);
        }


        bzero(buf, MAXLINE);
        n = recv(newSockfd, buf, sizeof(int), 0);
        buf[n] = 0;
        a = atoi(buf);

        bzero(buf, MAXLINE);
        n = recv(newSockfd, buf, sizeof(int), 0);
        buf[n] = 0;
        b = atoi(buf);


        bzero(buf, MAXLINE);
        n = recv(newSockfd, buf, sizeof(buf), 0);
        buf[n] = 0;
        c = *buf;



        if((a == 0) || (b ==0)){
            if(c == '/'){
                flag = 1;
            }
        }

        else if((b > a) && c == '-'){
            flag = 1;
        }

        if(flag == 0){
            if(c == '+'){
                result = a + b;
                valid = 1;
            }
            else if(c == '-'){
                result = a - b;
                valid = 1;
            }
            else if(c == 'x'){
                result = a * b;
                valid = 1;
            }
            else if(c == '/'){
                result = a / b;
                valid = 1;
            }
        }

        if (flag == 1){
            result = 0;
            valid = 2;
        }

        printf("%d%s%c%s%d%s%d\n", a, " ", c, " ", b, " = ", result);

        send(newSockfd, &c, sizeof(uint32_t), 0);
        send(newSockfd, &a, sizeof(uint32_t), 0);
        send(newSockfd, &b, sizeof(uint32_t), 0);
        send(newSockfd, &result, sizeof(uint32_t), 0);
        send(newSockfd, &valid, 1, 0);
        flag = 0;
    }
    close(newSockfd);
    close(sockfd);

    return 0;
}