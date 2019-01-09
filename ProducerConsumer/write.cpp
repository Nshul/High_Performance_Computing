#include <iostream>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <time.h>

using namespace std;

int main(){
    key_t key = ftok("./shared",1);
    srand(time(0));
    int shm_id;
    if((shm_id=shmget(key,sizeof(int),IPC_CREAT|0666))==-1){
        perror("Error in shmget\n");
        exit(1);
    }
    int *sharedVar = (int*)shmat(shm_id,NULL,0);
    *sharedVar=0;
    int i=1;
    bool flag=true;
    while(flag){
        while(*sharedVar!=0)
            sleep(1);
        if(i==11){
            flag=false;
            *sharedVar=-1;
        }else{
            *sharedVar=rand()+1;
            i++;
            cout<<"Generated number:"<<*sharedVar<<"\n";
        }
    }

    int shm_detach;
    if((shm_detach=shmdt(sharedVar))==-1){
        perror("Error in shmdt\n");
        exit(1);
    }

    if(shmctl(shm_id, IPC_RMID, NULL)==-1){
        perror("shmctl error\n");
        exit(1);
    }
}