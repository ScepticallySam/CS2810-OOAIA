#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
int MoMSelect(vector<int> &v) {
    if(v.size() == 1)return v[0];
    if(v.size() <= 5) {
        for(int x = 0 ; x < v.size() ; x++) {
            int temp = v[x];
            for(int y = 0 ; y < x ; y++) {
                if(v[y] > temp) {
                    for(int z = x ; z >= y ; z--) {
                        v[z] = v[z-1];
                    }
                    v[y] = temp;
                    break; 
                }
            }
        }
        return v[v.size()/2];
    }
    vector<int> t;
    vector<int> m;
    for(int x = 0 ; x < v.size() ; x++) {
        if(x%5 == 4){
            m.push_back(MoMSelect(t));
            t.clear();
        } 
        t.push_back(v[x]);
    }
    if(t.size() != 0)m.push_back(MoMSelect(t));
    return MoMSelect(m);
}

void QuickSort(vector<int>& v, int l, int r) {
    if(r < l)return;
    if(r - l + 1 <= 2) {
        if(v[l] > v[r]) {
            v[l] = v[l] + v[r];
            v[r] = v[l] - v[r];
            v[l] = v[l] - v[r];
        }
        return;
    }
    vector<int> v1;
    for(int x = l ; x <= r ; x++)v1.push_back(v[x]);
    int p = MoMSelect(v1);
    int pidx;
    int count = 0;
    for(int x = l ; x <= r ; x++) {
        if(v[x] == p) pidx = x;
        else if(v[x] < p)count++;
    }
    if(l + count != pidx){
        v[l + count] = v[l + count] + v[pidx];
        v[pidx] = v[l + count] - v[pidx];
        v[l + count] = v[l + count] - v[pidx];
    }
    int lptr = l;
    int rptr = r;
    while(lptr < l + count && rptr > l + count) {
        while(lptr < l + count && v[lptr] < p)lptr++;
        while(rptr > l + count && v[rptr] > p)rptr--;
        if(lptr != rptr) {
            v[lptr] = v[lptr] + v[rptr];
            v[rptr] = v[lptr] - v[rptr];
            v[lptr] = v[lptr] - v[rptr];
        }
    }
    QuickSort(v,l,l+count-1);
    QuickSort(v,l+count+1,r);
}

class CurrencySorter {
public:
    void operator () (vector<int>& nums){ //sorts nums using quicksort
        QuickSort(nums,0,nums.size()-1);
    }
};

class FibonacciGenerator {
public:
    const int mod = 1e9 + 7;
    long long operator() (long long n){ //returns fib(n)
        vector<long long> mat = {1,1,1,0};
        mat = binExp(mat,n);
        return mat[1];
    }
    vector<long long> binExp(vector<long long> mat, long long n) {
        if(n == 1)return mat;
        vector<long long> v = binExp(mat,n/2);
        vector<long long> p = {0,0,0,0};
        p[0] = ((v[0] * v[0])%mod + (v[1] * v[2])%mod)%mod;
        p[1] = ((v[0] * v[1])%mod + (v[1] * v[3])%mod)%mod;
        p[2] = ((v[2] * v[0])%mod + (v[3] * v[2])%mod)%mod; 
        p[3] = ((v[2] * v[1])%mod + (v[3] * v[3])%mod)%mod; 

        if(n%2 == 0) {
            return p;
        }
        else {
            vector<long long> q = {0,0,0,0};
            q[0] = ((p[0] * mat[0])%mod + (p[1] * mat[2])%mod)%mod;
            q[1] = ((p[0] * mat[1])%mod + (p[1] * mat[3])%mod)%mod;
            q[2] = ((p[2] * mat[0])%mod + (p[3] * mat[2])%mod)%mod; 
            q[3] = ((p[2] * mat[1])%mod + (p[3] * mat[3])%mod)%mod;
            return q;
        }
    }
};

