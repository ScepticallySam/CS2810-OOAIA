#pragma once
#include <iostream>
#include <vector>
using namespace std;

class RollNumber {
    public:
        RollNumber(string roll):roll(roll) {}
        RollNumber() {}
        string getRollNumber() {
            return roll;
        }
        string get_val() {
            return roll;
        }
    private:
        string roll;
};
