#include<stdio.h> 
#include<sys/types.h> 
#include<sys/socket.h> 
#include<netinet/in.h> 
#include<netdb.h> 
#include<string.h> 
#include <stdlib.h> 
void error(char *msg) 
{ 
 perror(msg); 
 exit(0); 
} 
int main(int argc,char *argv[]) 
{ 
 int sockfd,portno,n; 
 struct sockaddr_in serv_addr; 
 struct hostent *server; 
 char filepath[256],buf[3000]; 
 if(argc < 3) 
 { 
  fprintf(stderr,"usage %s hostname port\n",argv[0]); 
  exit(0); 
 } 
 portno=atoi(argv[2]); 
 sockfd=socket(AF_INET,SOCK_STREAM,0); 
 if(sockfd<0) 
  error("\nerror in opening socket"); 
 printf("\nclient online"); 
 server=gethostbyname(argv[1]); 
 if(server==NULL) 
 { 
  fprintf(stderr,"error ,no such host"); 
  exit(0); 
 } 
 printf("\n server online"); 
 bzero((struct sockaddr_in *)&serv_addr,sizeof(serv_addr)); 
 serv_addr.sin_family=AF_INET; 
 bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length); 
 serv_addr.sin_port=htons(portno); 
 if(connect(sockfd,(struct sockaddr_in*)&serv_addr,sizeof(serv_addr))<0) 
  error("error writing to socket"); 
 printf("\nclient:enter path with filename:\n"); 
 scanf("%s",filepath); 
 n=write(sockfd,filepath,strlen(filepath)); 
 if(n<0) 
  error("\nerror writing to socket"); 
 bzero(buf,3000); 
 n=read(sockfd,buf,2999); 
 if(n<0) 
  error("\nerror reading to socket"); 
 printf("\nclient:displaying from socket"); 
 fputs(buf,stdout); 
 return 0; 
} 
