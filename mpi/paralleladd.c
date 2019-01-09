#include <stdio.h>
#include <mpi.h>

int main(){
    int arr1[] = {1,5,8,7,1,6,5,9,7,4,1,45};
    int n = sizeof(arr1)/sizeof(int);
    MPI_Init(NULL,NULL);
    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    int temp=0;
    for(int i=rank;i<n;i+=size){
        temp+=arr1[i];
    }

    if(rank==0){
        int temp1;
        MPI_Status status;
        for(int i=1;i<size;i++){
            MPI_Recv(&temp1,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
            printf("Received from %d sum of%d\n",status.MPI_SOURCE,temp1);
            temp+=temp1;
        }
        printf("Total:%d\n",temp);
    }else{
        //child
        MPI_Send(&temp,1,MPI_INT,0,0,MPI_COMM_WORLD);
    }
    MPI_Finalize();
}
