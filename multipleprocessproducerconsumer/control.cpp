#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

using namespace std;

int main(){
    key_t key = ftok("./memory",1);
    int shm_id,sem_id;

    if((shm_id=shmget(key,4,IPC_CREAT|0666))<0){
        perror("Error in semget\n");
        exit(1);
    }

    if((sem_id = semget(key,3,IPC_CREAT|0666))<0){
        perror("Error while creating mutex\n");
        exit(1);
    }
    // 3 semaphores because
    // 0-> mutex, 1->empty, 2->full 

    short semarray[3];
    semarray[0] = semarray[1] = 1; //
    semarray[2] = 0;

    // Here 1 has no relevance since SETALL is used all semaphores are set
    if(semctl(sem_id,1,SETALL,semarray)<0){
        perror("Error while initialising semaphores\n");
        exit(1);
    }

    sleep(15);

    if(semctl(sem_id,1,IPC_RMID)<0){
        perror("Error while removing sempahores\n");
        exit(1);
    }
    if(shmctl(shm_id,IPC_RMID,NULL)<0){
        perror("Error while removing shared memory\n");
        exit(1);
    }
    return 0;
}