#include "myheader.h"
using namespace std;


/* Change function takes a graph as a input(ofcourse the CSR of the graph) ,V as the number of vertices in the graph ,
   E as the number of edges in the graph, and an edge represents by its two end nodes as "s" and "e" and the value which 
   has to be assigned to that corresponding edge
*/
int change(int V,int E,int s,int e,int *index,int *list,int *length,int value){
    int start=index[s];
    int end=E-1;
    if(s<V-1) end=index[s+1]-1;

    int ans=-1;
    //cout<<"called for changing   "<<s<<"   to   "<<e<<"   to   "<<value<<endl;
    int i;
    for(i=start;i<=end;i++){
        if(list[i]==e){
           // cout<<"\ngotch you at  "<<i <<"\n";
            ans=length[i];
            length[i]=value;
            break;
        }
    }
    //cout<<"  changed value is "<<length[i]<<endl;
    return ans;
}

/* 
Preflow function  takes the original network as input graph, and also the residual graph as the input (ofcourse both the graphs are represented in CSR)
it initialiss the height of every node,pushes max possible value from the source to the neighbours of the source ,initialises the excess_flow of 
each node and also initialsies the excess_total variable and also changes the residual network according to the preflow.  
*/
void preflow(int V,int E ,int Er,int source, int sink,int *cpu_index,int *cpu_edgeList,int *cpu_edgeLength,
            int *cpu_Rindex,int *cpu_RedgeList,int *cpu_RedgeLength,int *cpu_height, int *cpu_excess_flow,int *Excess_total ,int *khush)
{
    // initialising height values and excess flow, Excess_total values
    for(int i = 0; i < V; i++)
    {
        cpu_height[i] = 0; 
        cpu_excess_flow[i] = 0;
    }
    
    cpu_height[source] = V;
    *Excess_total = 0;


    int start=cpu_index[source];
    int end=E-1;
    if(source<V-1) end=cpu_index[source+1]-1;
    //cout<<"BEFORE   ";
   // for(int i=0;i<2*E;i++) cout<<cpu_RedgeLength[i]<<"  ";
   // cout<<endl;
    for(int i=start;i<=end;i++){
        int s=source;
        int e=cpu_edgeList[i];
        int w=cpu_edgeLength[i];
        if(khush[e]==0) continue;
        //cout<<"pushing  flow "<<w<<"   from    "<<s<<"    "<<e<<endl;
        change(V,Er,source,cpu_edgeList[i],cpu_Rindex,cpu_RedgeList,cpu_RedgeLength,0);
        change(V,Er,cpu_edgeList[i],source,cpu_Rindex,cpu_RedgeList,cpu_RedgeLength,w);
        cpu_excess_flow[cpu_edgeList[i]] =w ;
        *Excess_total += w;
    }

   // cout<<"AFTER   ";
   // for(int i=0;i<2*E;i++) cout<<cpu_RedgeLength[i]<<"  ";
   // cout<<endl;
    // cout<<"excess for 1   "<<cpu_excess_flow[0]<<endl;
    // cout<<"excess for 3   "<<cpu_excess_flow[3]<<endl;

    // // pushing flow in all edges going out from the source node
    // for(int i = 0; i < V; i++)
    // {
    //     // for all (source,i) belonging to E :
    //     if(cpu_adjmtx[IDX(source,i)] > 0)
    //     {
    //         // pushing out of source node
    //         cpu_rflowmtx[IDX(source,i)] = 0;
            
    //         /* updating the residual flow value on the back edge
    //          * u_f(x,s) = u_xs + u_sx
    //          * The capacity of the back edge is also added to avoid any push operation back to the source 
    //          * This avoids creating a race condition, where flow keeps travelling to and from the source
    //          */
    //         cpu_rflowmtx[IDX(i,source)] += cpu_adjmtx[IDX(source,i)];
            
    //         // updating the excess flow value of the node flow is pushed to, from the source
    //         cpu_excess_flow[i] = cpu_adjmtx[IDX(source,i)];

    //         // update Excess_total value with the new excess flow value of the node flow is pushed to
    //         *Excess_total += cpu_excess_flow[i];
    //     } 
    // }
    cout<<"Preflow completed"<<endl;

}