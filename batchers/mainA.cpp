#include <iostream>
#include <pthread.h>

using namespace std;

struct mergeArg{
    int l,r,sizeOfMerge;
};

// int arr[] = {39,8,15,12,10,6,4,2};
int arr[] = {8,7,6,5,4,3,2,1};

void *Swap(void *arg){
    int l = *((int*)arg);
    int temp = arr[l];
    arr[l] = arr[l+1];
    arr[l+1] = temp;
    pthread_exit(NULL);
}

void *compAndSwap(void *arg){
    int l = *((int*)arg);
    if(arr[l+1]<arr[l]){
        int temp = arr[l];
        arr[l] = arr[l+1];
        arr[l+1] = temp;
    }
    pthread_exit(NULL);
}

void shuffle(int l,int r){
    int n = r-l+1;
    int t = n/2;
    int i,j;
    pthread_t threads[t];
    for(i=1;i<t;i++){
        for(j=0;j<i;j++){
            int *L = new int; *L=l+t-i+2*j;
            pthread_create(&threads[j], NULL, &Swap,(void*)L);
        }
        for(j=0;j<i;j++){
            pthread_join(threads[j],NULL);
        }
    }
}

void unshuffle(int l,int r){
    int n = r-l+1;
    int t = n/2;
    int i,j;
    pthread_t threads[t];
    for(i=t-1;i>=1;i--){
        for(j=0;j<i;j++){
            int *L = new int; *L=l+t-i+2*j;
            pthread_create(&threads[j], NULL, &Swap,(void*)L);
        }
        for(j=0;j<i;j++){
            pthread_join(threads[j],NULL);
        }
    }
}

void *batcher_merge(void *arg){
    mergeArg *T = (mergeArg*)arg;
    int l = T->l;
    int r = T->r;
    int sizeOfMerge = T->sizeOfMerge;

    if(sizeOfMerge==2){
        if(arr[l]>arr[l+1]){
            int temp = arr[l+1];
            arr[l+1] = arr[l];
            arr[l] = temp;
        }
    }else{
        unshuffle(l,r);

        int newSize = sizeOfMerge/2;
        pthread_t threads[2];
        
        // Merge first half
        mergeArg* temp = new mergeArg;
        temp->l=l;
        temp->r=l+newSize-1;
        temp->sizeOfMerge=newSize;
        pthread_create(&threads[0],NULL,&batcher_merge,(void*)temp);

        // Merge second half
        mergeArg* temp1 = new mergeArg;
        temp1->l=l+newSize;
        temp1->r=r;
        temp1->sizeOfMerge=newSize;
        pthread_create(&threads[1],NULL,&batcher_merge,(void*)temp1);
        
        // Wait  for merge to complete
        pthread_join(threads[0],NULL);
        pthread_join(threads[1],NULL);

        // Shuffle the result
        shuffle(l,r);

        // Compare and Swap even index
        int i,j;
        pthread_t thr[sizeOfMerge/2];
        for(j=0;2*j+1<r;j++){
            int *newj = new int; *newj = l+2*j+1;
            pthread_create(&thr[j],NULL,&compAndSwap,(void*)newj);
        }
        for(j=0;2*j+1<r;j++){
            pthread_join(thr[j],NULL);
        }
    }
    pthread_exit(NULL);
}

// void batcher_sort(int n){
//     int i,j;
//     for(i=2;i<=n;i*=2){
        
//         pthread_t threads[n/i];
//         for(j=0;j<n/i;j++){
//             mergeArg *temp = new mergeArg;
//             temp->l = i*j;
//             temp->r = temp->l+i-1;
//             temp->sizeOfMerge = i;
            
//             pthread_create(&threads[j],NULL,&batcher_merge,(void*)temp);
            
//         }
//         for(j=0;j<n/i;j++){
//             pthread_join(threads[j],NULL);
//         }
//     }
// }

struct batcherarg{
    int l,r;
};

void *batcher_sort(void *arg){
    int l = ((batcherarg*)arg)->l;
    int r = ((batcherarg*)arg)->r;
    
    if(r-l==1){
        pthread_t thr[1];
        mergeArg *temp = new mergeArg;
        temp -> l = l;
        temp -> r = r;
        temp-> sizeOfMerge = 2;
        pthread_create(thr,NULL,&batcher_merge,(void*)temp);
        pthread_join(thr[0],NULL);
    }else{
        int mid = l+(r-l)/2;
        batcherarg *new1 = new batcherarg;
        new1 -> l = l;
        new1 -> r = mid;
        batcherarg *new2 = new batcherarg;
        new2 -> l = mid+1;
        new2 -> r = r;
        pthread_t threads[2];
        pthread_create(&threads[0],NULL,&batcher_sort,(void*)new1);
        pthread_create(&threads[1],NULL,&batcher_sort,(void*)new2);
        pthread_join(threads[0],NULL);
        pthread_join(threads[1],NULL);

        pthread_t t1[1];
        mergeArg *temp =  new mergeArg;
        temp -> l = l;
        temp -> r = r;
        temp -> sizeOfMerge = r-l+1;
        pthread_create(&t1[0],NULL,&batcher_merge,(void*)temp);
        pthread_join(t1[0],NULL);  
    }
    pthread_exit(NULL);
}

int main(){
    int sizearr = sizeof(arr)/sizeof(int);
    // batcher_sort(sizearr);

    batcherarg *temp = new batcherarg;
    temp -> l = 0;
    temp -> r = sizearr-1;
    pthread_t thread[1];
    pthread_create(&thread[0],NULL,&batcher_sort,(void*)temp);
    pthread_join(thread[0],NULL);

    for(int i=0;i<sizearr;i++){
        cout<<arr[i]<<" ";
    }
}