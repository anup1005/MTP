#include "graph.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <chrono>
#include <queue>
using namespace std;

void bfs(int source,int V,int E,int * index,int *edge_List,int *visited,int *h_active){
    queue<int> q;
    visited[source]=1;
    q.push(source);
   // cout<<"called for source"<<source<<endl;
    while(!q.empty()){
        int u=q.front();
        q.pop();
        int start=index[u];
        int end=index[u+1]-1;
        for(int i=start;i<=end;i++){
            int v=edge_List[i];
            if(v!=-1 && visited[v]==0 && h_active[v]==1){
                //cout<<source<<"   helps   "<<v<<endl;
                visited[v]=1;
                q.push(v);
            }
        }
    }
}

int main(int argc, char **argv){
    char *filepath=argv[1];
    graph g(filepath);
    std::chrono::steady_clock::time_point tic = std::chrono::steady_clock::now();
    g.parseGraph();
    std::chrono::steady_clock::time_point toc = std::chrono::steady_clock::now();
    std::cout << "Time required: to parse graphs is " << std::chrono::duration_cast<std::chrono::milliseconds>(toc - tic).count() << "[ms]" << "   "<<std::endl;

    int V=g.num_nodes();
    int E=g.num_edges();
    cout<<"no of nodes are "<<V<<endl;
    cout<<"no of edges are "<<E<<endl;
    int *cpu_index,*cpu_edgeList;
    cpu_index=(int*)malloc((V+1) * sizeof(int));
    cpu_edgeList=(int*)malloc((E) * sizeof(int));


    int *h_active,*hc_degree,*hn_degree;
    h_active=(int*)malloc(V * sizeof(int));
    hc_degree=(int*)malloc((V) * sizeof(int));
    hn_degree=(int*)malloc((V) * sizeof(int));
    int hflag=1;
    // cout<<"no of nodes are2 "<<V<<endl;
    // cout<<"no of edges are2 "<<E<<endl;


    for(int i=0;i<V;i++){
        cpu_index[i]=g.indexofNodes[i];
        h_active[i]=1;
        hc_degree[i]=0;
        hn_degree[i]=0;
    }
    cpu_index[V]=g.indexofNodes[V];
    // for(int i=0;i<=V;i++) cout<<cpu_index[i]<<"  ";
    // cout<<endl;

    for(int i=0;i<E;i++){
        int t=g.edgeList[i];
        cpu_edgeList[i]=t;
        hc_degree[t]=hc_degree[t]+1;
        hn_degree[t]=hn_degree[t]+1;
    }
    // for(int i=0;i<E;i++) cout<<cpu_edgeList[i]<<" ";
    // cout<<endl;
    // for(int i=0;i<V;i++){
    //     cout<<i<<" indegree   "<<hc_degree[i]<<endl;
    // }


    int k=1;
    std::chrono::steady_clock::time_point tic1 = std::chrono::steady_clock::now();
    
    while(hflag==1){
        hflag=0;

        
        for(int u=0;u<V;u++){
            if(h_active[u]==1 && hc_degree[u]<k){
                h_active[u]=0;
                for(int i=cpu_index[u];i<cpu_index[u+1];i++){
                    int v=cpu_edgeList[i];
                    cpu_edgeList[i]=-1;
                    hc_degree[v]-=1;
                    if(hc_degree[v]<k && h_active[v]==1){
                        hflag=1;
                    }

                }

            }
        }


    }
    std::chrono::steady_clock::time_point toc1 = std::chrono::steady_clock::now();
    std::cout << "Time required: KCD  is " << std::chrono::duration_cast<std::chrono::milliseconds>(toc1 - tic1).count() << "[ms]" << "   "<<std::endl;
    //cout<<"out of the main loop\n";
    int count=0;
    int components=0;
    int *visited=(int *)calloc(V,sizeof(int));
    for(int i=0;i<V;i++){
        if(h_active[i]){
            //cout<<i<<"survived\n";
            count++;
        }
        if(!visited[i]){
            components++;
            bfs(i,V,E,cpu_index,cpu_edgeList,visited,h_active);
        }
    }
    cout<<"No of active vertices remaining are "<<count<<endl;
    cout<<"No of dead vertices remaining are "<<V-count<<endl;
    cout<<"Total components  remaining are "<<components<<endl;
    cout<<"Total components  excluding dead vertices are "<<components-(V-count)<<endl;

}