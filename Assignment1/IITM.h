#pragma once
#include <iostream>
#include <vector>
#include "Btech.h"
#include "DualDegree.h"
#include "Faculty.h"
#include "RollNumber.h"
using namespace std;

class IITM{
    public:
        IITM(int n, int m, int h){
            for(int i = 0 ; i < h ; i++) {
                hostels.push_back("hostel"+to_string(i+1));
            }
            for(int i = 0 ; i < m ; i++) {

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