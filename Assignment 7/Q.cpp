#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cfloat>
#include <cmath>
#include <map>
using namespace std;


double distance(vector<long long int> &v1) {
    if(v1.empty()) return DBL_MAX;
    return sqrtf((v1[0]-v1[2])*(v1[0]-v1[2]) + (v1[1]-v1[3])*(v1[1]-v1[3]));
}

double distance(long long int x1,long long int y1, long long int x2, long long int y2) {
    return sqrtf((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

void MergeSort(vector<long long int>& v, long long int l, long long int r){ //merge sort function
    //base case:
    if(r - l <= 0) return;

    //recursion:
    long long int m = (l+r)/2;
    MergeSort(v,l,m);
    MergeSort(v,m+1,r);
    
    //combination
    vector<long long int> sol;
    long long int ptr1 = l;
    long long int ptr2 = m+1;
    while(ptr1 <= m && ptr2 <= r) {
        if(v[ptr1] < v[ptr2]){
            sol.push_back(v[ptr1]);
            ptr1++;
        }
        else {
            sol.push_back(v[ptr2]);
            ptr2++;
        }
    }
    while(ptr1 <= m) {
        sol.push_back(v[ptr1]);
        ptr1++;
    }
    while(ptr2 <= r) {
        sol.push_back(v[ptr2]);
        ptr2++;
    }
    for(long long int x = 0 ; x <= r-l ; x++) {
        v[x + l] = sol[x];
    }
}

long long int countInversionsHelper(vector<long long int> &v, long long int l, long long int r) {
    //base case:
    if(l >= r) return 0;

    //recursion:
    const long long int m = (l+r)/2;
    long long int p1 = countInversionsHelper(v,l,m);
    long long int p2 = countInversionsHelper(v,m+1,r);
    long long int count = p1 + p2;

    //combination
    vector<long long int> sol;
    long long int ptr1 = l;
    long long int ptr2 = m+1;
    while(ptr1 <= m && ptr2 <= r) {
        if(v[ptr1] <= v[ptr2]){
            sol.push_back(v[ptr1]);
            ptr1++;
        }
        else {
            sol.push_back(v[ptr2]);
            ptr2++;
            count += m - ptr1 + 1; //adding the number of points after a[ptr1] which would be greater than b[ptr2]
        }
    }
    while(ptr1 <= m) {
        sol.push_back(v[ptr1]);
        ptr1++;
    }
    while(ptr2 <= r) {
        sol.push_back(v[ptr2]);
        ptr2++;
    }
    for(long long int x = 0 ; x <= r-l ; x++) {
        v[x + l] = sol[x];
    }
    return count;
}

bool comp(pair<long long int,long long int>& p1, pair<long long int,long long int>& p2) {
    if(p1.second != p2.second)return p1.second < p2.second;
    return p1.first < p2.first;
}

vector<vector<long long int>> closestPairHelper(vector<pair<long long int,long long int>>& points, long long int l, long long int r){
    // base case:
    if(r - l == 1) {
        return {{points[l].first , points[l].second , points[r].first , points[r].second}};
    }
    else if (r - l <= 0) {
        return {{}}; //handled by my distance function
    }
    
    // recursion: 
    long long int m = l + (r-l)/2;
    vector<vector<long long int>> v1 = closestPairHelper(points,l,m);
    vector<vector<long long int>> v2 = closestPairHelper(points,m+1,r);
    double dis1 = v1.empty() ? DBL_MAX : distance(v1[0]);
    double dis2 = v2.empty() ? DBL_MAX : distance(v2[0]);

    // combination:
    double delta = min(dis1,dis2);
    // double xp = points[m].first - delta;
    // double xn = points[m].first + delta;
    vector<vector<long long int>> storage;
    if(dis1 < dis2) storage = v1;
    else if(dis1 > dis2) storage = v2;
    else if(dis1 != DBL_MAX){
        storage = v1;
        for(auto elem : v2)storage.push_back(elem);
    }
    double mind = delta;
    vector<pair<long long int, long long int>> vec;
    long long int left = m, right = m + 1;
    //finding the points in the delta range, going both sides from m
    while (left >= l && points[m].first - points[left].first <= delta) {
        vec.push_back(points[left]);
        left--;
    }
    while (right <= r && points[right].first - points[m].first <= delta) {
        vec.push_back(points[right]);
        right++;
    }
    sort(vec.begin(),vec.end(),comp); //sorting with y coordinates
    for(long long int x = 0 ; x < vec.size() ; x++) {
        for(int y = 1 ; y <= 15 && y + x < vec.size() ; y++) {
            if(vec[x+y].second - vec[x].second > delta) break;
            double d = distance(vec[x].first,vec[x].second,vec[x+y].first,vec[x+y].second);
            if(mind > d) {
                mind = d;
                storage.clear();
                storage.push_back({vec[x].first,vec[x].second,vec[x+y].first,vec[x+y].second});
            }
            else if(mind == d) {
                storage.push_back({vec[x].first,vec[x].second,vec[x+y].first,vec[x+y].second});
            }
        }
    }
    return storage;
} 

class Sorter{
public:
    Sorter(string s){ //initialises mode
        mode = s;
    }
    void operator() (vector<long long int> &v){ //functor
        // cout << "sorting :";
        // for(auto elem : v) cout << elem << " ";
        // cout << endl;
        MergeSort(v,0,v.size()-1);
        if(mode == "descending"){
            for(long long int x = 0 ; x < (v.size())/2 ; x++) {
                long long int temp = v[x];
                v[x] = v[v.size() - 1 - x];
                v[v.size() - 1 - x] = temp;
            }
        }
    }
private:
    string mode; //mentions if its "ascending" or "descending"
};

class Board{
public:
    Board(long long int size){ //initializes the board
        long long int temp;
        vector<long long int> v;
        for(long long int x = 0 ; x < size ; x++) {
            v.clear();
            for(long long int y = 0 ; y < size ; y++) {
                cin >> temp;
                v.push_back(temp);
            }
            board.push_back(v);
        }
        asc = board; 
        des = board;
        //precomputing the sorted matrices
        for_each(des.begin(),des.end(),Sorter("descending"));
        for_each(asc.begin(),asc.end(),Sorter("ascending"));
    }
    void sortRows(string comparator){ //sorts each matrix row with a comparator
        if(comparator == "ascending") board = asc;
        else board = des;
    } 
    long long int countInversions(){ //counts the number of inversions
        vector<long long int> v;
        for(auto row : board) { //converting the board into a linear vector so that its easier to deal with
            for(auto elem : row) v.push_back(elem);
        }
        return countInversionsHelper(v,0,v.size()-1);
    }
    void display(){ //prints out the matrix
        for(long long int x = 0 ; x < board.size() ; x++) {
            for(long long int y = 0 ; y < board[0].size() ; y++) {
                cout << board[x][y] << " ";
            }
            cout << endl;
        }
    }
    vector<long long int> closestPair(vector<pair<long long int,long long int>>& points) {
        vector<pair<long long int,long long int>> v1 = points; //having a sorted copy to maintain the original for checking
        sort(v1.begin(),v1.end());
        vector<vector<long long int>> v = closestPairHelper(v1,0,v1.size()-1);
        map<pair<long long int,long long int>,long long int> idx; //contains the index of an element as value and the point being the key
        for(long long int x = 0 ; x < points.size() ; x++) {
            idx[points[x]] = x;
        }
        long long int mini = 0; //contains the index in v of the best pair
        long long int minv = LONG_MAX; //contains the minimum value
        sort(v.begin(),v.end());
        // for(long long int x = 0 ; x < v.size() ; x++) {
        //     long long int a1 = min(idx[{v[x][0],v[x][1]}],idx[{v[x][2],v[x][3]}]);
        //     if(a1 < minv) {
        //         minv = a1;
        //         mini = x;
        //     }
        // }
        if(idx[{v[mini][0],v[mini][1]}] > idx[{v[mini][2],v[mini][3]}]) { //swapping into the required order
            swap(v[mini][0],v[mini][2]);
            swap(v[mini][1],v[mini][3]);
        }
        return v[mini];
    }
private:
    vector<vector<long long int>> board;
    vector<vector<long long int>> asc; //storing ascending row ordered board
    vector<vector<long long int>> des; //storing descending row ordered board
};

int main(){
    // Merge Sort Test
    // vector<int> v1 = {1,4,67,2,3,4,6,3,12};
    // for(auto elem : v1)cout << elem << " ";
    // cout << endl;
    // v1 = MergeSort(v1,0,v1.size()-1);
    // for(auto elem : v1)cout << elem << " ";
    // cout << endl;


    string query;
    cin >> query;
    Board* board = nullptr;
    while(query != "END") {
        if(query == "CREATE_2D") {
            long long int sz;
            cin >> sz;
            board = new Board(sz);
        }
        else if(query == "SORT_2D") {
            string mode;
            cin >> mode;
            board->sortRows(mode);
        }
        else if(query == "INVERSION_2D") {
            cout << board->countInversions() << endl;
        }
        else if(query == "DISPLAY_2D") {
            board->display();
        }
        else if(query == "CLOSEST_2D") {
            long long int num_points;
            cin >> num_points;
            vector<pair<long long int,long long int>> points;
            for(long long int i = 0 ; i < num_points ; i++) {
                long long int x,y;
                cin >> x >> y;
                points.push_back(make_pair(x,y));
            }
            vector<long long int> closest = board->closestPair(points);
            cout << closest[0] << " " << closest[1] << " " << closest[2] << " " << closest[3] << endl;
        }
        cin >> query;
    }
    delete board;
}