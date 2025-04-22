#include <iostream>
#include <vector>
#include <algorithm>
#define DEBUG cout <<"E" <<endl;
using namespace std;

template <typename T> class Kingdom;

class Sentinel {
public:
    //constructors
    Sentinel();

    //other functions
    virtual int countHigherRanked(Kingdom<Sentinel>& kingdom) = 0;
    int get_id();
    string get_name();
    string get_rank();
protected:
    int id;
    string name;
    string rank;
};

class Senapati : public Sentinel {
public:
    Senapati();
    Senapati(int id, string name, string rank);
    int countHigherRanked(Kingdom<Sentinel>& kingdom);
private:

};

class Dandanayaka : public Sentinel {
public:
    Dandanayaka();
    Dandanayaka(int id, string name, string rank);
    int countHigherRanked(Kingdom<Sentinel>& kingdom);
private:

};

class Chaturangini : public Sentinel {
public:
    Chaturangini();
    Chaturangini(int id, string name, string rank);
    int countHigherRanked(Kingdom<Sentinel>& kingdom);
private:

};

class HierarchyChecker{
public:
    bool operator() (Sentinel* s1, Sentinel* s2);
};

template <typename T>
class Kingdom {
public:
    //constructor
    Kingdom();
    Kingdom(int n);

    //other functions
    void addRoad(int n1, int n2);
    void addSentinel(int node, T* sentinel);
    int countHigherRanked(string rank);
    int minVertexCovering();
    vector<T*> ordering();
    int countHigherRankedHelper(int id);
private:
    vector<vector<int>> graph;
    vector<T*> sentinelList;
    int SenapatiCount;
    int DandanayakaCount;
    int ChaturanginiCount;
};

//Sentinel class
Sentinel::Sentinel() {}
int Sentinel::get_id() {return this->id;}
string Sentinel::get_name() {return this->name;}
string Sentinel::get_rank() {return this->rank;}

//Senapati class
Senapati::Senapati() {}
Senapati::Senapati(int id, string name, string rank) {
    this->id = id;
    this->name = name;
    this->rank = rank;
}
int Senapati::countHigherRanked(Kingdom<Sentinel>& kingdom) {
    return kingdom.countHigherRanked(this->get_rank());
}

//Dandanayaka class
Dandanayaka::Dandanayaka() {}
Dandanayaka::Dandanayaka(int id, string name, string rank) {
    this->id = id;
    this->name = name;
    this->rank = rank;
}
int Dandanayaka::countHigherRanked(Kingdom<Sentinel>& kingdom) {
    return kingdom.countHigherRanked(this->get_rank());
}

//Chaturangini class
Chaturangini::Chaturangini() {}
Chaturangini::Chaturangini(int id, string name, string rank) {
    this->id = id;
    this->name = name;
    this->rank = rank;
}
int Chaturangini::countHigherRanked(Kingdom<Sentinel>& kingdom) {
    return kingdom.countHigherRanked(this->get_rank());
}

//HierarchyChecker class
bool HierarchyChecker::operator() (Sentinel* s1, Sentinel* s2) {
    if(s1->get_rank() == s2->get_rank()) return s1->get_id() < s2->get_id();
    if(s1->get_rank() == "SENAPATI") return true;
    if(s2->get_rank() == "SENAPATI") return false;
    if(s1->get_rank() == "DANDANAYAKA") return true;
    return false;
}

