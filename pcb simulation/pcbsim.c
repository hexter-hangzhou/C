#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>





typedef struct pmsg
{
	int spid; // 进程的id;    
    char msg[512];	
	struct pmsg *next;
}pmsg;
typedef struct pcb
{
	int pid; // 进程的id;
    int priority;
    char name[512];
	int state;// 进程的状态
	struct pmsg *msg;    
	struct pcb *next;
}pcb;

void changerunningpcb();
void addreadyender(pcb *p);
void checkmsg(pcb *pt);


pcb *semlist[]={NULL,NULL,NULL,NULL,NULL};
pcb *runningpcb=NULL,*readyheader=NULL,*blockheader=NULL,*init;
int pidnum=0,sem[]={-1,-1,-1,-1,-1};

void preemptive(pcb *p)
{
    printf("preemptive,pid=%d is preemptived.\n",runningpcb->pid);
    if(runningpcb->pid==0){
        runningpcb=p;
        checkmsg(runningpcb);
    p->next=NULL;}
    else{
        addreadyender(runningpcb);
        runningpcb=p;
        checkmsg(runningpcb);
        p->next=NULL;
    }
    
    
    printf("pid=%d now gets control of the CPU.\n",runningpcb->pid);
}

void create(int priority,char *name)
{
    pcb *p = (pcb*)malloc(sizeof(pcb));//
	if(NULL == p)
    {
        printf("create a process failure.\n");
    }else{
        printf("create a process success,pid=%d.\n",pidnum);
        p->pid=pidnum;
        if(priority<0)priority=0;
        if(priority>2)priority=2;
        p->priority=priority;
        p->state=0;
        strcpy( p->name, name);
        p->msg=NULL;
        p->next=NULL;
               
        pidnum+=1;
        if(priority<runningpcb->priority)
        {
            preemptive(p);
        }else{
             addreadyender(p);
        }
       
    }
    
}
void addreadyender(pcb *p)
{
    pcb *pt;
    pt=readyheader;
     if(readyheader==NULL)
       {
           readyheader=p;
        }else
            {
                while(pt->next!=NULL)pt=pt->next;
                pt->next=p;
                p->next=NULL;
            }     
}
void addblockender(pcb *p)
{
    pcb *pt;
    pt=blockheader;
     if(blockheader==NULL)
       {
           blockheader=p;
             
        }else
            {
                while(pt->next!=NULL)pt=pt->next;
                pt->next=p;
                
               }
    p->next=NULL;               
   }
