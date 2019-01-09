#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

int main(int argc,char **argv){
    if(argc<2){
        printf("Less arg\n");
        return 0;
    }
    int n = atoi(argv[1]);
    srand(time(NULL));
    

    MPI_Init(&argc,&argv);
    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(rank==0){
        //main process
        int *arr = malloc(sizeof(int)*n);
        for(int i=0;i<n;i++){
            arr[i] = rand()%10;
        }
        printf("Input:");
        for(int i=0;i<n;i++){
            printf("%d ",arr[i]);
        }
        printf("\n");

        //Send size
        int SiZe = ceil((double)n/size);
        for(int i=1;i<size-1;i++){
            MPI_Send(&SiZe,1,MPI_INT,i,0,MPI_COMM_WORLD);
        }
        int newSize = n- ((size-1)*SiZe);
        if(size>1)
        MPI_Send(&newSize,1,MPI_INT,size-1,0,MPI_COMM_WORLD);

        //Send begin
        int j=SiZe;
        for(int i=1;i<size-1;i++){
            MPI_Send(&arr[j],SiZe,MPI_INT,i,1,MPI_COMM_WORLD);
            j+=SiZe;
        }
        if(size>1)
        MPI_Send(&arr[j],newSize,MPI_INT,size-1,1,MPI_COMM_WORLD);

        //Self compute
        for(j=1;j<n&&j<SiZe;j++){
            arr[j]+=arr[j-1];
        }

        int *arr2 = malloc(size*sizeof(int));
        arr2[0] = 0;
        arr2[1] = arr[j-1];
        MPI_Status status;

        for(int i=1;i<size;i++){
            int recdata;
            MPI_Recv(&recdata,1,MPI_INT,MPI_ANY_SOURCE,2,MPI_COMM_WORLD,&status);
            if(status.MPI_SOURCE!=size-1)
                arr2[status.MPI_SOURCE+1]=recdata;
        }
        // printf("hellalujah\n");
        for(int i=1;i<size;i++){
            arr2[i]+=arr2[i-1];
        }

        for(int i=1;i<size;i++){
            MPI_Send(&arr2[i],1,MPI_INT,i,3,MPI_COMM_WORLD);
        }

        int *temparr = malloc(sizeof(int)*SiZe);
        for(int i=1;i<size;i++){
            MPI_Recv(&temparr[0],SiZe,MPI_INT,MPI_ANY_SOURCE,4,MPI_COMM_WORLD,&status);
            for(int k=0;k<n&&k<(status.MPI_SOURCE+1)*SiZe;k++){
                arr[status.MPI_SOURCE*SiZe+k]=temparr[k];
            }
        }

        printf("Finally:");
        for(int i=0;i<n;i++){
            printf("%d ",arr[i]);
        }
    }else{
        int SiZe,*Beg;
        MPI_Status s;
        MPI_Recv(&SiZe,1,MPI_INT,0,0,MPI_COMM_WORLD,&s);
        Beg = malloc(sizeof(int)*SiZe);
        MPI_Recv(&Beg[0],SiZe,MPI_INT,0,1,MPI_COMM_WORLD,&s);
        int i;
        for(i=1;i<SiZe;i++){
            printf("Be:%d\t",Beg[i]);
            Beg[i]+=Beg[i-1];
        }

        MPI_Send(&Beg[i-1],1,MPI_INT,0,2,MPI_COMM_WORLD);
        int adder;
        MPI_Recv(&adder,1,MPI_INT,0,3,MPI_COMM_WORLD,&s);

        for(i=0;i<SiZe;i++){
            Beg[i]+=adder;
        }

        MPI_Send(&Beg[0],SiZe,MPI_INT,0,4,MPI_COMM_WORLD);
    }

    MPI_Finalize();
}