//Kingdom class
template <typename T>
Kingdom<T>::Kingdom(): SenapatiCount(0), DandanayakaCount(0), ChaturanginiCount(0) {}
template <typename T>
Kingdom<T>::Kingdom(int n): SenapatiCount(0), DandanayakaCount(0), ChaturanginiCount(0) {
    graph.resize(n);
    sentinelList.resize(n);
}
template <typename T>
void Kingdom<T>::addRoad(int n1, int n2) {
    graph[n1].push_back(n2);
    graph[n2].push_back(n1);
}
template <typename T>
void Kingdom<T>::addSentinel(int node, T* sentinel) {
    sentinelList[node] = sentinel;
    if(sentinel->get_rank() == "SENAPATI") SenapatiCount++;
    else if(sentinel->get_rank() == "DANDANAYAKA") DandanayakaCount++;
    else if(sentinel->get_rank() == "CHATURANGINI") ChaturanginiCount++; 
    else cout << "Sentinel Type Not Found: " << sentinel->get_rank() << "\n";
}
template <typename T>
int Kingdom<T>::countHigherRanked(string rank) {
    if(rank == "SENAPATI") return 0;
    else if(rank == "DANDANAYAKA") return SenapatiCount;
    else if(rank == "CHATURANGINI") return SenapatiCount + DandanayakaCount;
    cout << "Sentinel Type Not Found: " << rank << "\n";
    return -1; //to show that the sentinel type is not found;
}
template <typename T>
vector<T*> Kingdom<T>::ordering() {
    vector<T*> order = sentinelList;
    HierarchyChecker h;
    sort(order.begin(),order.end(),h);
    // DEBUG
    return order;
}   
int dfs(vector<vector<int>>& graph, vector<vector<int>>& dp, int node, vector<bool>& visited) {
    if(visited[node] == true)return 0;
    visited[node] = true;
    for(auto elem : graph[node]) {
        if(visited[elem])continue;
        dfs(graph,dp,elem,visited);
        dp[node][1] += dp[elem][0];
        dp[node][0] += min(dp[elem][1],dp[elem][0]);
    }
    dp[node][0]++;
    return min(dp[node][0],dp[node][1]);
}
template <typename T>
int Kingdom<T>::minVertexCovering() {
    vector<vector<int>> dp(sentinelList.size(),vector<int>(2,0)); //vec[x][0] -> colored, vec[x][1] -> not colored
    vector<bool> visited(sentinelList.size(),false);
    return dfs(this->graph,dp,0,visited);
}
template <typename T>
int Kingdom<T>::countHigherRankedHelper(int id) {
    return sentinelList[id]->countHigherRanked(*this);
}

int main() {
    int n;
    cin >> n;
    Kingdom<Sentinel> MauryanEmpire(n);

    //edge inputs
    for(int x = 0 ; x < n-1 ; x++) {
        int n1,n2;
        cin >> n1 >> n2;
        // cout << n1 << " " << n2 << endl;
        MauryanEmpire.addRoad(n1,n2);
    }

    // DEBUG
    //nodes inputs
    for(int x = 0 ; x < n ; x++) {
        string name, rank;
        cin >> name >> rank;
        // cout << name << " " << rank << endl;
        Sentinel* temp = nullptr;
        if(rank == "SENAPATI") temp = new Senapati(x,name,rank);
        else if(rank == "DANDANAYAKA") temp = new Dandanayaka(x,name,rank);
        else if(rank == "CHATURANGINI") temp = new Chaturangini(x,name,rank);
        else cout << "invalid rank " << rank << "\n";
        MauryanEmpire.addSentinel(x,temp);
    }
    
    //precomputations, since the tree won't change after initialization
    vector<Sentinel*> v = MauryanEmpire.ordering(); 
    int minVertexCover = MauryanEmpire.minVertexCovering();
    //queries
    int q;
    cin >> q;

    for(int x = 0 ; x < q ; x++) {
        int query;
        cin >> query;
        if(query == 1) {
            cout << minVertexCover << "\n";
        }
        else if(query == 2) {
            for(auto sentinel : v)cout << sentinel->get_id() << " "; cout << "\n";
        }
        else if(query == 3) {
            int id;
            cin >> id;
            cout << MauryanEmpire.countHigherRankedHelper(id) << "\n";
        }
        else {
            cout << "Invalid Query\n";
        }
    }
}