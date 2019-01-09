#include <stdio.h>
#include <omp.h>
#include <time.h>

int main(){
    int arr[] = {1,4,5,7,2,3,54,9};
    int sum =0;
    int n = sizeof(arr)/sizeof(int);

    clock_t t1 = clock();
    #pragma omp parallel for reduction(+:sum)
    for(int i=0;i<n;i++){
        if(omp_get_thread_num()==0){
            printf("total:%d\n",omp_get_num_threads());
        }
        sum+=arr[i];
        // printf("Calc:%d\n",sum);
    }
    clock_t final = (double)(clock()-t1)/CLOCKS_PER_SEC;
    printf("Time:%f\n",final);
    printf("Sum:%d\n",sum);
}