class PrimeCalculator {
public:
    static void EratosthenesSieve(int n) {
        primes.resize(n+1,true);
        primes[0] = false;
        primes[1] = false;
        for(int p = 0 ; p*p <= n ; p++) {
            if(primes[p]) {
                for(int x = p*p ; x <= n ; x += p) {
                    primes[x] = false;
                }
            }
        }
        for(int x = 2 ; x < primes.size() ; x++) {
            if(primes[x])primes_vector.push_back(x);
        }
    }
    static void printPrimes(long long int L, long long int R){ //prints all primes from L to R
        vecPrimes(L,R);
    }
    static long long int printPrimeSum(long long int L, long long int R){ //returns sum of all primes from L to R
        return vecPrimes(L,R,"Sum");
    }
    static long long int vecPrimes (long long int L, long long int R, string param = "notSum"){ //returns a vector of all primes between L and R
        const int mod = 1e9 + 7;
        long long sum = 0;
        vector<bool> v(R-L+1,true);
        if(L == 0){
            v[0] = false;
            if(R >= 1)v[1] = false;
        }
        if(L == 1) v[0] = false;
        for(auto prime : primes_vector){
            long long int start = (L+(prime)-1)/prime * prime;
            for(long long x = max(prime*prime,start) ; x <= R ; x+=prime) {
                v[x - L] = false;
            }
        }
        for(long long x = L ; x <= R ; x++) {
            if(v[x - L]) {
                if(param == "Sum") sum += x;
                else cout << x  << " " ;
            }
        } 
        if(param != "Sum") cout << endl;
        return sum;
    }
private:
    static vector<bool> primes;
    static vector<long long> primes_vector;
};

vector<bool> PrimeCalculator::primes;
vector<long long> PrimeCalculator::primes_vector;

class NumberAnalyzer {
public:
    static bool isSquareFree(int X){ //returns true if X is not divisible by a square number
        for(int x = 2 ; x*x <= X ; x++) {
            if(X%x == 0 && (X/x)%x == 0)return false;
            if(X%x == 0) X/=x;
        }
        return true;
    }
    static int countDivisors(int X){ //returns the number of divisors
        int count = 0;
        for(int x = 1 ; x*x <= X ; x++){
            if(X%x == 0){
                count++;
                if(X/x != x)count++;
            }
        }
        return count;
    }
    static long long int sumOfDivisors(int X){ //returns sum of divisors
        long long int acc = 0;
        for(int x = 1 ; x*x <= X ; x++){
            if(X%x == 0){
                acc += x;
                if(X/x != x)acc += X/x;
            }
        }
        return acc;
    }
};


int main(){
    PrimeCalculator::EratosthenesSieve(1000000);
    int mode;
    cin >> mode;
    int t;
    cin >> t;
    if(mode == 1) {
        vector<vector<int>> v;
        while(t--) {
            vector<int> temp;
            int n;
            cin >> n;
            while(n--) {
                int t;
                cin >> t;
                temp.push_back(t);
            } 
            v.push_back(temp);
        }
        for_each(v.begin(),v.end(),CurrencySorter());
        for(auto elem : v) {
            for(auto num : elem) {
                cout << num << " ";
            }
            cout << endl;
        }
    }
    else if(mode == 2) {
        vector<long long> v;
        while(t--) {
            long long i;
            cin >> i;
            v.push_back(i);
        }
        transform(v.begin(),v.end(),v.begin(),FibonacciGenerator());
        for(auto elem : v)cout << elem <<endl;
    }
    else if(mode == 3) {
        while(t--){
            string query;
            long long int L,R;
            cin >> query >> L >> R;
            if(query == "printPrimes") PrimeCalculator::printPrimes(L,R);
            else if(query == "printPrimeSum") cout << PrimeCalculator::printPrimeSum(L,R) << endl;
        }  
    }
    else if(mode == 4) {
        while(t--) {
            string query;
            int X;
            cin >> query >> X;
            if(query == "isSquareFree") {
                if(NumberAnalyzer::isSquareFree(X))cout << "yes";
                else cout << "no";
            }
            else if(query == "countDivisors") {
                cout << NumberAnalyzer::countDivisors(X);
            }
            else if(query == "sumOfDivisors") {
                cout << NumberAnalyzer::sumOfDivisors(X);
            }
            cout << endl;
        }
    }
}