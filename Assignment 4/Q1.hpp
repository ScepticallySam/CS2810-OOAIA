#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <queue>
using namespace std;

//Quantum Nano Suits class
class QNS {
    public:

        //constructors & desctuctor
        QNS(int P = 1000, int D = 500, int E = 300, int H = 0);
        QNS(QNS &Suit);
        ~QNS();

        //operator overloadings
        QNS& operator+ (QNS& upgradeSuit);
        bool operator- (int damage);
        bool operator* (int amplificationRate);
        void operator/ (int cool);
        bool operator== (QNS& otherSuit);
        bool operator< (QNS& otherSuit);

        //getters and setters
        int getP() {return powerLevel;}
        int getD() {return durability;}
        int getE() {return energyStorage;}
        int getH() {return heatLevel;}
        void setP(int P);queue<QNS> armory;

        void setD(int D);
        void setE(int E);
        void setH(int H);

        //other methods
        void boostPower(int factor);
        void boostPower(QNS otherSuit);

    private:

        //attributes
        int powerLevel;
        int durability;
        int energyStorage;
        int heatLevel;
};

queue<QNS> armory;

class Avenger {
    public:
        
        //constructor and destructor
        Avenger(string avName, QNS avSuit, int strength);
        ~Avenger();

        //other methods
        void attack(Avenger& enemy);
        void upgradeSuit();
        void repair(int x);
        void printStatus();
        int getSuitVal() {return suit.getP() + suit.getD();}
        void findArmoryAndUpgrade(queue<int>& armory) {
            
        }

    private:

        //attributes
        string name;
        QNS suit;
        int attackStrength;
};

class Battle {
    public:

        //constructor and destructor
        Battle();
        ~Battle();

        //methods
        void startBattle();
        void printBattleLog();
        int Result();
    private:

        //attributes
        vector<Avenger> heroes;
        vector<Avenger> enemies;
        vector<string> battleLog;
};