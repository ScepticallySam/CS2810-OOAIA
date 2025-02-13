#include <iostream>
#include <set>
#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include <queue>
using namespace std;


class Graph{
    friend istream& operator >> (istream& stream, Graph& graph);
    friend ostream& operator << (ostream& stream, Graph& graph);
    public:
        Graph() {}
        Graph& operator+ (const Graph& graph) {
            for(auto vertex : graph.vertices) {
                this->vertices.insert(vertex);
            }
            this->size = this->vertices.size();
            for(auto adj_list : graph.edges) {
                for(auto neighbour : adj_list.second) {
                    this->edges[adj_list.first].insert(neighbour);
                }
            }
            return *this;
        }
        Graph& operator- (Graph& graph) {
            for(auto vertex : graph.vertices) {
                this->vertices.insert(vertex);
            }
            this->size = this->vertices.size();
            for(auto adj_list : this->edges) {
                for(auto neighbour : adj_list.second) {
                    if(graph.edges[adj_list.first].find(neighbour) == graph.edges[adj_list.first].end()) {
                        this->edges[adj_list.first].erase(neighbour);
                    }
                }
            }
            return *this;
        }
        Graph& operator! () {
            for(int x = 0 ; x < size ; x++) {
                for(int y = x+1 ; y < size ; y++) {
                    if(!this->remove_edge(x,y)) this->add_edge(x,y);
                }
            }
            // cout << "complements\n";
            return *this;
        }
        void add_edge(int u, int v) {
            edges[u].insert(v);
            edges[v].insert(u);
        }
        bool remove_edge(int u, int v) {
            if(edges[u].find(v) != edges[u].end()) {
                edges[u].erase(v);
                edges[v].erase(u);
                return true;
            }
            return false;
        }
        bool isReachable(int u, int v) {
            vector<bool> visited(this->size,false);
            queue<int> q;
            q.push(u);
            visited[u] = true;
            while(!q.empty() && !visited[v]) {
                int curr = q.front();
                q.pop();
                for(auto neighbour : edges[curr]) {
                    if(!visited[neighbour]) {
                        visited[neighbour] = true;
                        q.push(neighbour);
                        if(neighbour == v)break;
                    }
                }
            }
            return visited[v];
        }
    private:
        set <int> vertices;
        map <int,set<int>> edges;
        int size;
};

istream& operator >> (istream& stream, Graph& graph) {
    int vertices_count, edges_count;
    cin >> vertices_count >> edges_count;
    graph.size = vertices_count;
    for(int x = 0 ; x < vertices_count ; x++) {
        graph.vertices.insert(x);
    }
    while(edges_count--) {
        int u,v;
        cin >> u >> v;
        graph.edges[u].insert(v);
        graph.edges[v].insert(u);
    }
    return stream;
}

ostream& operator << (ostream& stream, Graph& graph) {
    for(int x = 0 ; x < graph.size ; x++) {
        cout << "Vertex " << x << ": ";
        for(auto neighbour : graph.edges[x]) {
            cout << neighbour << " ";
        }
        cout << endl;
    }
    return stream;
}

int main(){
    Graph my_graph;
    string query;
    cin >> query;
    while(query != "end") {
        if(query == "Graph") {
            cin >> my_graph;
        }
        else if(query == "union") {
            Graph g1;
            cin >> query >> g1;
            my_graph + g1;
        }
        else if(query == "intersection") {
            Graph g1;
            cin >> query >> g1;
            my_graph - g1;
        }
        else if(query == "complement") {
            !my_graph;
        }
        else if(query == "isReachable") {
            int u,v;
            cin >> u >> v;
            if(my_graph.isReachable(u,v))cout << "Yes" << endl;
            else cout << "No" << endl;
        }
        else if(query == "add_edge") {
            int u,v;
            cin >> u >> v;
            my_graph.add_edge(u,v);
        }
        else if(query == "remove_edge") {
            int u,v;
            cin >> u >> v;
            my_graph.remove_edge(u,v);
        }
        else if(query == "printGraph") {
            cout << my_graph;
        }
        cin >> query;
    }
}