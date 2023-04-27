#include <func.h>
#define NUM 10000000

int main()
{
    int shmid = shmget(IPC_PRIVATE, 4096, IPC_CREAT|0600);
    ERROR_CHECK(shmid, -1, "shmget");
    int *p = (int*)shmat(shmid, NULL, 0);
    p[0] = 0;
    bool wants_to_enter[2] = {false, false};
    int turn = 0;
    if(fork() == 0){
        wants_to_enter[0] = true;
        while (wants_to_enter[1]){
            if (turn != 0){
                wants_to_enter[0] = false;
                while (turn != 0){
                    sleep(1);
                }
                wants_to_enter[0] = true;
            }
        }
        for (int i = 0; i < NUM; ++i){
            ++p[0];
        }
        printf("%d\n", p[0]);
        turn = 1;
        wants_to_enter[0] = false;
    } else {
        wants_to_enter[1] = true;
        while (wants_to_enter[0]){
            if (turn != 1){
                wants_to_enter[1] = false;
                while (turn != 1){
                    sleep(1);
                }
                wants_to_enter[1] = true;
            }
        }
        for (int i = 0; i < NUM; ++i){
            ++p[0];
        }
        printf("%d\n", p[0]);
        turn = 0;
        wants_to_enter[1] = false;
        wait(NULL);
        printf("sum = %d\n", p[0]);
    }
}

