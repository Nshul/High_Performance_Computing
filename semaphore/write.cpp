#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

using namespace std;

int main()
{
    key_t key = 20;

    int sem_id;
    if ((sem_id = semget(key, 1, IPC_CREAT | 0666)) == -1)
    {
        perror("semget error\n");
        exit(1);
    }
    cout << "Current process id:" << getpid() << "\n";


    sembuf sem_temp;
    sem_temp.sem_num = 0;
    sem_temp.sem_op = 10;
    sem_temp.sem_flg = 0;

    if(semop(sem_id, &sem_temp, 1)==-1){
        cout<<"Error in semop\n";
    }

    int val = semctl(sem_id, 0, GETVAL);
    cout << val<<"\n";
    cout << "Last access process id:" << semctl(sem_id, 0, GETPID) << "\n";

    int a;
    cin>>a;
    if(semctl(sem_id,0,SETVAL,a)<0){
        perror("Error in semctl val set\n");
        exit(1);
    }

    val = semctl(sem_id, 0, GETVAL);
    cout << val<<"\n";
    cout << "Last access process id:" << semctl(sem_id, 0, GETPID) << "\n";

    if(semctl(sem_id,1,IPC_RMID)<0){
        perror("Error while removing semaphores\n");
        exit(1);
    }

    return 0;
}