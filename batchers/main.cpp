#include <bits/stdc++.h>
#include <stdlib.h>
using namespace std;

void printAndWait(int arr[], int low, int N)
{
    cout<<"\n";
    for(int i=low;i<low + N;i++)
    {
        cout<<arr[i]<<" ";
    }
    int x = cin.get();
}
void unshuffle(int arr[], int N, int low)
{
    cout<<"unshuffle: ";
    printAndWait(arr,low,N);
    int temp[N], j=0;
    for(int i=0;i<N;i++)
    temp[i] = arr[low+i];

    for(int i=low;i<low+(N/2);i++)
    {
        arr[i] = temp[j];
        j+=2;
    }
    j=1;
    for(int i=low+(N/2);i<low+N;i++)
    {
        arr[i] = temp[j];
        j+=2;
    }
    return ;
}

void shuffle(int arr[], int N, int low)
{
    cout<<"shuffle: ";
    printAndWait(arr,low,N);
    int temp[N];
    for(int i=0;i<N;i++)
        temp[i] = arr[low+i];
    int l = 0, r = (N/2);
    for(int i=low;i<low+N;i+=2)
    {
        arr[i] = temp[l];
        arr[i+1] = temp[r];
        l++;
        r++;
    }
    
    return ;
}
void compExchg(int arr[], int N, int low)
{
    cout<<"compExchg: ";
    printAndWait(arr,low,N);
    for(int i=low+1;i<low+N;i+=2)
    {
        if(arr[i]>arr[i+1])
            swap(arr[i],arr[i+1]);    
    }
    return ;
}
void batchers_merge(int arr[], int N, int low)
{
    cout<<"batchers_merge: ";
    printAndWait(arr,low,N);
    if(N<=1)
        return ;
    if(N==2)
    {
        if(arr[low]>arr[low+1])
            swap(arr[low],arr[low+1]);
        return ;
    }
    unshuffle(arr,N,low);

    batchers_merge(arr,N/2,low);
    batchers_merge(arr,N/2,low+(N/2));

    shuffle(arr,N,low);

    compExchg(arr,N,low);
    
    return ;
}

void batchers(int arr[], int N, int low)
{
    cout<<"batchers: ";
    printAndWait(arr,low,N);
    if(N<=1)
        return ;
    else if(N==2)
    {
        if(arr[low]>arr[low+1])
            swap(arr[low], arr[low+1]);
        return ;
    }
    batchers(arr,N/2,low);
    batchers(arr,N/2,low+(N/2));

    batchers_merge(arr,N,low);
}
int main()
{
    int arr[8] = {12,10,40,33,22,45,54,1};
    int N=8;

    batchers(arr,N,0);
    for(int i=0;i<N;i++)
        cout<<arr[i]<<" ";
    cout<<"\nSort Ends\n";
}