#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>
#include <cassert>
#define E(x) {cout << "E" << x << endl;}
using namespace std;


class Comp {
public:
    bool operator() (int a, int b){
        return a > b;
    }
};

vector<int> topoSort(int nodes_count,map<int,vector<int>>& graph){
    vector<int> in_deg(nodes_count+1,0);
    for(auto neighbours : graph) {
        for(auto neighbour : neighbours.second) in_deg[neighbour]++;
    }
    priority_queue<int,vector<int>,Comp> pq; //priority queue is used to break the ties between current in degree 0 nodes
    for(int x = 1 ; x <= nodes_count ; x++) if(in_deg[x] == 0) pq.push(x);
    if(pq.empty()) {
        return {};
    }
    vector<int> topo_sorting;
    while(!pq.empty()) {
        int curr = pq.top();
        pq.pop();
        topo_sorting.push_back(curr);
        for(auto elem : graph[curr]) {
            in_deg[elem]--;
            if(in_deg[elem] == 0)pq.push(elem);
        }
    }
    if(topo_sorting.size() != nodes_count) {
        return {};
    }
    return topo_sorting;
}

void postorder(int node, vector<int>& order, vector<bool>& vis, map<int,vector<int>> &graph) {
    vis[node] = true;
    for(auto elem : graph[node]) {
        if(vis[elem])continue;
        postorder(elem,order,vis,graph);
    }
    order.push_back(node);
}

void SCC_Helper_postorder(int node, vector<int>& scc_group, map<int,vector<int>>& transpose, int grp_no,vector<int>& scc) {
    scc_group[node] = grp_no;
    scc.push_back(node);
    for(auto elem : transpose[node]) {
        if(scc_group[elem] != 0)continue;
        SCC_Helper_postorder(elem,scc_group,transpose,grp_no,scc);
    }
}

bool cycle_postorder(map<int,vector<int>> &map, vector<int>& color, int node) {
    color[node] = 1;
    bool cycleCheck = false;
    for(auto elem : map[node]) { //elem is each neightbour of node
        if(color[elem] == 0)cycleCheck |= cycle_postorder(map,color,elem);
        else if(color[elem] == 1)return true;
    }
    color[node] = 2;
    return cycleCheck;
}

bool isCycleCheck(map<int,vector<int>>& graph, int nodes_count) {
    vector<int> color(nodes_count + 1,0); //0 = unvisited, 1 = processing, 2 = visited
    for(int x = 1 ; x <= nodes_count ; x++) {
        if(color[x] == 0) {
            if(cycle_postorder(graph,color,x)) return true;
        }
    }
    return false;
}

class GraphAlgorithm{
public:
    virtual void Query() = 0; //not taking parameters as per info doc
};

class isCycle : public GraphAlgorithm {
public:
    isCycle(map<int,vector<int>> &graph, int nodes_count) {
        this->graph = graph;
        this->nodes_count = nodes_count;
    }
    void Query() {
        if(isCycleCheck(graph,nodes_count))cout << "YES";
        else cout << "NO";
        cout << endl; 
    }

protected:
    map<int,vector<int>> graph;
    int nodes_count;
};

class indepComponent : public GraphAlgorithm{
public:
    indepComponent(map<int,vector<int>> &graph, int nodes_count) {
        this->graph = graph;
        this->nodes_count = nodes_count;
    }
    void Query() {
        vector<int> order;
        vector<bool> vis(nodes_count+1,false);
        for(int x = 1 ; x <= nodes_count ; x++) {
            if(!vis[x])postorder(x,order,vis,graph);
        }
        // for(auto elem : order ) cout << elem << " ";
        // cout << endl;
        reverse(order.begin(),order.end());
        map<int,vector<int>> transpose;
        for(auto elem : graph) {
            for(auto el : elem.second) transpose[el].push_back(elem.first);
        }
        vector<vector<int>> scc;
        vector<int> scc_group (nodes_count+1,0);
        int grp_no = 1;
        int highest = 0;
        for(auto x : order) {
            if(scc_group[x] == 0) {
                vector<int> scc1;
                SCC_Helper_postorder(x,scc_group,transpose,grp_no,scc1);
                scc.push_back(scc1);
                grp_no++;
                highest = (highest > scc1.size() ? highest : scc1.size());
            }
        }
        cout << scc.size() << " " << highest << endl;
        // for(auto elem : scc) {
        //     for(auto el : elem) cout << el << " ";
        //     cout << endl;
        // }
    }
protected:
    map<int,vector<int>> graph;
    int nodes_count;
};

