#include <iostream>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

using namespace std;

struct memoryUnit{
    int status;
    char data[100];
};

int main(){

    // http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/shm/example-2.html
    //https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_shared_memory.htm

    /**** ftok ****/
    //https://stackoverflow.com/questions/7358400/whats-is-purpose-ftok-in-message-queues
    key_t key = ftok("./sharedFile",65);

    /**** shmget ****/
    //http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/shm/shmget.html
    
    //0666 is to grant read and write access
    // 0 -> octal so it is actually 110110110 in binary
    // without 0, 666 -> 1010011010
    
    //IPC_CREAT is to grant the read and write access to server
    //(Due to UNIX's tradition) it is CREAT and not CREATE
    int shm_id = shmget(key,1024,IPC_CREAT|0666);
    if(shm_id<0){
        //Cannot get requested shared memory
        cout<<"shmget error\n";
        exit(1);
    }
    // We now have the requested shared memory

    /**** shmat ****/
    // Used to attach shared memory to address space
    // http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/shm/shmat.html
    // we can use SHM_RDONLY instead of 0 as flag then
    // this shared memory would be just read only but with 0
    // it is both read and write
    memoryUnit *shm_ptr = (memoryUnit*)shmat(shm_id,NULL,0);

    if(shm_ptr==(memoryUnit*)(-1)){
        //Cannot attach shared memory to address space
        cout<<"shmat error\n";
        exit(1);
    }

    cout<<"Server has attached the shared memory\n";
    shm_ptr->status = -1;   //NOT FILLED
    bool flag=true;
    char op;
    while(flag){
        string s;
        cout<<"\nEnter:"; 
        getline(cin,s);
        strcpy(shm_ptr->data,s.c_str());
        shm_ptr->status = 0;    //FILLED

        cout<<"Written\n";

        while(shm_ptr->status!=1)
            sleep(1);

        cout<<"Read by client.\nDo you want to Enter more?";cin>>op;
        if(op=='y'||op=='Y'){
            flag=true;
            shm_ptr->status=-1; //NOT FILLED
        }else{
            flag=false;
        }
        cin.ignore();
    }

    // http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/shm/shmdt.html
    int shm_detach = shmdt(shm_ptr);
    if(shm_detach==-1){
        cout<<"shmdt error\n";
        exit(1);
    }

    if(shmctl(shm_id, IPC_RMID, NULL)==-1){
        perror("shmctl error\n");
        exit(1);
    }
}