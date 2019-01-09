#include <iostream>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

using namespace std;

int main(){
    key_t key = ftok("./shared",1);

    int shm_id;
    if((shm_id=shmget(key,sizeof(int),0666))==-1){
        perror("Error in shmget\n");
        exit(1);
    }
    int *sharedVar = (int*)shmat(shm_id,NULL,0);
    
    bool flag=true;
    while(flag){
        while(*sharedVar==0)
            sleep(1);
        if(*sharedVar<0)
            flag=false;
        else{
            cout<<"Received number:"<<*sharedVar<<"\n";
            *sharedVar = 0;
        }
    }

    int shm_detach;
    if((shm_detach=shmdt(sharedVar))==-1){
        perror("Error in shmdt\n");
        exit(1);
    }
}