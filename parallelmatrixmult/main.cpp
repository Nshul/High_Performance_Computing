#include <iostream>
#include <pthread.h>

using namespace std;

struct param{
    int row,col;
    int itr;
};

int M1[3][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};
int M2[4][2] = {{1,2},{3,4},{5,6},{7,8}};
int M3[3][2];

void *multMatrix(void* arg){
    param *Arg = (param*)arg;
    int r = Arg->row;
    int c = Arg->col;
    int k = Arg->itr;
    int temp=0;
    for(int i=0;i<k;i++){
        temp+=M1[r][i]*M2[i][c];
    }
    M3[r][c] = temp;

    pthread_exit(NULL);
}

int main(){
    int i,j;
    int n=3,m=2,k=4;
    pthread_t threads[n*m];
    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            param *temp = new param;
            temp->row = i;
            temp->col = j;
            temp->itr = k; 
            pthread_create(&threads[i*m+j],NULL,&multMatrix,(void*)temp);
        }
    }

    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            pthread_join(threads[i*m+j],NULL);
        }
    }

    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            cout<<M3[i][j]<<" ";
        }
        cout<<"\n";
    }
    return 0;
}