class validOrder : public GraphAlgorithm {
public: 
    validOrder(map<int,vector<int>> &graph,int nodes_count) {
        this->graph = graph;
        this->nodes_count = nodes_count;
    }
    void Query() {
        vector<int> topo_sorting = topoSort(nodes_count,graph);
        if(topo_sorting.size() != nodes_count) {
            cout << "NO" << endl;
            return;
        }
        for(auto elem : topo_sorting)cout << elem << " ";
        cout << endl;
    }
protected:
    map<int,vector<int>> graph;
    int nodes_count;
};

class maxHype : public GraphAlgorithm {
public:    
    maxHype(map<int,vector<int>> &graph, vector<int> &hype_vector) {
        this->graph = graph;
        this->hype_vector = hype_vector;
    }
    void Query() {
        vector<int> order;
        vector<bool> vis(hype_vector.size()+1,false);
        for(int x = 1 ; x <= hype_vector.size() ; x++) {
            if(!vis[x]){
                vis[x] = true;
                postorder(x,order,vis,graph);
            }
        }
        // assert(order.size() == hype_vector.size());
        // for(auto elem : order ) cout << elem << " ";
        // cout << endl;
        reverse(order.begin(),order.end());
        map<int,vector<int>> transpose;
        for(auto elem : graph) {
            for(auto el : elem.second) transpose[el].push_back(elem.first);
        }
        vector<vector<int>> scc;
        vector<int> scc_group (hype_vector.size()+1,0);
        int grp_no = 1;
        for(auto x : order) {
            if(scc_group[x] == 0) {
                vector<int> scc1;
                SCC_Helper_postorder(x,scc_group,transpose,grp_no,scc1);
                grp_no++;
                scc.push_back(scc1);
            }
        }
        map<int,vector<int>> compressed_graph;
        vector<int> new_hype (scc.size()+1,0);
        for(int x = 0 ; x < scc.size() ; x++) {
            // cout << x << endl;
            for(auto elem : scc[x]){
                for(auto edge : graph[elem]) {
                    if(scc_group[edge] == scc_group[elem])continue;
                    compressed_graph[scc_group[elem]].push_back(scc_group[edge]);
                }
                // cout << elem << " ";
                new_hype[x+1] += hype_vector[elem-1];
                // cout << new_hype[x+1] << endl;
            }
        }
        // for(auto elem : new_hype) cout << elem << " ";cout <<  endl;
        vector<int> topo_sort = topoSort(scc.size(),compressed_graph);
        assert(topo_sort.size() == scc.size());
        // for(auto elem : topo_sort) cout << elem << " "; cout << endl;
        vector<int> dp = new_hype;
        for(auto elem : topo_sort) {
            for(auto node : compressed_graph[elem]) {
                // cout << elem << " " << node << " " << dp[node] << " " << new_hype[node] << " " << dp[elem] << endl;
                dp[node] = max(dp[node],new_hype[node] + dp[elem]);
            }
        }
        int max = 0;
        for(auto elem : dp) max = (max > elem ? max : elem);
        cout << max << endl;
    }
protected:
    map<int,vector<int>> graph;
    vector<int> hype_vector;
};

int main(){
    int n,m;
    cin >> n >> m;
    vector<int> hype(n);
    for(int x = 0 ; x < n ; x++) {
        cin >> hype[x];
    }
    map<int,vector<int>> graph;
    for(int x = 0 ; x < m ; x++) {
        int u,v;
        cin >> u >> v;
        graph[u].push_back(v);
    }
    GraphAlgorithm* base_ptr;
    int q;
    cin >> q;
    while(q--) {
        int query;
        cin >> query;
        if(query == 1) {
            base_ptr = new isCycle(graph,n);
            base_ptr->Query();
            delete base_ptr;
        }
        else if(query == 2) {
            base_ptr = new indepComponent(graph,n);
            base_ptr->Query();
            delete base_ptr;
        }
        else if(query == 3) {
            base_ptr = new validOrder(graph,n);
            base_ptr->Query();
            delete base_ptr;
        }
        else if(query == 4) {
            base_ptr = new maxHype(graph,hype);
            base_ptr->Query();
            delete base_ptr;
        }
    }
}