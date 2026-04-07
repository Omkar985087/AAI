#include <iostream>
#include <bits/stdc++.h>

using namespace std;

vector<int> toposort(int n,vector<vector<int>>& adj)
{
    vector<int> indegree(6,0);

    for(int i=0;i<n;i++)
    {
        for(int j:adj[i])
        {
            indegree[j]++;
        }
    }

    queue<int> q;

    for(int i=0;i<n;i++)
    {
        if(indegree[i]==0)
        {
            q.push(i);
        }
    }

    vector<int> topo;

    while(q.size()>0)
    {
        int node=q.front();
        q.pop();

        topo.push_back(node);

        for(int i:adj[node])
        {
            indegree[i]--;

            if(indegree[i]==0)
            {
                q.push(i);
            }
        }
    }

    return topo;
}
int main()
{
    int n;
    cin>>n;

    vector<vector<int>> adj(6);

    adj[5].push_back(2);
    adj[5].push_back(0);
    adj[4].push_back(0);
    adj[4].push_back(1);
    adj[2].push_back(3);
    adj[3].push_back(1);

    vector<int> result=toposort(n,adj);

    for(int i:result)
        cout<<i<<" ";
}