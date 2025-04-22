#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <climits>
#include <unordered_map>

using namespace std;

template <typename T> //ASSUMPTION: this T is always of pointer type
class MinHeap { //My MinHeap uses 1-indexing
public:
    MinHeap(); //constructor

    const size_t size() const; //returns size
    void push(const T& elem); //pushes element into the pq
    void pop(); //pops element from the pq
    const T& top() const; //returns the top element from the pq
    const bool isEmpty() const; //checks if the heap is empty or not
private:
    vector<T> heap;
    size_t heap_size;
    unordered_map<T,int> mapping;
    void percolate_up(size_t idx); //pushes element up the heap
    void percolate_down(size_t idx); //pushes element down the heap
};

class Node {
// friend bool operator< (Node* n1, Node* n2);
public:
    Node();
    Node(string roomID);
    
    void add_path(Node* neighbour, long long int weight);
    const string get_roomID() const;
    const vector<pair<Node*,long long int>>& get_neighbours() const;
    const long long int get_shortest_source_path() const;
    void set_shortest_source_path(long long int val); 

    bool operator<(Node& rhs) {
        return this->shortest_source_path < rhs.shortest_source_path;
    }
private:
    string roomID;
    vector<pair<Node*,long long int>> neighbours; //contains the roomID's of the neighbouring rooms and path weights to each of them
    long long int shortest_source_path; //will be set to LONG_LONG_MAX initially
};

// bool operator< (Node* n1, Node* n2) {
//     return (*n1) < (*n2);
// }

//Node class
Node::Node() : shortest_source_path(LLONG_MAX) {}
Node::Node(string name) : roomID(name) , shortest_source_path(LLONG_MAX) {}

void Node::add_path(Node* neighbour, long long int weight) {neighbours.push_back(make_pair(neighbour,weight));}
const string Node::get_roomID() const {return roomID;}
const vector<pair<Node*,long long int>>& Node::get_neighbours() const {return neighbours;}
const long long int Node::get_shortest_source_path() const {return shortest_source_path;}
void Node::set_shortest_source_path(long long int val) {shortest_source_path = val;}

//MinHeap class
template<typename T>
MinHeap<T>::MinHeap() {
    heap.resize(8);
    heap_size = 0;
}

template<typename T>
const size_t MinHeap<T>::size() const {return heap_size;}
template<typename T>
const T& MinHeap<T>::top() const {return heap[1];}
template<typename T>
const bool MinHeap<T>::isEmpty() const {return (heap_size == 0);}
template<typename T>
void MinHeap<T>::percolate_up(size_t idx) {
    if(isEmpty())return;
    T elem = heap[idx];
    while(idx/2 > 0 && *elem < (*heap[idx/2])) {
        heap[idx] = heap[idx/2];
        mapping[heap[idx]] = idx;
        idx = idx/2;
    }
    heap[idx] = elem;
    mapping[heap[idx]] = idx;
}
template<typename T>
void MinHeap<T>::percolate_down(size_t idx) {
    if(isEmpty())return;
    T elem = heap[idx];
    while(idx <= heap_size) {
        size_t child1 = 2*idx;
        size_t child2 = 2*idx + 1;
        T temp = new typename std::remove_pointer<T>::type(); //Used to convert T back to the type that it is pointing to
        T elem1 = ((child1 < heap_size) ? (heap[child1]) : temp); 
        T elem2 = ((child2 < heap_size) ? (heap[child2]) : temp);
        size_t child_min = (*elem1 < *elem2 ? child1 : child2);
        delete temp;
        if(child_min >= heap_size || *elem < (*heap[child_min]))break;
        heap[idx] = heap[child_min];
        mapping[heap[idx]] = idx;
        idx = child_min;
    }
    heap[idx] = elem;
    mapping[heap[idx]] = idx;
}
template <typename T>
void MinHeap<T>::push(const T& elem) {
    if(mapping[elem] != 0) {
        percolate_up(mapping[elem]);
        return;
    }
    heap_size++;
    mapping[elem] = heap_size;
    if(heap_size >= heap.size()) heap.resize(2*heap.size());
    heap[heap_size] = elem;
    percolate_up(heap_size);
}
template <typename T>
void MinHeap<T>::pop() {
    mapping[heap[1]] = 0;
    heap[1] = heap[heap_size];
    percolate_down(1);
    heap_size--;
}

long long int Dijsktra(unordered_map<string,Node*> node_mapping, string source, string destination) {
    unordered_map<string,long long int> min_dists;
    unordered_map<string,bool> visited;
    MinHeap<Node*> pq;
    for(auto elem : node_mapping) min_dists[elem.first] = LLONG_MAX;
    min_dists[source] = 0;
    node_mapping[source]->set_shortest_source_path(0);
    pq.push(node_mapping[source]);
    while(!pq.isEmpty() && visited[destination] == false) {
        Node* curr = pq.top();
        visited[curr->get_roomID()] = true;
        pq.pop();
        auto neighbours = curr->get_neighbours();
        for(auto neighbour : neighbours) {
            if(!visited[neighbour.first->get_roomID()] && min_dists[curr->get_roomID()] + neighbour.second < min_dists[neighbour.first->get_roomID()]) {
                neighbour.first->set_shortest_source_path(curr->get_shortest_source_path() + neighbour.second);
                min_dists[neighbour.first->get_roomID()] = min_dists[curr->get_roomID()] + neighbour.second;
                pq.push(neighbour.first);
            }
        }
    }
    if(min_dists[destination] == LLONG_MAX)return -1;
    return min_dists[destination];
}

int main(){
    int n,m;
    cin >> n >> m;
    unordered_map<string,Node*> node_mapping;
    //Algorithm: simulate the transtion of even and odd no of edges via odd and even variants of each node by giving the neighbours of the odd vertex as the even variants of the original neighbours and the other way round as well. Now running Dijktra between even source to even destination should work.
    for(int x = 0 ; x < n ; x++) {
        string name;
        cin >> name;
        Node* temp = new Node("o" + name);
        node_mapping["o" + name] = temp;
        Node* temp1 = new Node("e" + name);
        node_mapping["e" + name] = temp1;
    }
    for(int x = 0 ; x < m ; x++) {
        string r1,r2;
        long long int weight;
        cin >> r1 >> r2 >> weight;
        Node* temp1o = node_mapping["o" + r1];
        Node* temp2e = node_mapping["e" + r2];
        Node* temp1e = node_mapping["e" + r1];
        Node* temp2o = node_mapping["o" + r2];
        temp1o->add_path(temp2e,weight);
        temp2e->add_path(temp1o,weight);
        temp1e->add_path(temp2o,weight);
        temp2o->add_path(temp1e,weight);
    }
    string source,destination;
    cin >> source >> destination;
    source = "e" + source;
    destination = "e" + destination;
    cout << Dijsktra(node_mapping,source,destination) << endl;
}