void pfork()
{
    if(runningpcb->pid==0)
    {
         printf("fork a process failure.\n");
         return;
    }  ;
     pcb *p = (pcb*)malloc(sizeof(pcb));//
	if(NULL == p)
    {
        printf("fork a process failure.\n");
    }else{
        printf("fork a process success,pid=%d.\n",pidnum);
        p->pid=pidnum;
       
        p->priority=runningpcb->priority;
        p->state=runningpcb->state;
        strcpy( p->name, runningpcb->name);
        p->msg=NULL;
        p->next=NULL;
        addreadyender(p);
        pidnum+=1;
    }
}
void pkill(int pid)
{
    if(readyheader==NULL&&blockheader==NULL)
       {
           printf("kill a process failure.\n");
           return;
        }
    
    if(pid==0)
    {
         printf("kill a process failure.\n");
         return;
    }  ;    
    pcb *pt,*prept;
    pt=readyheader;
    
    if(readyheader->pid==pid)
            {
                readyheader=readyheader->next;
                free(pt);
                printf("kill a process success,pid=%d.\n",pid);
                return;
            }
    prept=pt;
    pt=pt->next;
    if(listkill(pt,prept,pid)==1)return;
    
    pt=blockheader;
    
    if(blockheader->pid==pid)
            {
                blockheader=blockheader->next;
                free(pt);
                printf("kill a process success,pid=%d.\n",pid);
                return;
            }
    prept=pt;
    pt=pt->next;
    if(listkill(pt,prept,pid)==1)return;
    printf("kill a process failure.\n");
}
int listkill(pcb *pt,pcb *prept,int pid)
{
    while(pt!=NULL)
        {
            if(pt->pid==pid)
            {
                prept->next=pt->next;
                free(pt);
                printf("kill a process success,pid=%d.\n",pid);
                return 1;
            }
            prept=pt;
          pt=pt->next;
          
        }
        return 0;
}
void pexit()
{
    if(runningpcb->pid==0&&readyheader==NULL&&blockheader==NULL)
    {
         printf("App end,bye bye.\n");
         exit(1); 
    };
    
    if(runningpcb->pid==0)
    {   printf("exit a process failure.\n");
        return;
        }
        if(readyheader==NULL)
    { 
        free(runningpcb);
        runningpcb=init;
        printf("exit a process success,pid=%d now gets control of the CPU.\n",runningpcb->pid);
        return;
        }
     free(runningpcb);     
     printf("exit a process success.\n");
     runningpcb=readyheader;
     checkmsg(runningpcb);
     readyheader=readyheader->next;
     printf("exit a process success,pid=%d now gets control of the CPU.\n",runningpcb->pid);
     return;    
}
void  quantum()
{
    
    if(runningpcb->pid==0)
       {
           printf("quantum a process failure.\n");
           return;
        }
        
    addreadyender(runningpcb);       
    
    printf("pid=%d now quantum expires.\n",runningpcb->pid);
    
    changerunningpcb();
    
}
void changerunningpcb()
{
    if(runningpcb->pid==0)
    { 
        
        runningpcb=init;
        printf("pid=%d now gets control of the CPU.\n",runningpcb->pid);
        return;
        }
     addreadyender(runningpcb);
     runningpcb=readyheader;
     checkmsg(runningpcb);
     readyheader=readyheader->next;
     runningpcb->next=NULL;
     printf("pid=%d now gets control of the CPU.\n",runningpcb->pid);
     return;
}
void getrunningpcb()
{
    if(readyheader==NULL)
    { 
        
        runningpcb=init;
        printf("pid=%d now gets control of the CPU.\n",runningpcb->pid);
        return;
        }
     
     runningpcb=readyheader;
     checkmsg(runningpcb);
     readyheader=readyheader->next;
     runningpcb->next=NULL;
     printf("pid=%d now gets control of the CPU.\n",runningpcb->pid);
     return;
}
void checkmsg(pcb *pt)
{
    if(pt->msg!=NULL)printf("You have msg:pid=%d,content:%s\n",pt->msg->spid,pt->msg->msg);
}
 void reply(int argint,char *argstr)
{
     pmsg *smsg = (pmsg*)malloc(sizeof(pmsg));//
     strcpy( smsg->msg, argstr);
     smsg->spid=runningpcb->pid;
     smsg->next=NULL;
     pcb *pt,*prept;
     pt=blockheader;
        prept=pt;
        if(blockheader!=NULL)
        {
            if(blockheader->pid==argint)
            { smsg->next=pt->msg;
                pt->msg=smsg;
                pt->state=0;
                //runningpcb->state=-1;//blocked;
                printf("reply a message success.\n");
                blockheader=blockheader->next;
                addreadyender(pt);
                runningpcb->msg=NULL;
                //addblockender(runningpcb);
                //changerunningpcb();              
                return;}
                
            pt=blockheader->next;
            while(pt!=NULL)
            {
            if(pt->pid==argint)
            {
                smsg->next=pt->msg;
                pt->msg=smsg;
                pt->state=0;
                //runningpcb->state=-1;//blocked;
                printf("reply a message success.\n");
                prept->next=pt->next;
                addreadyender(pt);
                runningpcb->msg=NULL;
                //addblockender(runningpcb);
                //changerunningpcb();               
                
                return;
                }
                else{
                prept=pt;
                pt=pt->next;
                }
            }
        }
        
        printf("reply a message failure.\n");
}
void sendmsg(int argint,char *argstr)
{
    if(argint==runningpcb->pid)
        {
             printf("failure,can not send a message to self.\n");
            return;
        }
         if(argint==0||runningpcb->pid==0)
        {
            printf("failure,can not send a message to init.\n");
            return;
        }
     pmsg *smsg = (pmsg*)malloc(sizeof(pmsg));//
     strcpy( smsg->msg, argstr);
     smsg->spid=runningpcb->pid;
     smsg->next=NULL;
     
     pcb *pt,*prept;
     pt=readyheader;
    while(pt!=NULL)
        {
            if(pt->pid==argint)
            {
                smsg->next=pt->msg;
                pt->msg=smsg;
                
                runningpcb->state=-1;//blocked;
                printf("send a message success.\n");
                addblockender(runningpcb);
                
               getrunningpcb();
                
                return;
            }
        else{
            pt=pt->next;
        }
        }
        
        pt=blockheader;
        prept=pt;
        if(blockheader!=NULL)
        {
            if(blockheader->pid==argint)
            { smsg->next=pt->msg;
                pt->msg=smsg;
                pt->state=0;
                runningpcb->state=-1;//blocked;
                printf("send a message success.\n");
                blockheader=blockheader->next;
                addreadyender(pt);
                addblockender(runningpcb);
                getrunningpcb();              
                return;}
                
            pt=blockheader->next;
            while(pt!=NULL)
            {
            if(pt->pid==argint)
            {
                smsg->next=pt->msg;
                pt->msg=smsg;
                pt->state=0;
                runningpcb->state=-1;//blocked;
                printf("send a message success.\n");
                prept->next=pt->next;
                 addreadyender(pt);
                addblockender(runningpcb);
                getrunningpcb();
                
                
                return;
            }
            else{
                prept=pt;
                pt=pt->next;
                }
            }
        }
        
        printf("failure,pid=%d is not a valid pid.\n",argint);
}
void receivemsg()
{
     if(runningpcb->pid==0)
        {
            printf("failure,init can not blocked.\n");
            return;
        }
        runningpcb->state=-1;//blocked;
                printf("wait message.\n");
                addblockender(runningpcb);
                getrunningpcb();
}
void newsemaphore(int argint, int argint2)
{
  
    if(argint<=4&&argint>=0&&argint2>=0)
    {
         if(NULL==semlist[argint]&&sem[argint]==-1){sem[argint]=argint2; printf("new semaphore success.\n");return;}
         
    }
     printf("new semaphore failed.\n");
     return;
}
void psemaphore(int argint)
{
    if(argint<=4&&argint>=0)
    {
         sem[argint]-=1;
         if(sem[argint]<0)
         {
             printf("pid=%d p semaphore blockde.\n",runningpcb->pid);
             runningpcb->next=semlist[argint];
             semlist[argint]=runningpcb;
             getrunningpcb();
             
         }
         
         printf("p semaphore success.\n");
         return;
    }
     printf("p semaphore failed.\n");
     return;
    }
