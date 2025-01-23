#include <iostream>
#include <vector>
#include "Btech.h"
#include "DualDegree.h"
#include "Faculty.h"
#include "RollNumber.h"
#include "IITM.h"
using namespace std;

int main() {
    int n,m,h;
    cout << "Enter the number of students: ";
    cin >> n;
    cout << "Enter the number of Faculty members: ";
    cin >> m;
    cout << "Enter the number of Hostels: ";
    cin >> h;
    IITM iitm(n,m,h);
}