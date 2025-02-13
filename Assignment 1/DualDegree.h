#pragma once
#include <iostream>
#include <vector>
//#include "Faculty.h"
#include "RollNumber.h"
using namespace std;

class Faculty;
class DualDegree {
    public:
        DualDegree(RollNumber r, string hostel, Faculty* facad, float cgpa): hostel(hostel), cgpa(cgpa){
            facad = facad;
            roll = r;
        }
        DualDegree(){}
        string getHostel() {
            return hostel;
        }
        Faculty* getFacad() {
            return facad;
        }
        void changeHostel(string new_hostel) {
            hostel = new_hostel;
        }
        Faculty* getDDPGuide() {
            return DDPGuide;
        }
        void setDDPGuide(Faculty* fac) {
            DDPGuide = fac;
        }
        Faculty* getTASupervisor() {
            return TASupervisor;
        }
        void setTASupervisor(Faculty* fac) {
            TASupervisor = fac;
        }
        string get_roll() {
            return roll.get_val();
        }
    private:
        RollNumber roll;
        string hostel;
        Faculty* facad;
        float cgpa;
        Faculty* DDPGuide = nullptr;
        Faculty* TASupervisor = nullptr;
};