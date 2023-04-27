#include<func.h>
#define NUM 10000000

int main()
{
    int shmid=shmget(IPC_PRIVATE,4096,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget");
    int* p=(int*)shmat(shmid,NULL,0);
    p[0]=0;//wants to enter[0]
    p[1]=0;//wants to enter[1]
    p[2]=0;//turn
    p[3]=0;//操作的加数
    if(fork()==0)
    {
        p[1]=1;
        while(p[0])
        {
            if(p[2]==0)
            {
                p[1]=0;
                while(p[2]==0);
                p[1]=1;
            }
        }
        for(int i=0;i<NUM;i++)
        {
            p[3]++;
        }
        p[1]=0;
        p[2]=0;
    }
    else
    {
        p[0]=1;
        while(p[1])
        {
            if(p[2]==1)
            {
                p[0]=0;
                while(p[2] != 0);
                p[0]=1;
            }
        }
        for(int i=0;i<NUM;i++)
        {
            p[3]++;       
        }
        p[0]=0;
        p[2]=1; 
        wait(NULL);
        printf("%d\n",p[3]);
    }
    shmdt(p);
}
