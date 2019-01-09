#include <stdio.h>
#include <omp.h>

int main(){
    int *arr = malloc(36*sizeof(int));
    
    #pragma omp parallel for num_threads(12)
    for(int i=0;i<36;i++){
        arr[i] = rand()%100;
    }

    for(int i=0;i<36;i++){
        printf("%d ",arr[i]);
    }
    printf("\n");

    int Max=-1;
    #pragma omp parallel for reduction(max: Max)
    for(int j=0;j<36;j++){
        if(omp_get_thread_num()==0){
            printf("Total:%d",omp_get_num_threads());
        }
        if(arr[j]>Max)
            Max=arr[j];
    }

    printf("Max:%d",Max);
}