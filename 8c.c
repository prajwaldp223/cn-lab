#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <netdb.h> 
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h> 
 
void error(char *msg) { 
    perror(msg); 
    exit(1); 
} 
 
int main(int argc, char *argv[]) { 
    int sock, length, n; 
    struct sockaddr_in server, from; 
    struct hostent *hp; 
    char buffer[1024]; 
 
    if (argc != 4) { 
        printf("Usage: %s <server> <port> <filename>\n", argv[0]); 
        exit(1); 
    } 
 
    sock = socket(AF_INET, SOCK_DGRAM, 0); 
    if (sock < 0) { 
        error("socket"); 
    } 
 
    server.sin_family = AF_INET; 
    hp = gethostbyname(argv[1]); 
 
    if (hp == 0) { 
        error("Unknown host"); 
    } 
 
    bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length); 
    server.sin_port = htons(atoi(argv[2])); 
    length = sizeof(struct sockaddr_in); 
 

    FILE *fp = fopen(argv[3], "r"); 
    if (fp == NULL) { 
        error("Error opening file"); 
    } 

    while (fgets(buffer, 1024, fp) != NULL) { 
        n = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server, length); 
        if (n < 0) { 
            error("sendto"); 
        } 
 
   
        n = recvfrom(sock, buffer, 1024, 0, (struct sockaddr *)&from, &length); 
        if (n < 0) { 
            error("recvfrom"); 
        } 
 
        buffer[n] = '\0'; 
        printf("Server: %s\n", buffer); 
    } 
 
    fclose(fp); 
    close(sock); 
    return 0; 
} 
