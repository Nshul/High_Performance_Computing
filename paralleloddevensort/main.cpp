#include <iostream>
#include <pthread.h>
using namespace std;

int arr[] = {4,5,6,7,2,23,4,1};
int n = 8;

void *addele(void *Arg){
    int *I = (int* )Arg;
    int i=*I;
    if(i+1<n){
        if(arr[i]>arr[i+1]){
            int temp = arr[i+1];
            arr[i+1] = arr[i];
            arr[i] = temp; 
        }
    }
    pthread_exit(NULL);
}

int main(){
    int i,j,turn;
    pthread_t threads[4];
    for(i=1;i<=n;i++){
        turn = i&1;
        for(j=0;j<n/2;j++){
            int *tr = new int;
            *tr = 2*j+1-turn;
            pthread_create(&threads[j],NULL,&addele,(void*)tr);
        }
        for(j=0;j<n/2;j++){
            pthread_join(threads[j],NULL);   
        }
    }
    for(i=0;i<8;i++){
        cout<<arr[i]<<" ";
    }
    cout<<"\n";
    return 0;
}