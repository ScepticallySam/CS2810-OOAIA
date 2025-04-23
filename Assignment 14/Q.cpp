#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cmath>
#include <stack>
using namespace std;

#define DEBUG cout<<"This line ran\n";
typedef pair<int,int> Point;
#define X first 
#define Y second

double distance(Point &p1, Point &p2) {
    return sqrtl(((p2.X - p1.X) * (p2.X - p1.X)) + ((p2.Y - p1.Y) * (p2.Y - p1.Y)));
}

class Island {
friend ostream& operator<< (ostream& stream, Island& island);
friend istream& operator>> (istream& stream, Island& island);
public:
    //constructor
    Island() {}

    //other functions
    string getID() const {return ID;}
    void setID(string ID) {this->ID = ID;}
    Point getCenter() const {return center;}
    virtual void setCenter() = 0;
    double getRadius() const {return radius;}
    virtual void setRadius() = 0;
protected:
    virtual istream& input_helper(istream& stream) = 0;
    virtual ostream& output_helper(ostream& stream) = 0;

    double radius;
    Point center;
    string ID;
};

ostream& operator<< (ostream& stream, Island& island) {
    return island.output_helper(stream);
};

istream& operator>> (istream& stream, Island& island) {
    return island.input_helper(stream);
}

class Triangle : public Island {
public:
    //constructor
    Triangle() {}

    //other functions
    void setCenter();
    void setRadius();
private:
    istream& input_helper(istream& stream);
    ostream& output_helper(ostream& stream);

    Point p1,p2,p3;
};

class Rectangle : public Island {
public:
    //constructor
    Rectangle() {}

    //other functions
    void setCenter();
    void setRadius();
private:
    istream& input_helper(istream& stream);
    ostream& output_helper(ostream& stream);

    Point p1,p2,p3,p4;
};

class Circle : public Island {
public:
    //constructor
    Circle() {}

    //other functions
    void setCenter();
    void setRadius();
private:
    istream& input_helper(istream& stream);
    ostream& output_helper(ostream& stream);
};

bool isReachable(Island* i1, Island* i2) {
    Point center1 = i1->getCenter();
    Point center2 = i2->getCenter();
    return i1->getRadius() + i2->getRadius() >= distance(center1,center2);
}

int setBitsCount(int a) {
    int count = 0;
    while(a > 0) {
        count += a%2;
        a/=2;
    }
    return count;
}

vector<int> setBitsIndex(int a) {
    vector<int> p;
    for(int i = 0 ; i < 32 ; i++) {
        if(a%2)p.push_back(i);
        a/=2;
    }
    return p;
}

vector<int> ordering(int n) {
    vector<vector<int>> p (n+1,vector<int>(0));
    for(int x = 1 ; x < (1 << n) - 1 ; x++) {
        p[setBitsCount(x)].push_back(x);
    }
    vector<int> a;
    for(auto vec : p) {
        for(auto elem : vec) {
            a.push_back(elem);
        }
    }
    return a;
}

vector<int> maxPath(vector<vector<int>>& graph) {
    int n = graph.size();
    vector<vector<pair<int,int>>> dp ((1 << n),vector<pair<int,int>>(n,make_pair(-1,-1)));
    vector<int> order = ordering(n);
    for(int i = 0 ; i < n ; i++) {
        dp[0][i] = {0,i}; 
    }
    pair<int,int> max_val = {0,0};
    int node = 0;
    for(auto num : order) {
        for(int i = 0 ; i < n ; i++) {
            vector<int> setBits = setBitsIndex(num);
            for(auto elem : setBits) {
                if(graph[elem][i] == 1 && dp[num - (1 << elem)][elem].first != -1 && (num & 1 << i) == 0){
                    dp[num][i] = {num - (1 << elem),elem};
                    max_val = {num,i};
                    node = i;
                    break;
                }
            }
        }
    }
    stack<pair<int,pair<int,int>>> st;
    st.push({node,{dp[max_val.first][max_val.second]}});
    while(st.top().first != st.top().second.second) {
        // cout << st.top().second.first << " " << st.top().second.second << endl;
        st.push({st.top().second.second,dp[st.top().second.first][st.top().second.second]});
    }
    vector<int> path;
    while(!st.empty()) {
        path.push_back(st.top().first);
        st.pop();
    }
    // if(path.size() != 1)path.pop_back();
    return path;
}

