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

int main(int argc, char **argv){
    int sockfd, n;
    int a, b, result, valid;
    char choice;
    struct sockaddr_in serv_addr;
    char sendLine[MAXLINE], recvLine[MAXLINE];


    if(argc != 4){
        perror("need more arguments 2 ints and a operator");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0){
        perror("Error opening socket");
        exit(2);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        perror("Problem in connecting to the server\n");
        exit(3);
    }

    bzero(sendLine, MAXLINE);

    send(sockfd, argv[1], sizeof(uint32_t), 0);
    send(sockfd, argv[2], sizeof(uint32_t), 0);
    send(sockfd, argv[3], sizeof(char), 0);


    recv(sockfd, &choice, sizeof(int), 0);
    recv(sockfd, &a, sizeof(int), 0);
    recv(sockfd, &b, sizeof(int), 0);
    recv(sockfd, &result, sizeof(int), 0);
    recv(sockfd, &valid, sizeof(int), 0);

    if (valid ==1){
        printf("%s%d%s%c%s%d%s%d%s\n", "returned message ", a, " ", choice, " ", b, " = ", result, " is valid");
    }
    else{
        printf("%s\n", "is not a valid request");
    }

    close(sockfd);

    return 0;
}