#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include "Btech.h"
#include "DualDegree.h"
#include "Faculty.h"
#include "RollNumber.h"
using namespace std;

class IITM{
    public:
        IITM(int n, int m, int h){
            string alphabets = "AAAAAAAAABCCDDDEEEEEEEEEFFFFGHHHIIIIIJKLLLMMNNNNNNNOOOOOOOPQRRRSSSSSTTTTTTUUVWWXYYZ    ";
            int size = alphabets.size();
            mt19937 rng(static_cast<unsigned long long>(time(nullptr)));
            uniform_int_distribution<> symbol(0,size-1);
            uniform_int_distribution<> boolval(0,1);
            uniform_int_distribution<> probab(0,100);
            uniform_real_distribution<> cg_gen(0,10);
            for(int i = 0 ; i < h ; i++) {
                string name = "";
                for(int x = 0 ; x < 20 ; x++) {
                    name += alphabets[symbol(rng)];
                }
                hostels.push_back(name);
            }
            for(int i = 0 ; i < m ; i++) {
                string name = "";
                for(int x = 0 ; x < 20 ; x++) {
                    name += alphabets[symbol(rng)];
                }
                bool perma = boolval(rng);
                Faculty temp(name,perma);
            }
        }
        void addBTechStudent(Btech* student) {
            BTechStudents.push_back(student);
        }
        void addDDStudent(DualDegree* student) {
            DDStudents.push_back(student);
        }
        void addHostel(string hostel) {
            hostels.push_back(hostel);
        }
        void addFaculty(Faculty* faculty){
            faculties.push_back(faculty);
        }
    private:
        vector<Btech*> BTechStudents;
        vector<DualDegree*> DDStudents;
        vector<string> hostels;
        vector<Faculty*> faculties;
};