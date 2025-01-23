#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include "Btech.h"
#include "DualDegree.h"
#include "Faculty.h"
#include "RollNumber.h"
using namespace std;

class IITM{
    public:
        IITM(int n, int m, int h){
            string alphabets = "AAAAAAAAABCCDDDEEEEEEEEEFFFFGHHHIIIIIJKLLLMMNNNNNNNOOOOOOOPQRRRSSSSSTTTTTTUUVWWXYYZ    ";
            string nums = "0123456789";
            int size = alphabets.size();
            int size1 = nums.size();
            mt19937 rng(static_cast<unsigned long long>(time(nullptr)));
            uniform_int_distribution<> symbol(0,size-1);
            uniform_int_distribution<> num_idx(0,size1-1);
            uniform_int_distribution<> boolval(0,1);
            uniform_int_distribution<> probab(0,100);
            uniform_real_distribution<> cg_gen(0,10);
            uniform_int_distribution<> stringsize(15,25);
            uniform_int_distribution<> facaulty_pick(0,m-1);
            uniform_int_distribution<> hostel_pick(0,h-1);
            for(int i = 0 ; i < h ; i++) {
                string name = "";
                int rand_len = stringsize(rng);
                for(int x = 0 ; x < rand_len ; x++) {
                    name += alphabets[symbol(rng)];
                }
                hostels.push_back(name);
            }
            int perma_faculty_count = 0;
            for(int i = 0 ; i < m ; i++) {
                string name = "";
                int rand_len = stringsize(rng);
                for(int x = 0 ; x < rand_len ; x++) {
                    name += alphabets[symbol(rng)];
                }
                bool perma = boolval(rng);
                if(perma == 1)perma_faculty_count++;
                Faculty* temp = new Faculty(name,perma);
                faculties.push_back(temp);
            }
            for(int i = 0 ; i < n ; i++) {
                int random_num = probab(rng);
                Faculty* facad = faculties[facaulty_pick(rng)];
                string hostel = hostels[hostel_pick(rng)];
                float cg = cg_gen(rng);
                string roll = "";
                for(int x = 0 ; x < 5 ; x++) {
                    roll += nums[num_idx(rng)];
                }
                if(random_num <= 17) {
                    roll = 'D' + roll;
                    DualDegree* dds = new DualDegree(roll,hostel,facad,cg);
                    DDStudents.push_back(dds);
                }
                else {
                    roll = 'B' + roll;
                    Btech* bts = new Btech(roll,hostel,facad,cg);
                    BTechStudents.push_back(bts);
                }
            }

            vector<Btech*> copy = BTechStudents;
            sort(copy.begin(),copy.end(),[](Btech* b1, Btech* b2){
                return b1->getCG() > b1->getCG();
            });
            for(int x = 0 ; x < 2*perma_faculty_count;) {
                Faculty* fac = faculties[facaulty_pick(rng)];
                if(fac->addBTPAdvisee(copy[x]))x++;
            }

            vector<DualDegree*> copy1 = DDStudents;
            sort(copy1.begin(),copy1.end(),[](DualDegree* d1, DualDegree* d2){
                return d1->getHostel() < d2->getHostel();
            });
            for(int x = 0 ; x < 2*perma_faculty_count;) {
                Faculty* fac = faculties[facaulty_pick(rng)];
                if(fac->addDDAdvisee(copy1[x]))x++;
            }

            int assignedTAs = 0;
            for(int x = 0 ; x < DDStudents.size() ;) {
                Faculty* fac = faculties[facaulty_pick(rng)];
                if(fac->addTA(DDStudents[x])){
                    x--;
                    assignedTAs++;
                }
                if(assignedTAs == 5*faculties.size())break;
            }
        }

        bool constraint_check() {
            //not sure what exactly this has to do
            return true;
        }

        double same_mentors_percent() {
            int count = 0;
            for(auto elem : DDStudents) {
                if(elem->getDDPGuide() != nullptr && elem->getTASupervisor() != nullptr && elem->getDDPGuide()->get_name() == elem->getTASupervisor()->get_name())count++;
            }
            return static_cast<double>(count)/DDStudents.size();
        }

        void print_students(string mode) {
            vector<Btech*> copy = BTechStudents;
            vector<DualDegree*> copy1 = DDStudents;
            vector<Faculty*> copy2 = faculties;
            if(mode == "Roll") {
                sort(copy1.begin(),copy1.end(),[](DualDegree* d1, DualDegree* d2){
                    return d1->get_roll() < d2->get_roll();
                });
                sort(copy.begin(),copy.end(),[](Btech* b1, Btech* b2){
                    return b1->get_roll() < b2->get_roll();
                });
                cout << "Roll Number Sorting: \n";
                cout << "B.Tech. Students:\n";
                for(auto elem : copy)cout << elem->get_roll() << endl;
                cout << "Dual Degree Students:\n";
                for(auto elem : copy1) cout << elem->get_roll() << endl;
            }
            else if(mode == "Guide") {
                sort(copy2.begin(),copy2.end(),[](Faculty* f1,Faculty* f2){
                    return f1->get_name() < f2->get_name();
                });
                for(auto elem : copy2) {
                    if(elem->get_status() == "Permanent")elem->print_Advisees();
                }
            }
            else if(mode == "Hostel") {
                sort(copy1.begin(),copy1.end(),[](DualDegree* d1, DualDegree* d2){
                    return d1->getHostel() < d2->getHostel();
                });
                sort(copy.begin(),copy.end(),[](Btech* b1, Btech* b2){
                    return b1->getHostel() < b2->getHostel();
                });
                cout << "Hostel Sorting: \n";
                cout << "B.Tech. Students:\n";
                for(auto elem : copy)cout << elem->get_roll() << endl;
                cout << "Dual Degree Students:\n";
                for(auto elem : copy1) cout << elem->get_roll() << endl;
            }
            else cout << "Invalid Sorting Type" << endl;
        }

        void print_guideless() {
            cout << "Students without Guides:\n";
            for(auto elem : BTechStudents){
                if(elem->getBTPGuide() == nullptr) cout << elem->get_roll() << endl;
            }
            for(auto elem : DDStudents) {
                if(elem->getDDPGuide() == nullptr) cout << elem->get_roll() << endl;
            }
        }

        void get_particular_students(string facad, string hostel) {
            for(auto elem : DDStudents) {
                if(elem->getFacad()->get_name() == facad && elem->getHostel() == hostel)cout << elem->get_roll() << endl;
            }
        }
        
    private:
        vector<Btech*> BTechStudents;
        vector<DualDegree*> DDStudents;
        vector<string> hostels;
        vector<Faculty*> faculties;
};