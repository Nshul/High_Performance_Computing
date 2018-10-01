#include <iostream>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <cstdlib>
#include <cstdio>

using namespace std;

struct memoryUnit{
    int status;
    char data[100];
};

int main(){

    key_t key = ftok("./sharedFile",65);

    int shm_id = shmget(key,1024,0666);

    if(shm_id<0){
        perror("shmget error:");
        exit(1);
    }

    memoryUnit *shm_ptr = (memoryUnit*)shmat(shm_id,NULL,0);

    if(shm_ptr==(memoryUnit*)(-1)){
        cout<<"shmat error\n";
        exit(1);
    }

    while(shm_ptr->status!=0); //while NOT FILLED
    cout<<"Reading Data from File:"<<shm_ptr->data<<"\n";

    shm_ptr->status=1; //READ DATA

    if(shmdt(shm_ptr)==-1){
        cout<<"shmdt error\n";
        exit(1);
    }
}