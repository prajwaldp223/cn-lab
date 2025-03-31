#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
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
int sock, length, fromlen, n; 
struct sockaddr_in server, from; 
char buf[1024]; 
if (argc < 2) { 
fprintf(stderr, "ERROR, no port provided\n"); 
exit(1); 
} 
 
    sock = socket(AF_INET, SOCK_DGRAM, 0); 
    if (sock < 0) { 
        error("Opening socket"); 
    } 
 
    length = sizeof(server); 
    bzero((char *)&server, length); 
    server.sin_family = AF_INET; 
    server.sin_addr.s_addr = INADDR_ANY; 
    server.sin_port = htons(atoi(argv[1])); 
 
    if (bind(sock, (struct sockaddr *)&server, length) < 0) { 
        error("binding"); 
    } 
 
    fromlen = sizeof(struct sockaddr_in); 
    FILE *fp = fopen("received_file.txt", "w"); 
    if (fp == NULL) { 
        error("Error opening file"); 
    } 
 
    while (1) { 
        n = recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&from, &fromlen); 
        if (n < 0) { 
            error("recvfrom"); 
        } 
 
        buf[n] = '\0';  
        printf("Received: %s\n", buf); 
        fprintf(fp, "%s", buf); 
 
        // Send acknowledgment 
        n = sendto(sock, "Received", 8, 0, (struct sockaddr *)&from, fromlen); 
        if (n < 0) { 
            error("sendto"); 
        } 
    } 
 
    fclose(fp); 
    close(sock); 
    return 0; 
} 
