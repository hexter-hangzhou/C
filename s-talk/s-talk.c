#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "list.h"
LIST *getList,*sendList;
char kbBuff[512]="Hi";
char uBuff[512]="Hi";
char **args;

void* getKeyBoard()
{   
char* p; 

do{
        fgets(kbBuff,511,stdin);
       
        int i=0;
        for(i=0;i<511;i++)
        {if(kbBuff[i]=='\n')
            {
                //buff[i]='\0';
        kbBuff[i+1]=0;      
        break;};
            }
            p=(char*)malloc(strlen(kbBuff)+1);
            strcpy(p,kbBuff);
            ListAppend(sendList,p);
    }while(strcmp("!\n", kbBuff) !=0);
   
}

void* printUDP()
{   
    char* temp_data="hi";
    
while(strcmp("!", temp_data) !=0)
    {
        if(ListCount(getList)==0)
        {
            sched_yield();
            continue;
        }
        temp_data=ListLast(getList);
        printf("%s:%s\n",args[2],temp_data);
        ListRemove(getList);
    }
    printf("bye,bye\n");
}

void* getUDP()
{
     char* p; 
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(args[1]));
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    int sock;
    if ( (sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(1);
    }
   
    struct sockaddr_in clientAddr;
    int n;
    int len = sizeof(clientAddr);
    do
    {
        n = recvfrom(sock, uBuff, 511, 0, (struct sockaddr*)&clientAddr, &len);
        if (n>0)
        {
            uBuff[n-1] = 0;
            p=(char*)malloc(strlen(uBuff)+1);
            strcpy(p,uBuff);
            ListAppend(getList,p);
        }
        else
        {
            perror("recv");
            break;
        }
    }while (strcmp("!", uBuff) !=0 );
   
     close(sock);
}

void* sendUDP()
{  struct sockaddr_in addr;
    int sock;

    if ( (sock=socket(AF_INET, SOCK_DGRAM, 0)) <0)
    {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(args[3]));
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (addr.sin_addr.s_addr == INADDR_NONE)
    {
        printf("Incorrect ip address!");
        close(sock);
        exit(1);
    }

    char *temp_data="hi";
    int len = sizeof(addr);
    do
    {
       
      
        //getKeyBoard();
        if(ListCount(sendList)==0)
        {
            sched_yield();
            continue;
        }
        temp_data=ListLast(sendList);
           
       
        int n;
        n = sendto(sock, temp_data, strlen(temp_data), 0, (struct sockaddr *)&addr, sizeof(addr));
        if (n < 0)
        {
            perror("sendto");
            close(sock);
            break;
        }
        ListRemove(sendList);
    }while(strcmp("!\n", temp_data) !=0);
     close(sock);
    return 0;
}

int main(int argc, char **argv)
{
    args=argv;
    pthread_t getKeyBoard_t,getUDP_t,printUDP_t,sendUDP_t;
    getList = ListCreate();
    sendList= ListCreate();
    if (argc != 4)
    {
        printf("Usage: %s [my port number] [remote machine name] [remote port number] \n", argv[0]);
        exit(1);
    }
    printf("Welcome! This is s-talk APP\n");
    int ret = pthread_create(&getUDP_t,NULL,getUDP,NULL);
if(ret != 0)
printf("Create getUDP_t error!\n");

ret = pthread_create(&sendUDP_t,NULL,sendUDP,NULL);
if(ret != 0)
printf("Create sendUDP_t error!\n");


ret = pthread_create(&getKeyBoard_t,NULL,getKeyBoard,NULL);
if(ret != 0)
printf("Create getKeyBoard_t error!\n");


ret = pthread_create(&printUDP_t,NULL,printUDP,NULL);
if(ret != 0)
printf("Create printUDP_t error!\n");



 pthread_exit(NULL);

return 0;
   

}  