//Triangle
void Triangle::setCenter() {
    int x = (p1.X + p2.X + p3.X)/3;
    int y = (p1.Y + p2.Y + p3.Y)/3;
    center = {x,y};
}
void Triangle::setRadius() {
    double d1 = distance(p1,center);
    double d2 = distance(p2,center);
    double d3 = distance(p3,center);
    double m1 = (d1 > d2 ? d1 : d2);
    radius = (m1 > d3 ? m1 : d3);
}
istream& Triangle::input_helper(istream& stream) {
    string ID;
    stream >> ID;
    setID(ID);
    int x,y;
    stream >> x >> y;
    p1 = {x,y};
    stream >> x >> y;
    p2 = {x,y};
    stream >> x >> y;
    p3 = {x,y};
    setCenter();
    setRadius();
    return stream;
}
ostream& Triangle::output_helper(ostream& stream) {
    stream << ID;
    return stream;
}

//Recatangle
void Rectangle::setCenter() {
    int x = (p1.X + p2.X + p3.X + p4.X)/4;
    int y = (p1.Y + p2.Y + p3.Y + p4.Y)/4;
    center = {x,y};
}
void Rectangle::setRadius() {
    radius = distance(center,p1);
}
istream& Rectangle::input_helper(istream& stream) {
    string ID;
    stream >> ID;
    setID(ID);
    int x,y;
    stream >> x >> y;
    p1 = {x,y};
    stream >> x >> y;
    p2 = {x,y};
    stream >> x >> y;
    p3 = {x,y};
    stream >> x >> y;
    p4 = {x,y};
    setCenter();
    setRadius();
    return stream;
}
ostream& Rectangle::output_helper(ostream& stream) {
    stream << ID;
    return stream;
}

//Circle
void Circle::setCenter() {
    //nothing since center will be whatever inputted
}
void Circle::setRadius() {
    //nothing since radius will be whatever inputted
}
istream& Circle::input_helper(istream& stream) {
    string ID;
    stream >> ID;
    setID(ID);
    int x,y;
    stream >> x >> y;
    center = {x,y};
    double r;
    stream >> r;
    radius = r;
    return stream;
}
ostream& Circle::output_helper(ostream& stream) {
    stream << ID;
    return stream;
}

int main() {
    vector<Island*> islands;
    int n;
    cin >> n;
    for(int x = 0 ; x < n ; x++) {
        string query;
        cin >> query;
        if(query == "TRIANGLE") {
            Triangle* t = new Triangle;
            cin >> *t;
            islands.push_back(t);
        }
        else if(query == "RECTANGLE") {
            Rectangle* r = new Rectangle;
            cin >> *r;
            islands.push_back(r);
        }
        else if(query == "CIRCLE") {
            Circle* c = new Circle;
            cin >> *c;
            islands.push_back(c);
        }
        else cout << "Invalid Shape!!" << endl;
    }
    // for(auto elem : islands) cout << *elem << " "; cout << endl;
    vector<vector<int>> graph(n,vector<int>(n)); //each node's interger value corresponds to it vector position in islands

    for(int x = 0 ; x < n ; x++) {
        for(int y = x+1 ; y < n ; y++) {
            if(isReachable(islands[x],islands[y])) {
                graph[x][y] = 1;
                graph[y][x] = 1;
                // cout << x << " " << y << endl;
            }
        }
    }

    vector<int> path = maxPath(graph);
    if(path.size() == n) cout << "YES\n";
    else cout << "NO\n" << path.size() << "\n";
    for(auto island : path) cout << *(islands[island]) << " "; cout << "\n";
}