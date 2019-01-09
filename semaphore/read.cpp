#include <iostream>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main(int argc,char **argv){
    
    cout<<"This process:"<<getpid()<<"\n";
    key_t key =20;

    int sem_id;
    if((sem_id = semget(key,1,0666))==-1){
        cout<<"Error in semget\n";
        exit(1);
    }

    int val;
    val = semctl(sem_id,0,GETVAL);

    cout<<"Value read:"<<val<<"\n";
    cout << "Last access process id:" << semctl(sem_id, 0, GETPID) << "\n";

    return 0;
}