void vsemaphore(int argint)
{
    pcb *pt;
     if(argint<=4&&argint>=0)
    {
        
         sem[argint]+=1;
         if(sem[argint]==0)
         {
             pt=semlist[argint];
             semlist[argint]=NULL;
             addreadyender(pt);
             
             
         }else if(sem[argint]<0)
         {
             pt=semlist[argint];
          
             
             while(pt->next->next!=NULL)pt=pt->next;
             addreadyender(pt->next);
                pt->next=NULL;
             
         }
         
         printf("v semaphore success.\n");
       
         
    }
     printf("v semaphore failed.\n");
     return;
    }

void printpcb(pcb *pt)
{
     printf("pcb info:\n");
     printf("pid:%d\n",pt->pid);
     printf("state:%d\n",pt->state);
     printf("priority:%d\n",pt->priority);
}
void procinfo(int argint)
{
     pcb *pt;
     if(argint==runningpcb->pid){printpcb(runningpcb);return;}
      if(argint==0){printpcb(init);return;}
     pt=readyheader;
    while(pt!=NULL)
        {
            if(pt->pid==argint)
            {   printpcb(pt);
                return;
            }
            pt=pt->next;
        }
         pt=blockheader;
    while(pt!=NULL)
        {
            if(pt->pid==argint)
            {
                printpcb(pt);
                return;
            }
            pt=pt->next;
        }
         printf("failed,pid is wrong .\n");
}    

