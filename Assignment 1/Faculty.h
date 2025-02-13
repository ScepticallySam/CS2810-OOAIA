#pragma once
#include <iostream>
#include <vector>
#include "Btech.h"
#include "DualDegree.h"
#include "RollNumber.h"
using namespace std;
class DualDegree;
class Btech;
class Faculty {
    public:
        Faculty(string name, bool perma_status): name(name), isPermanent(perma_status) {}
        Faculty(){}
        string get_name() {
            return name;
        }
        string get_status() {
            if(isPermanent) return "Permanent";
            else return "Adjunct";
        }
        bool addBTPAdvisee(Btech* student) {
            if(BTPAdvisees.size() == 2 || !isPermanent)return false;
            BTPAdvisees.push_back(student);
            student->setBTPGuide(this);
            return true;
        }
        bool addDDAdvisee(DualDegree* student) {
            if(DDAdvisees.size() == 2 || !isPermanent)return false;
            DDAdvisees.push_back(student);
            student->setDDPGuide(this);
            return true;
        }
        bool addTA(DualDegree* student) {
            if(TAs.size() == 5)return false;
            TAs.push_back(student);
            student->setTASupervisor(this);
            return true;
        }
        void print_Advisees () {
            cout << name << "'s Advisees:\n";
            for(auto elem : BTPAdvisees)cout << elem->get_roll() << endl;
            for(auto elem : DDAdvisees)cout << elem->get_roll() << endl;
        }
    private:
        string name;
        bool isPermanent;
        vector<Btech*> BTPAdvisees;
        vector<DualDegree*> DDAdvisees;
        vector<DualDegree*> TAs;
};