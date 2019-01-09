#include <iostream>
#include <cstdio>
#include <set>
#include <vector>
#include <map>
#include <list>
#include <utility>

using namespace std;

void dfsUtil(vector <int> &visited,vector < list< pair<int,int> > > &adjacencyMatrix,vector < pair<int,int> > &stack,int i,int par,bool canBeGreedy,double &mal){
    visited[i] = 1;
    int j,k,l;
    list < pair<int,int> >::iterator ite;
    for(ite=adjacencyMatrix[i].begin();ite!=adjacencyMatrix[i].end();ite++){
        if(ite->second==par){
            bool isGreedy = false;
            cout<<par<<"->";
            for(j=0;j<stack.size();j++)
                cout<<stack[j].second<<"->";
            cout<<ite->second<<"\n";
            cout<<"(";
            int sum=0;
            for(j=0;j<stack.size();j++){
                cout<<stack[j].first<<",";
                sum+=stack[j].first;
            }
            cout<<ite->first<<")\n";
            sum+=ite->first;
            double temp = ((double)sum/(stack.size()+1));
            cout<<"Avg Latency:"<<temp<<"\n";
            canBeGreedy = canBeGreedy&(ite==adjacencyMatrix[i].begin());
            if(canBeGreedy){
                if(mal==-1){
                    mal = temp;
                }else if(temp<mal){
                    mal = temp;
                }
                cout<<"This is Greedy Cycle\n";
            }
        }
        else if(visited[ite->second]!=2){
            if(visited[ite->second]==1){
                //present in stack
                //ignore
            }else{
                stack.push_back({ite->first,ite->second});
                canBeGreedy = canBeGreedy&(ite==adjacencyMatrix[i].begin());
                dfsUtil(visited,adjacencyMatrix,stack,ite->second,par,canBeGreedy,mal);
                visited[ite->second] = 0;
                stack.pop_back();
            }
        }
    }

    visited[i] = 2;
}

void addNodesToMatrix(map <string,bool> &Visited,map <string,int> &Map,vector < list< pair<int,int> > > &adjacencyMatrix,string initialCollisionVector,string newNode,int edge,int parnode,int &counter){
    string currentVector = initialCollisionVector;
    int i,j;
    int tempCounter;

    string newVector = string((initialCollisionVector.length()-newNode.length()),'0')+newNode;
    for(i=currentVector.length()-1;i>=0;i--){
        if(currentVector[i]=='1'||newVector[i]=='1')
            currentVector[i]='1';
    }

    if(Visited[currentVector]){
        tempCounter = Map[currentVector];
        adjacencyMatrix[parnode].push_back({edge,tempCounter});
        return;
    }

    if(!Map[currentVector]){
        tempCounter = counter++;
        Map[currentVector] = tempCounter;
    }else{
        tempCounter = Map[currentVector];
        if(tempCounter==parnode){
            adjacencyMatrix[parnode].push_back({edge,tempCounter});
            return;        
        }
    }

    adjacencyMatrix[parnode].push_back({edge,tempCounter});
    
    for(i=initialCollisionVector.length()-1;i>=0;i--){
        if(currentVector[i]=='0'){
            string newVector = currentVector.substr(0,i);
            addNodesToMatrix(Visited,Map,adjacencyMatrix,initialCollisionVector,newVector,initialCollisionVector.length()-i,tempCounter,counter);
        }
    }
    bool parpresent = false;
    list < pair<int,int> >::iterator ite;
    for(ite = adjacencyMatrix[tempCounter].begin();ite!=adjacencyMatrix[tempCounter].end();ite++){
        if(ite->second==parnode){
            parpresent=true;
            break;
        }     
    }

    if(!parpresent){
        adjacencyMatrix[tempCounter].push_back({initialCollisionVector.length()+1,parnode});
    }

    Visited[currentVector] = true;
}

int main(int argc,char *argv[]){
    freopen(argv[1],"r",stdin);
    int stages=0;
    int totaltime = 0;
    int i,j;

    // stores forbidden latency
    set <int> forbidden;

    string s;
    while(getline(cin,s)){

        // stores cycle numbers with x for current stage
        vector <int> cyclePresent;
        for(i=0;i<s.length();i++){
            if(s[i]=='x'){
                for(j=0;j<cyclePresent.size();j++){
                    // (i/2)+1 because input contains ',' between two cycles
                    forbidden.insert((i/2)+1-cyclePresent[j]);
                }
                // cout<<i/2+1<<" ";
                // add current cycle to cyclePresent
                cyclePresent.push_back(i/2+1);
            }
        }
        // cout<<"\n";
    }
    set <int>::iterator itr = forbidden.end();
    itr--;
    int collisionvectorlength = *itr;
    string initialCollisionVector = string(collisionvectorlength,'0');
    for(itr=forbidden.begin();itr!=forbidden.end();itr++){
        initialCollisionVector[collisionvectorlength-(*itr)] = '1';
    }
    cout<<"Forbidden latency:";
    for(int tempi = collisionvectorlength-1;tempi>=0;tempi--){
        if(initialCollisionVector[tempi]=='1'){
            cout<<collisionvectorlength-tempi<<" ";
        }
    }
    cout<<"\nPermissible latency:";
    for(int tempi = collisionvectorlength-1;tempi>=0;tempi--){
        if(initialCollisionVector[tempi]=='0'){
            cout<<collisionvectorlength-tempi<<" ";
        }
    }
    cout<<"\nCollision Vector:"<<initialCollisionVector<<"\n";
    map <string,int> Map;
    map <string,bool> Visited;
    int nodeCtr=2;
    Map[initialCollisionVector] = 1;

    // {weight,nodenumber}
    vector < list< pair<int,int> > > adjacencyMatrix(101);
    for(i=initialCollisionVector.length()-1;i>=0;i--){
        if(initialCollisionVector[i]=='0'){
            string newVector = initialCollisionVector.substr(0,i);
            addNodesToMatrix(Visited,Map,adjacencyMatrix,initialCollisionVector,newVector,collisionvectorlength-i,1,nodeCtr);
        }
    }

    bool parpresent = false;
    list < pair<int,int> >::iterator ite;
    for(ite = adjacencyMatrix[1].begin();ite!=adjacencyMatrix[1].end();ite++){
        if(ite->second==1){
            parpresent=true;
            break;
        }     
    }

    if(!parpresent){
        adjacencyMatrix[1].push_back({initialCollisionVector.length()+1,1});
    }

    for(i=1;i<=100;i++){
        if(adjacencyMatrix[i].size()>0){
            cout<<i<<"->";
            for(ite = adjacencyMatrix[i].begin();ite!=adjacencyMatrix[i].end();ite++){
                cout<<ite->second<<"("<<ite->first<<")"<<",";
            }
            cout<<"\b \n";
        }
    }

    vector <int> visited(101,0);
    vector < pair<int,int> > stack; //{weight,nodenumber}
    double mal = -1;

    for(i=1;i<=100;i++){
        if(visited[i]==0){
            dfsUtil(visited,adjacencyMatrix,stack,i,i,true,mal);
        }
    }

    cout<<"MAL:"<<mal<<"\n";
    return 0;
}