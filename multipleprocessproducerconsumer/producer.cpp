#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <cstdio>
#include <cstdlib>

using namespace std;

void wait(int sem_id,int sem_no){
    sembuf operation;
    operation.sem_num = sem_no;
    operation.sem_op = -1;
    operation.sem_flg = 0;
    if(semop(sem_id,&operation,1)<0){
        perror("Error in wait in producer\n");
        exit(1);
    }
}

void signal(int sem_id,int sem_no){
    sembuf operation;
    operation.sem_num = sem_no;
    operation.sem_op = 1;
    operation.sem_flg = 0;
    if(semop(sem_id,&operation,1)<0){
        cout<<"sem:"<<sem_no<<"\t";
        perror("Error in signal in producer\n");
        exit(1);
    }
}

int main(){
    key_t key;
    int shm_id,sem_id;
    key = ftok("./memory",1);

    if((shm_id = shmget(key,4,0666))<0){
        perror("Error in shmget in producer\n");
        exit(1);
    }

    if((sem_id = semget(key,3,0666))<0){
        perror("Error in semget in producer\n");
        exit(1);
    }

    int *data = (int*)shmat(shm_id,NULL,0);
    if(data == (int*)(-1)){
        perror("Error while attaching shm in producer\n");
        exit(1);
    }

    for(int itr=0;itr<10;itr++){

        // Wait on empty
        wait(sem_id,1);

        // Wait on mutex
        wait(sem_id,0);

        *data = 200+itr;
        cout<<"Produced:"<<*data<<"\n";
        // Signal mutex
        signal(sem_id,0);

        // Signal full
        signal(sem_id,2);
    }

    if(shmdt(data)<0){
        perror("Error while detaching shm in producer\n");
        exit(1);
    }

    return 0;
}