void  totalinfo()
{
    pcb *pt;
    printf("===running pcb====.\n");
      printpcb(runningpcb);
     printf("===init pcb====.\n");
      printpcb(init);
       printf("===ready pcb====.\n");
     pt=readyheader;
    while(pt!=NULL)
        {
            printpcb(pt);
            pt=pt->next;
        }
        printf("===blocked pcb====.\n");
         pt=blockheader;
    while(pt!=NULL)
        {
            printpcb(pt);
            pt=pt->next;
        }
        printf("===Sempahore Blocked====.\n");
        int i;
        for(i=0;i<=4;i++)
            {
                pt=semlist[i];
            while(pt!=NULL)
            {
                printpcb(pt);
                pt=pt->next;
                }
           }
    
}
int main(int argc, char **argv)
{        int i=0;
        char cc,kbBuff[512],argstr[128],argcc;
        int argint,argint2;
        init = (pcb*)malloc(sizeof(pcb));//
        init->pid=0;

        init->priority=3;
        init->state=0;
        strcpy( init->name, "init");
        init->msg=NULL;
        init->next=NULL;
        
        pidnum+=1;
        runningpcb=init;
        printf("MY PCBSimulator Start:\nnow please input first command:\n");
        while(1)//main loop
            {
        fgets(kbBuff,511,stdin);              
        for(i=0;i<511;i++)
        {
            if(kbBuff[i]==' ')
            {
                continue;
                }else {
                    cc=kbBuff[i];
                    break;
                };
           }          
           for(i=0;i<511;i++)
        {
            if(kbBuff[i]=='\n')
            {       kbBuff[i]=0;      
                    break;
                    };
        }
                switch(cc)
                    {
                        case 'C':
                           
                            sscanf( kbBuff, "%c %d", &argcc, &argint);
                            create(argint,"user process");
                        break;
                        case 'F':
                            
                            //sscanf( kbBuff, "%c %d %s", &argcc, &argint, argstr);
                            pfork();
                        break;
                        case 'K':
                            
                            sscanf( kbBuff, "%c %d", &argcc, &argint);
                            pkill(argint);
                        break;
                        case 'E':
                            
                            //sscanf( kbBuff, "%c %d %s", &argcc, &argint, argstr);
                            pexit();
                        break;
                        case 'Q':
                            
                            //sscanf( kbBuff, "%c %d %s", &argcc, &argint, argstr);
                            quantum();
                        break;
                        case 'S':
                            
                            sscanf( kbBuff, "%c %d %s", &argcc, &argint, argstr);
                            sendmsg(argint,argstr);
                        break;
                        case 'R':
                            
                            //sscanf( kbBuff, "%c %d %s", &argcc, &argint, argstr);
                            receivemsg();
                        break;
                        case 'Y':
                            
                            sscanf( kbBuff, "%c %d %s", &argcc, &argint, argstr);
                            reply(argint, argstr);
                        break;
                        case 'N':
                            
                            sscanf( kbBuff, "%c %d %d", &argcc, &argint,  &argint2);
                            newsemaphore(argint, argint2);
                        break;
                        case 'P':
                            
                            sscanf( kbBuff, "%c %d", &argcc, &argint);
                            psemaphore( argint);
                        break;
                        case 'V':
                            
                            sscanf( kbBuff, "%c %d", &argcc, &argint );
                            vsemaphore( argint);
                        break;
                        case 'I':
                            
                            sscanf( kbBuff, "%c %d", &argcc, &argint);
                            procinfo(argint);
                        break;
                        case 'T':
                            
                            //sscanf( kbBuff, "%c %d %s", &argcc, &argint, argstr);
                            totalinfo();
                        break;
                    }
                     printf("---------------\n      please input this step command:\n");
            }
        
                    printf("cmd:%c,pid:%d,msg:%s\n",argcc,argint,argstr);
return 0;
}
