#include <iostream>
#include <iomanip>
using namespace std;

class Complex {
    private:
        double real;
        double imaginary;
    public:
        Complex (double r , double i) {
            this->real = r;
            this->imaginary = i;
        }
        Complex operator+ (const Complex& rhs) {
            double r = this->real + rhs.real;
            double i = this->imaginary + rhs.imaginary;
            Complex c(r,i);
            return c;
        }
        Complex operator- (Complex& rhs) {
            return this->operator+(-rhs);
        }
        Complex operator- (){
            double r = -this->real;
            double i = -this->imaginary;
            Complex c(r,i);
            return c;
        }
        Complex operator* (const Complex& rhs) {
            double r = this->real * rhs.real - this->imaginary * rhs.imaginary;
            double i = this->real * rhs.imaginary + this->imaginary * rhs.real;
            Complex c(r,i);
            return c;
        }
        pair<double,double> val() {
            return {real,imaginary};
        }
};

long long int gcd(long long int a1, long long int a2) {
    if(a2 == 0)return a1;
    return gcd(a2,a1%a2);
}

class Rational {
    protected:
        long long int p;
        long long int q;
    public:
        Rational (long long int n, long long int d) {
            if(d < 0) {
                this->q = -d;
                this->p = -n;
            }
            else {
                this->p = n;
                this->q = d;
            }
        }
        Rational operator+(const Rational& rhs) {
            long long int p1;
            long long int q1;
            p1 = this->p * rhs.q + this->q * rhs.p;
            q1 = this->q*rhs.q;
            Rational r(p1,q1);
            r.reduce();
            return r;
        }
        Rational operator-(Rational& rhs) {
            return this->operator+(-rhs);
        }
        Rational operator- () {
            long long p1 = -this->p;
            Rational r(p1,this->q);
            return r;
        }
        Rational operator* (Rational& rhs) {
            long long int q1 = this->q * rhs.q;
            long long int p1 = this->p * rhs.p;
            Rational r(p1,q1);
            r.reduce();
            return r;
        }
        void reduce(){
            bool isNeg = false;
            if(this->p < 0) isNeg = true;
            this->p = max(this->p,-this->p);
            long long g = gcd(this->p,this->q);
            this->p /= g;
            this->q /= g;
            if(isNeg)this->p = -this->p;
        }
        double val() {
            return static_cast<double>(p)/q;
        }
        pair<long long int,long long int> valr() {
            return {p,q};
        }
};

long long int modExp(long long int base, long long int power, long long int mod) {
    if(power <= 0)return 1;
    long long int p = modExp(base,power/2,mod) % mod;
    p = (p*p)%mod;
    if(power % 2 == 1) {
        return (p*base)%mod;
    }
    return p;
}

class Natural : public Rational {
    private:
        const int MOD = 1000000007;
    public:
        Natural(int n): Rational(n,1){}
        bool isPrime(){
            for(int i = 2 ; i * i <= this->p ; i++) {
                if(this->p % i == 0)return false;
            }
            return true;
        }
        long long int invModulo(){
            return modExp(this->p,MOD-2,MOD);
        }
};

double add(Rational& r1, Rational& r2) {
    return r1.val() + r2.val();
}
double sub(Rational& r1, Rational& r2) {
    return r1.val() - r2.val();
}
double mul(Rational& r1, Rational& r2) {
    return r1.val() * r2.val();
}
int main() {
    int n;
    cin >> n;
    cout << setiosflags(ios::fixed);
    while(n--) {
        string query;
        cin >> query;
        if(query == "complex") {
            string q1;
            cin >> q1;
            double r,i;
            cin >> r >> i;
            Complex c1(r,i);
            cin >> r >> i;
            Complex c2(r,i);
            pair<double,double> v;
            if(q1 == "add") {
                Complex c3 = c1 + c2;
                v = c3.val();
            }
            else if(q1 == "sub") {
                Complex c3 = c1 - c2;
                v = c3.val();
            }
            else if(q1 == "mult") {
                Complex c3 = c1 * c2;
                v = c3.val();
            }
            cout << setprecision(3) << v.first << " " << v.second << endl;
        }
        else if(query == "rational") {
            string q1;
            cin >> q1;
            long long int p,q;
            cin >> p >> q;
            Rational r1(p,q);
            if(q1 == "reduce") {
                r1.reduce();
                pair<long long,long long> p = r1.valr();
                cout << p.first << " " << p.second << endl;
            }
            else {
                cin >> p >> q;
                Rational r2(p,q);
                if(q1 == "add") {
                    double r3 = add(r1,r2);
                    cout << setprecision(3) << r3 << endl;
                }
                else if(q1 == "sub") {
                    double r3 = sub(r1,r2);
                    cout << setprecision(3) << r3<< endl;
                }
                else if(q1 == "mult") {
                    double r3 = mul(r1,r2);
                    cout << setprecision(3) << r3<< endl;
                }
            }
        }
        else if(query == "natural") {
            string q1;
            cin >> q1;
            long long int a;
            cin >> a;
            Natural num(a);
            if(q1 == "isprime") {
                cout << num.isPrime()<< endl;
            }
            else if(q1 == "inverse") {
                cout << num.invModulo()<< endl;
            }
        }
    }
}