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

        //constructors and destructor
        QNS(int P = 1000, int D = 500, int E = 300, int H = 0) {
            this->powerLevel = P;
            this->powerLevel = min(this->powerLevel,5000);
            this->heatLevel = max(this->heatLevel,0);
            this->durability = D;
            this->energyStorage = E;
            this->heatLevel = max(H,0);
        }
        QNS(const QNS &suit) {
            this->powerLevel = suit.powerLevel;
            this->durability = suit.durability;
            this->energyStorage = suit.energyStorage;
            this->heatLevel = suit.heatLevel;
            this->powerLevel = min(this->powerLevel,5000);
            this->heatLevel = max(this->heatLevel,0);
        }

        //operator overloadings
        QNS& operator+ (QNS& upgradeSuit) {
            this->powerLevel += upgradeSuit.getE();
            this->durability += upgradeSuit.getD();
            this->energyStorage += upgradeSuit.getP();
            this->powerLevel = min(this->powerLevel,5000);
            this->powerLevel = min(this->powerLevel,5000);
            this->heatLevel = max(this->heatLevel,0);
            return *this;
        }
        int operator- (int damage) {
            this->durability -= damage;
            this->energyStorage += damage;
            this->heatLevel += damage;
            this->powerLevel = min(this->powerLevel,5000);
            this->heatLevel = max(this->heatLevel,0);
            if(this->durability <= 0)return 1;
            else if(this->heatLevel > 500)return 2;
            else return 0;
        }
        bool operator* (int amplificationRate) {
            this->powerLevel += (this->powerLevel * amplificationRate)/100;
            this->energyStorage += 5*amplificationRate;
            this->heatLevel += amplificationRate;
            this->powerLevel = min(this->powerLevel,5000);
            this->heatLevel = max(this->heatLevel,0);
            this->powerLevel = min(this->powerLevel,5000);
            return this->heatLevel <= 500;
        }
        void operator/ (int cool) {
            this->durability += cool;
            this->heatLevel -= cool;
            this->powerLevel = min(this->powerLevel,5000);
            this->heatLevel = max(this->heatLevel,0);
            if(this->heatLevel < 0)this->heatLevel = 0;
        }
        bool operator== (QNS& otherSuit) {
            return (this->powerLevel == otherSuit.getP()) && (this->durability == otherSuit.getD());
        }
        bool operator< (QNS& otherSuit) {
            return ((this->powerLevel + this->durability) < (otherSuit.getP() + otherSuit.getD()));
        }

        //getters and setters
        int getP() {return powerLevel;}
        int getD() {return durability;}
        int getE() {return energyStorage;}
        int getH() {return heatLevel;}
        void setP(int P) {this->powerLevel = P;}
        void setD(int D) {this->durability = D;}
        void setE(int E) {this->energyStorage = E;}
        void setH(int H) {this->heatLevel = H;}

        //other methods
        bool boostPower(int factor) {
            return this->operator*(factor);
        }
        void boostPower(QNS otherSuit) {
            this->operator+(otherSuit);
        }

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
        
        //constructor
        Avenger() {}
        Avenger(string avName, QNS avSuit, int strength) {
            this->name = avName;
            this->suit = avSuit;
            this->attackStrength = strength;
        }
        ~Avenger() {}

        //other methods
        int attack(Avenger& enemy) {
            return (enemy.suit - attackStrength);
        }
        void upgradeSuit() {
            this->suit + (armory.front());
            armory.pop();
        }
        void repair(int x) {
            this->suit / x;
        }
        void printStatus() {
            cout << name << " " << this->suit.getP() << " " << this->suit.getD() << " " << this->suit.getE() << " " << this->suit.getH() << endl;
        }
        int getSuitVal() {return suit.getP() + suit.getD();}
        bool boostPower(int x) {return suit.boostPower(x);}
        void boostPower(QNS osuit) {suit.boostPower(osuit);}
        int getD() {return suit.getD();}
        int getH() {return suit.getH();}

    private:

        //attributes
        string name;
        QNS suit;
        int attackStrength;
};

