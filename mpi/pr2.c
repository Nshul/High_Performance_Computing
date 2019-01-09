#include <stdio.h>
#include <mpi.h>

int main(int argc,char **argv){
    MPI_Init(&argc,&argv);

    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    int data_hold=13;
    if(rank==0){
        data_hold = 89;
        MPI_Send(&data_hold,1,MPI_INT,1,0,MPI_COMM_WORLD);
        printf("Data Sent\n");
    }else if(rank==1){
        printf("Currently datahold=%d\n",data_hold);
        MPI_Recv(&data_hold,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        printf("Now:%d\n",data_hold);
    }
    MPI_Finalize();
    return 0;
}