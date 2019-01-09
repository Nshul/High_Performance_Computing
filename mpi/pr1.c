#include <stdio.h>
#include <mpi.h>

int main(int argc,char **argv){
    MPI_Init(&argc,&argv);
    int processor_no,tot;
    MPI_Comm_rank(MPI_COMM_WORLD,&processor_no);
    MPI_Comm_size(MPI_COMM_WORLD,&tot);
    printf("Hello I'm %d of %d\n",processor_no,tot);
    MPI_Finalize();
    return 0;
}