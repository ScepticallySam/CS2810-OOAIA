#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Card{
friend istream& operator>> (istream& stream, Card& card);
friend ostream& operator<< (ostream& stream, Card& card);
public:
    //Constructor
    Card();
    //Operator Overloading
    bool operator< (const Card& rhs) const {
        return this->area < rhs.area;
    }
    bool operator<= (const Card& rhs) const {
        return this->area <= rhs.area;
    }
    //Other functions
    virtual istream& input_assist (istream& stream) = 0;
    virtual ostream& output_assist (ostream& stream) = 0;
    virtual void set_area() = 0;
    long long int get_area();
protected:
    long long int area;
};

istream& operator>> (istream& stream, Card& card) {
    return card.input_assist(stream);
}
ostream& operator<< (ostream& stream, Card& card) {
    return card.output_assist(stream);
}

class Square: public Card {
public:
    //Constructor
    Square();

    //Other functions
    istream& input_assist (istream& stream);
    ostream& output_assist (ostream& stream);
    void set_area();

private:
    long long int side;
};

class Triangle: public Card {
public:
    //Constructor
    Triangle();

    //Other functions
    istream& input_assist (istream& stream);
    ostream& output_assist (ostream& stream);
    void set_area();

private:
    long long int base;
    long long int height;
};

class Rectangle: public Card {
public:
    //Constructor
    Rectangle();

    //Other functions
    istream& input_assist (istream& stream);
    ostream& output_assist (ostream& stream);
    void set_area();

private:
    long long int length;
    long long int breadth;
};

class CardOrganizer {
public:
    //constructor
    CardOrganizer();
    void addCard(Card* card);
    size_t getPileCount() const;
    vector<Card*> getLIS() const;
    void organize();
private:
    vector<vector<Card*>> game_state; 
    vector<Card*> card_sequence;
    vector<int> area_seq; 
    vector<int> v;
};

//Card
Card::Card(): area(0) {}
long long int Card::get_area() {return area;}

//Square
Square::Square(): side(0) {}
istream& Square::input_assist (istream& stream) {
    stream >> side;
    set_area();
    return stream;
}
ostream& Square::output_assist (ostream& stream) {
    stream << "SQUARE " << side << "\n";
    return stream;
}
void Square::set_area () {
    this->area = side*side;
}

//Triangle
Triangle::Triangle() : height(0), base(0) {}
istream& Triangle::input_assist (istream& stream) {
    stream >> base >> height;
    set_area();
    return stream;
}
ostream& Triangle::output_assist (ostream& stream) {
    stream << "TRIANGLE " << base << " " << height << "\n";
    return stream;
}
void Triangle::set_area() {
    area = base*height/2;
}

//Rectangle
Rectangle::Rectangle() : length(0), breadth(0) {}
istream& Rectangle::input_assist (istream& stream) {
    stream >> length >> breadth;
    set_area();
    return stream;
}
ostream& Rectangle::output_assist (ostream& stream) {
    stream << "RECTANGLE " << length << " " << breadth << "\n";
    return stream;
}
void Rectangle::set_area() {
    area = length*breadth;
}

//CardOrganizer class
CardOrganizer::CardOrganizer() {}
void CardOrganizer::addCard(Card* card) {
    card_sequence.push_back(card);
    v.push_back(0);
    organize();
    for(auto &pile : game_state) {
        if((*card) <= (*pile[pile.size()-1])) {
            pile.push_back(card);
            return;
        }
    }
    vector<Card*> new_pile;
    new_pile.push_back(card);
    game_state.push_back(new_pile);
}
size_t CardOrganizer::getPileCount() const {
    return game_state.size();
}
vector<Card*> CardOrganizer::getLIS() const {
    // vector<int> v(card_sequence.size(),-1); //notes down where the card can be placed in the LIS
    // vector<long long int> area_seq;
    // for(int x = LIS_indices.size() ; x < card_sequence.size() ; x++) {
    //     auto itr = lower_bound(area_seq.begin(),area_seq.end(),card_sequence[x]->get_area());
    //     if(itr == area_seq.end()) {
    //         area_seq.push_back(card_sequence[x]->get_area());
    //         v[x] = area_seq.size()-1;
    //     }
    //     else {
    //         *itr = card_sequence[x]->get_area();
    //         v[x] = itr - area_seq.begin();
    //     }
    // }
    int num = area_seq.size()-1;
    vector<Card*> sequence;
    for(int x = v.size()-1 ; x >= 0 ; x--) {
        if(v[x] == num) {
            sequence.push_back(card_sequence[x]);
            num--;
        }
        if(num == -1)break;
    }
    reverse(sequence.begin(),sequence.end());
    return sequence;
}
void CardOrganizer::organize() {
    int x = card_sequence.size()-1;
    auto itr = lower_bound(area_seq.begin(),area_seq.end(),card_sequence[x]->get_area());
    if(itr == area_seq.end()) {
        area_seq.push_back(card_sequence[x]->get_area());
        v[x] = area_seq.size()-1;
    }
    else {
        *itr = card_sequence[x]->get_area();
        v[x] = itr - area_seq.begin();
    }
}

int main(){
    CardOrganizer GeoSolitaireBot;
    int q;
    cin >> q;
    while(q--) {
        int query;
        cin >> query;
        if (query == 1) {
            string shape;
            cin >> shape;
            if(shape == "SQUARE") {
                Square card;
                cin >> card;
                Square* card_ptr = new Square(card);
                GeoSolitaireBot.addCard(card_ptr);
            }
            else if(shape == "RECTANGLE") {
                Rectangle card;
                cin >> card;
                Rectangle* card_ptr = new Rectangle(card);
                GeoSolitaireBot.addCard(card_ptr);
            }
            else if(shape == "TRIANGLE") {
                Triangle card;
                cin >> card;
                Triangle* card_ptr = new Triangle(card);
                GeoSolitaireBot.addCard(card_ptr);
            }
            else cout << "Not a valid shape\n";
        }
        else if (query == 2) {
            cout << GeoSolitaireBot.getPileCount() << "\n";
        }
        else if (query == 3) {
            vector<Card*> LIS = GeoSolitaireBot.getLIS();
            cout << LIS.size() << "\n";
            for(auto elem : LIS) cout << *elem;
        }
        else cout << "You are not supposed to be here :|\n";
    }
}