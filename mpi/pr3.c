#include <stdio.h>
#include <mpi.h>

int main(){
    MPI_Init(NULL,NULL);
    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    int pingpongcount=0;
    int partnerrank = 1-rank;

    while(pingpongcount<5){
        if(pingpongcount%2==rank){
            printf("Currently in %d and ppc:%d\n",rank,pingpongcount);
            pingpongcount++;
            MPI_Send(&pingpongcount,1,MPI_INT,partnerrank,0,MPI_COMM_WORLD);
            printf("Sent from %d and ppc:%d\n",rank,pingpongcount);
        }else{
            printf("Currently in %d and ppc:%d\n",rank,pingpongcount);
            MPI_Recv(&pingpongcount,1,MPI_INT,partnerrank,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            printf("Recv in %d and ppc:%d\n",rank,pingpongcount);
        }
    }

    MPI_Finalize();
}