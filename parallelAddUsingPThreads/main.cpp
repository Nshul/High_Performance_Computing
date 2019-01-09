#include <iostream> 
#include <pthread.h>   
using namespace std; 
struct argument{
    int *arr1;
    int *arr2;
    int id;
    int turn;
};

void *findSum(void *argu) 
{ 
    argument *arg = (argument *)argu;
    int id = arg->id;
    if(arg->turn==0){
        arg->arr2[id]=0;
        for(int i=(arg->id)*2;i<(arg->id+1)*2;i++){
            arg->arr2[id]+=arg->arr1[i];
        }
    }else{
        arg->arr1[id]=0;
        for(int i=(arg->id)*2;i<(arg->id+1)*2;i++){
            arg->arr1[id]+=arg->arr2[i];
        }
    }

    pthread_exit(NULL);
} 
  
int main() 
{ 
    pthread_t threads[8]; 
    int arr1[] = {1, 5, 7, 10, 12, 14, 15, 18, 20, 22, 25, 27, 30, 64, 110, 220 }; 
    int arr2[16] = {0};
    int temp = 8;
    int turn=0;
    while(temp){
        for (int i = 0; i < temp; i++){ 
            argument *arg = new argument;
            arg->arr1 = arr1;
            arg->arr2 = arr2;
            arg->id=i;
            arg->turn=turn;
            pthread_create(&threads[i], NULL, &findSum,(void*)arg); 
        }
  
        for (int i = 0; i < temp; i++) 
            pthread_join(threads[i], NULL);

        temp/=2; 
        turn=1-turn;
    }
  
    cout << "sum is " << arr1[0] << endl; 
  
    return 0; 
} 