class Battle {
    public:

        //methods
        Battle() {
            int k,n,m;
            cin >> k >> n >> m;
            while(k--) {
                int p,d,e,h;
                cin >> p >> d >> e >> h;
                QNS q(p,d,e,h);
                armory.push(q);
            }
            while(n--) {
                string name;
                int dmg;
                cin >> name >> dmg;
                if(armory.empty()) {
                    cout << (name + " is out of fight") << endl;
                    continue;
                }
                QNS front = armory.front();
                armory.pop();
                Avenger lukeSkywalker(name,front,dmg);
                heroes.push_back(lukeSkywalker);
                hmap[name] = heroes.size()-1;
            }
            while(m--) {
                string name;
                int dmg;
                cin >> name >> dmg;
                if(armory.empty()) {
                    cout << (name + " is out of fight") << endl;
                    continue;
                }
                QNS front = armory.front();
                armory.pop();
                Avenger anakinSkywalker(name,front,dmg);
                enemies.push_back(anakinSkywalker);
                emap[name] = enemies.size()-1;
            }
        }
        void startBattle() {
            string query;
            cin >> query;
            while(query != "End") {
                if(query == "Attack") {
                    string a1,a2;
                    cin >> a1 >> a2;
                    int s = 0;
                    if(hmap.find(a1) != hmap.end() && emap.find(a2) != emap.end()) {
                        Avenger& av1 = heroes[hmap[a1]];
                        Avenger& av2 = enemies[emap[a2]];
                        if(av1.getD() > 0 && av1.getH() <= 500 && av2.getD() > 0) {
                            s = av1.attack(av2);
                            battleLog.push_back(a1 + " attacks " + a2);
                        }
                    }
                    else if(emap.find(a1) != emap.end() && hmap.find(a2) != hmap.end()) {
                        Avenger& av2 = heroes[hmap[a2]];
                        Avenger& av1 = enemies[emap[a1]]; 
                        if(av1.getD() > 0 && av1.getH() <= 500 && av2.getD() > 0) {
                            s = av1.attack(av2);
                            battleLog.push_back(a1 + " attacks " + a2);
                        }
                    }
                    else if(hmap.find(a1) != hmap.end() && hmap.find(a2) != hmap.end()) {
                        Avenger& av2 = heroes[hmap[a2]];
                        Avenger& av1 = heroes[hmap[a1]]; 
                        if(av1.getD() > 0 && av1.getH() <= 500 && av2.getD() > 0) {
                            s = av1.attack(av2);
                            battleLog.push_back(a1 + " attacks " + a2);
                        }
                    }
                    else if(emap.find(a1) != emap.end() && emap.find(a2) != emap.end()){
                        Avenger& av2 = enemies[emap[a2]];
                        Avenger& av1 = enemies[emap[a1]]; 
                        if(av1.getD() > 0 && av1.getH() <= 500 && av2.getD() > 0) {
                            s = av1.attack(av2);
                            battleLog.push_back(a1 + " attacks " + a2);
                        }
                    }
                    if(s == 1)battleLog.push_back(a2 + " suit destroyed");   
                    else if(s == 2)battleLog.push_back(a2 + " suit overheated");     
                }
                else if(query == "Repair") {
                    string a;
                    int x;
                    cin >> a >> x;
                    if(hmap.find(a) != hmap.end()) {
                        Avenger& a1 = heroes[hmap[a]];
                        if(a1.getD() > 0){
                            a1.repair(x);
                            battleLog.push_back(a + " repaired");
                        }   
                    }
                    else if(emap.find(a) != emap.end()){
                        Avenger& a1 = enemies[emap[a]];
                        if(a1.getD() > 0){
                            a1.repair(x);
                            battleLog.push_back(a + " repaired");
                        }   
                    }
                }
                else if(query == "BoostPowerByFactor") {
                    string a;
                    int x;
                    cin >> a >> x;
                    battleLog.push_back(a + " boosted");
                    if(hmap.find(a) != hmap.end()) {
                        Avenger& a1 = heroes[hmap[a]];
                        if(a1.getD() <= 0) {
                            cin >> query;
                            continue;
                        }
                        a1.boostPower(x);
                        if(a1.getH() > 500) battleLog.push_back(a +" suit overheated");
                    }
                    else if(emap.find(a) != emap.end()){
                        Avenger& a1 = enemies[emap[a]];
                        if(a1.getD() <= 0) {
                            cin >> query;
                            continue;
                        }
                        a1.boostPower(x);
                        if(a1.getH() > 500) battleLog.push_back(a +" suit overheated");
                    }
                }
                else if(query == "BoostPower") {
                    string a;
                    int p,d,e,h;
                    cin >> a >> p >> d >> e >> h;
                    QNS temp(p,d,e,h);
                    if(hmap.find(a) != hmap.end()) {
                        Avenger& a1 = heroes[hmap[a]];
                        if(a1.getD() <= 0) {
                            cin >> query;
                            continue;
                        }
                        a1.boostPower(temp);
                        battleLog.push_back(a + " boosted");
                        if(a1.getH() > 500) battleLog.push_back(a +" suit overheated");
                    }
                    else if(emap.find(a) != emap.end()){
                        Avenger& a1 = enemies[emap[a]];

                        if(a1.getD() <= 0) {
                            cin >> query;
                            continue;
                        }
                        a1.boostPower(temp);
                        battleLog.push_back(a + " boosted");
                        if(a1.getH() > 500) battleLog.push_back(a +" suit overheated");
                    }
                }
                else if(query == "AvengerStatus") {
                    string a;
                    cin >> a;
                    if(hmap.find(a) != hmap.end()) {
                        Avenger& a1 = heroes[hmap[a]];
                        a1.printStatus();
                    }
                    else if(emap.find(a) != emap.end()){
                        Avenger& a1 = enemies[emap[a]];
                        a1.printStatus();
                    }
                }
                else if(query == "Upgrade") {
                    string a;
                    cin >> a;
                    if(armory.empty()) {
                        cin >> query;
                        battleLog.push_back(a + " upgrade Fail");
                        continue;
                    }
                    if(hmap.find(a) != hmap.end()) {
                        Avenger& a1 = heroes[hmap[a]];
                        if(a1.getD() <= 0) {
                            cin >> query;
                            continue;
                        }
                        a1.upgradeSuit();
                        battleLog.push_back(a + " upgraded");
                    }
                    else if(emap.find(a) != emap.end()){
                        Avenger& a1 = enemies[emap[a]];
                        if(a1.getD() <= 0) {
                            cin >> query;
                            continue;
                        }
                        a1.upgradeSuit();
                        battleLog.push_back(a + " upgraded");
                    }
                }
                else if(query == "PrintBattleLog") {
                    printBattleLog();
                }
                else if(query == "BattleStatus") {
                    int q = Result();
                    if(q == 1) cout << "heroes are winning";
                    else if(q == -1) cout << "enemies are winning";
                    else cout << "tie";
                    cout << endl;
                }
                cin >> query;
            }
        }
        void printBattleLog() {
            for(auto log : battleLog)cout << log << endl;
        }
        int Result() {
            int heroVal = 0;
            for(auto hero : heroes) {
                if(hero.getD() <= 0)continue;
                heroVal += hero.getSuitVal();
            }
            int enemyVal = 0;
            for(auto enemy : enemies) {
                if(enemy.getD() <= 0)continue;
                enemyVal += enemy.getSuitVal();
            }
            if(heroVal > enemyVal)return 1;
            else if(heroVal < enemyVal)return -1;
            else return 0;
        }

    private:

        //attributes
        vector<Avenger> heroes;
        vector<Avenger> enemies;
        map<string,int> hmap;
        map<string,int> emap;
        vector<string> battleLog;
};


int main(){
    Battle StarWars;
    string query;
    cin >> query;
    if(query == "BattleBegin") {
        StarWars.startBattle();
    }
}
