#pragma once
#include <iostream>
#include <vector>
//#include "Faculty.h"
#include "RollNumber.h"
using namespace std;

class Faculty;
class Btech {
    public:
        Btech(RollNumber r, string hostel, Faculty* facad, float cgpa): hostel(hostel), cgpa(cgpa){
            facad = facad;
            roll = r;
        }
        Btech(){}
        string getHostel() {
            return hostel;
        }
        float getCG() {
            return cgpa;
        }
        Faculty* getFacad() {
            return facad;
        }
        void changeHostel(string new_hostel) {
            hostel = new_hostel;
        }
        Faculty* getBTPGuide() {
            return BTPGuide;
        }
        void setBTPGuide(Faculty* fac) {
            BTPGuide = fac;
        }
        string get_roll() {
            return roll.get_val();
        }
    private:
        RollNumber roll;
        string hostel;
        Faculty* facad;
        float cgpa;
        Faculty* BTPGuide = nullptr;
};