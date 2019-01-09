#include <iostream>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <cstdio>
#include <cstdlib>
#include <pthread.h>

#define BUFFER_SIZE 10
#define MUTEX 0
#define EMPTY 1
#define FULL 2

using namespace std;

struct sharedMem{
    int arr[BUFFER_SIZE];
    int p_ite;
    int c_ite;
};

struct argStruct{
    sharedMem *ptr;
    int sem_id;
    int noOfProd;
};

void signal(int sem_id,int sem_no){
    sembuf operation;
    operation.sem_num = sem_no;
    operation.sem_op = 1;
    operation.sem_flg = 0;
    if(semop(sem_id,&operation,1)<0){
        perror("Error in sem signal\n");
        exit(1);
    }
}

void wait(int sem_id,int sem_no){
    sembuf operation;
    operation.sem_num = sem_no;
    operation.sem_op = -1;
    operation.sem_flg = 0;
    if(semop(sem_id,&operation,1)<0){
        perror("Error in sem signal\n");
        exit(1);
    }
}

void *producer(void *param){
    argStruct *params;
    params = (argStruct*)param;
    sharedMem *ptr = params->ptr;
    int sem_id = params->sem_id;
    int noOfProd = params->noOfProd;
    for(int i=0;i<noOfProd;i++){
        wait(sem_id,EMPTY);
        wait(sem_id,MUTEX);
            //produce
        ptr->arr[ptr->p_ite] = 200+i;
        cout<<"Produced:"<<200+i<<" at:"<<ptr->p_ite<<"\n";
        ptr->p_ite = (ptr->p_ite+1)%BUFFER_SIZE;

        signal(sem_id,MUTEX);
        signal(sem_id,FULL);
    }
    pthread_exit(NULL);
}

void *consumer(void *param){
    argStruct *params;
    params = (argStruct*)param;
    sharedMem *ptr = params->ptr;
    int sem_id = params->sem_id;
    int noOfProd = params->noOfProd;
    for(int i=0;i<noOfProd;i++){
        wait(sem_id,FULL);
        wait(sem_id,MUTEX);
            //produce
        int data = ptr->arr[ptr->c_ite];
        cout<<"Read:"<<data<<" at:"<<ptr->c_ite<<"\n";
        ptr->c_ite = (ptr->c_ite+1)%BUFFER_SIZE;

        signal(sem_id,MUTEX);
        signal(sem_id,EMPTY);
    }
    pthread_exit(NULL);
}

int main(){
    key_t key = ftok("./sharedMem",1);
    int shm_id,sem_id;
    if((shm_id = shmget(key,sizeof(sharedMem),IPC_CREAT|0666))<0){
        perror("Error in shmget\n");
        exit(1);
    }

    if((sem_id = semget(key,3,IPC_CREAT|0666))<0){
        perror("Error in semget\n");
        exit(1);
    }

    sharedMem* ptr = (sharedMem*)shmat(shm_id,NULL,0);
    if(ptr==(sharedMem*)(-1)){
        perror("Error in shmat\n");
        exit(1);
    }

    ptr->p_ite=ptr->c_ite=0;

    short semarr[3];
    semarr[0] = 1;  //MUTEX
    semarr[1] = BUFFER_SIZE;    //EMPTY
    semarr[2] = 0;  //FULL
    if(semctl(sem_id,1,SETALL,semarr)<0){
        perror("Error while initialising semaphores\n");
        exit(1);
    }

    pthread_t threads[2];

    argStruct ToBePassed;
    ToBePassed.ptr = ptr;
    ToBePassed.sem_id = sem_id;
    ToBePassed.noOfProd = 20;

    pthread_create(&threads[0],NULL,&producer,(void*)&ToBePassed);
    pthread_create(&threads[1],NULL,&consumer,(void*)&ToBePassed);

    pthread_join(threads[0],NULL);
    pthread_join(threads[1],NULL);

    if(shmdt(ptr)<0){
        perror("Error in detaching memory\n");
        exit(1);
    }

    if(shmctl(shm_id,IPC_RMID,NULL)<0){
        perror("Error while deleting shared memory\n");
        exit(1);
    }
    if(semctl(sem_id,1,IPC_RMID)<0){
        perror("Error while removing semaphores\n");
        exit(1);
    }
}