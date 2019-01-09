#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

int main(int argc,char **argv){
    if(argc<3){
        printf("Less arg\n");
        return 0;
    }
    int n = atoi(argv[1]);
    int np = atoi(argv[2]);
    omp_set_num_threads(np);
    int sizeblock = ceil(((double)n/np));

    int *arr = malloc(sizeof(int)*n);
    int *arr2 = malloc(sizeof(int)*np);
    arr2[0]=0;
    srand(time(NULL));
    for(int i=0;i<n;i++){
        arr[i] = rand()%10;
    }

    printf("Input array:");
    for(int i=0;i<n;i++){
        printf("%d ",arr[i]);
    }
    printf("\n");

    clock_t t1 = clock();
    #pragma omp parallel for shared(arr)
    for(int i=0;i<np;i++){
        int pid = omp_get_thread_num();
        int j;
        printf("For process id:%d\n",pid);
        for(j=pid*sizeblock+1;j<n&&j<(pid+1)*sizeblock;j++){
            arr[j]+=arr[j-1];
        }
        for(j=pid*sizeblock;j<n&&j<(pid+1)*sizeblock;j++){
            printf("%d ",arr[j]);
        }
        if(pid!=(omp_get_num_threads()-1))
            arr2[pid+1]=arr[j-1];
        printf("\n");
    }

    for(int i=1;i<np;i++){
        arr2[i]+=arr2[i-1];
    }
    for(int i=0;i<np;i++){
        printf("%d ",arr2[i]);
    }
    printf("\n");

    #pragma omp parallel for
    for(int i=0;i<np;i++){
        int pid = omp_get_thread_num();
        int adder = arr2[pid];
        int j;
        for(j=pid*sizeblock;j<n&&j<(pid+1)*sizeblock;j++){
            arr[j]+=adder;
        }
    }
    clock_t t2 = clock()-t1;
    double timer = (double)t2/CLOCKS_PER_SEC;

    for(int i=0;i<n;i++){
        printf("%d ",arr[i]);
    }
    printf("\nTime %f\n",timer);
}