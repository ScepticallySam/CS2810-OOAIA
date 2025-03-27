#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <algorithm>
using namespace std;

class Complex {
friend ostream& operator<< (ostream& stream, Complex& comp);
public:
    //constructors
    Complex (long double real, long double imaginary = 0);
    Complex ();

    //operator overladings
    Complex operator= (const Complex& rhs);
    Complex operator* (const Complex& rhs);
    Complex operator* (const long long int rhs);
    Complex operator+ (const Complex& rhs);
    Complex operator- (const Complex& rhs);
    Complex operator/ (const long double val);
    bool operator== (const Complex& rhs) {
        return (this->real == rhs.real) && (this->imaginary == rhs.imaginary);
    }

    //other functionalities
    Complex complement();
    long double mod_square(); 
    Complex invert();
    long double get_real() {return real;}
    void print();

protected:
    long double real;
    long double imaginary;
};

class UnitComplex : public Complex {
public:
    UnitComplex(int n); // complex numbers of the form e^((2*pi*i)/k)
};

template <class T> class Evaluation;
template <class T> class Multiplication;
template <class T> class Differentiation;

template <class T>
class Polynomial {
friend class Evaluation<T>;
friend class Multiplication<T>;
friend class Differentiation<T>;
public:
    //constructor
    Polynomial();
    Polynomial(vector<T> &poly);

    //functionalities
    Polynomial<T> operator* (Polynomial<T>& rhs);
    Polynomial<T> differentiate();
    T evaluate (long long int x); //the polynomial is to be evaluated at a particular integer value of x
    void print();
    vector<T> get_vec() {return coeff_vector;}

private:
    vector<T> coeff_vector;
};

template <class T>
class Multiplication {
public:
    Multiplication();
    
    vector<Complex> FFT_2n(vector<Complex>& v1); //evaluation
    vector<Complex> FFT_n(vector<Complex>& v1); //interpolation
    //if this gives floating point precision errors, I'll use Karatsuba
    vector<T> Karatsuba(vector<T>& p1, vector<T>& p2) {
        if(p1.size() == 1) {
            return {p1[0]*p2[0],0};
        }
        vector<T> a,b,c,d,e,f;
        for(int x = 0 ; x < p1.size() ; x++) {
            if(x < p1.size()/2)a.push_back(p1[x]);
            else b.push_back(p1[x]);
        }
        for(int x = 0 ; x < p2.size() ; x++) {
            if(x < p2.size()/2)c.push_back(p2[x]);
            else d.push_back(p2[x]);
        }
        for(int x = 0 ; x < a.size() ; x++) {
            e.push_back(a[x] + b[x]);
            f.push_back(d[x] + c[x]);
        }
        vector<T> a1 = Karatsuba(a,c);
        vector<T> a2 = Karatsuba(b,d);
        vector<T> a3 = Karatsuba(e,f);
        vector<T> res = a1;
        for(auto elem : a2) res.push_back(elem);
        for(int x = p1.size()/2 ; x < 3*p1.size()/2 ; x++) {
            res[x] = res[x] + a3[x - p1.size()/2] - a1[x - p1.size()/2] - a2[x - p1.size()/2];
        }
        return res;
    }
    Polynomial<T> multiply(Polynomial<T>& p1, Polynomial<T>& p2);
};

template <class T>
class Evaluation {
public:
    Evaluation();

    T evaluate(Polynomial<T>& poly, long long x); //using Horner's method
    //has to be evaluated separately for strings
};

template <class T>
class Differentiation {
public:
    Differentiation();

    Polynomial<T> differentiate(Polynomial<T>& poly);
};

ostream& operator<< (ostream& stream, Complex& comp) {
    cout << fixed << setprecision(0) << comp.real << " " << comp.imaginary;
    return stream;
}

//Complex Class
Complex::Complex (long double real, long double imaginary) {
    this->real = real;
    this->imaginary = imaginary;
}
Complex::Complex () {real = 0;imaginary = 0;}
Complex Complex::operator= (const Complex& rhs) {
    this->real = rhs.real;
    this->imaginary = rhs.imaginary;
    return *this;
}
Complex Complex::operator* (const Complex& rhs) {
    return Complex((this->real * rhs.real) - (this->imaginary * rhs.imaginary), (this->real * rhs.imaginary) + (this->imaginary * rhs.real));
}
Complex Complex::operator* (const long long int rhs) {
    return Complex((this->real * rhs),(this->imaginary * rhs));
}
Complex Complex::operator+ (const Complex& rhs) {
    return Complex(this->real + rhs.real, this->imaginary + rhs.imaginary);
}
Complex Complex::operator- (const Complex& rhs) {
    return Complex(this->real - rhs.real, this->imaginary - rhs.imaginary);
}
Complex Complex::operator/ (const long double val) {
    return Complex(this->real/val, this->imaginary/val);
}

Complex Complex::complement() {
    return Complex(this->real, -(this->imaginary));
}
long double Complex::mod_square() {
    return (this->real*this->real) + (this->imaginary*this->imaginary);
}
Complex Complex::invert() {
    long double val = mod_square();
    return complement()/val;
}
void Complex::print() {
    cout << this->real << " " << this->imaginary << "i" << endl;
}

//UnitComplex
UnitComplex::UnitComplex(int n) {
    this->real = cos(2*M_PI/n);
    this->imaginary = sin(2*M_PI/n); 
}

//Polynomial class
template <class T>
Polynomial<T>::Polynomial() {}
template <class T>
Polynomial<T>::Polynomial(vector<T> &poly) {
    this->coeff_vector = poly;
}
template <class T>
Polynomial<T> Polynomial<T>::operator* (Polynomial<T>& rhs) {
    Multiplication<T> multiplier;
    return multiplier.multiply(*this,rhs);
}
template <class T>
Polynomial<T> Polynomial<T>::differentiate () {
    Differentiation<T> differentiator;
    return differentiator.differentiate(*this);
}
template <class T>
T Polynomial<T>::evaluate (long long int x) {
    Evaluation<T> evaluator;
    return evaluator.evaluate(*this, x);
}
template <class T>
void Polynomial<T>::print () {
    cout << setiosflags(ios::fixed);
    for(auto elem : coeff_vector) cout << setprecision(6) << elem << " ";
    cout << endl;
}

//Evaluation
template <class T>
Evaluation<T>::Evaluation() {}
template <class T>
T Evaluation<T>::evaluate(Polynomial<T>& poly, long long x) {
    T val {};
    // for(auto elem : poly.coeff_vector)cout << elem << " " ; cout << endl;
    // if(x == 0) return poly.coeff_vector[0];
    long long int mul = 1;
    for(int i = poly.coeff_vector.size() - 1 ; i >= 1 ; i--) {
        mul *= x; 
    }
    for(int i = poly.coeff_vector.size() - 1 ; i >= 0; i--) {
        val += (poly.coeff_vector[i] * mul);
        mul /= x;
    }
    return val;
}

//Differentiation
template <class T>
Differentiation<T>::Differentiation() {}
template <class T>
Polynomial<T> Differentiation<T>::differentiate(Polynomial<T>& poly) {
    vector<T> v1;
    for(int i = 1 ; i < poly.coeff_vector.size() ; i++) {
        v1.push_back(i*poly.coeff_vector[i]);
    }
    return Polynomial<T>(v1);
}

//Multiplication
template <class T>
Multiplication<T>::Multiplication() {}
template <class T>
vector<Complex> Multiplication<T>::FFT_2n(vector<Complex>& a) {
    vector<Complex> y(2*a.size(),0);
    int n = a.size();
    if(n == 1) return {a[0],a[0]};
    if(n == 0)return {};
    UnitComplex wn(2*n);
    Complex w(1);
    vector<Complex> aodd;
    vector<Complex> aeven;
    for(int x = 0 ; x < n ; x++) {
        if(x % 2 == 0) aeven.push_back(a[x]);
        else aodd.push_back(a[x]);
    }
    vector<Complex> yeven = FFT_2n(aeven);
    vector<Complex> yodd = FFT_2n(aodd);
    for(int k = 0 ; k < n ; k++) {
        y[k] = yeven[k] + w*yodd[k];
        y[k + n] = yeven[k] - w*yodd[k];
        w = w * wn;
    }
    return y;
}
template <class T>
vector<Complex> Multiplication<T>::FFT_n(vector<Complex>& a) {
    vector<Complex> y(a.size(),0);
    int n = a.size();
    if(n == 1) return a;
    if(n == 0)return {};
    UnitComplex wn(-n);
    Complex w(1);
    vector<Complex> aodd;
    vector<Complex> aeven;
    for(int x = 0 ; x < n ; x++) {
        if(x % 2 == 0) aeven.push_back(a[x]);
        else aodd.push_back(a[x]);
    }
    vector<Complex> yeven = FFT_n(aeven);
    vector<Complex> yodd = FFT_n(aodd);
    for(int k = 0 ; k < n/2 ; k++) {
        y[k] = yeven[k] + w*yodd[k];
        y[k + n/2] = yeven[k] - w*yodd[k];
        w = w * wn;
    }
    return y;
}
template <class T>
Polynomial<T> Multiplication<T>::multiply(Polynomial<T>& p1, Polynomial<T>& p2) {
    vector<T> b1 = p1.coeff_vector;
    vector<T> b2 = p2.coeff_vector;
    int deg = b1.size() + b2.size() - 1;
    // for(auto elem : b1) {
    //     Complex temp(elem);
    //     c1.push_back(temp);
    // }
    // for(auto elem : b2) {
    //     Complex temp(elem);
    //     c2.push_back(temp);
    // }
    int s1 = b1.size();
    int s2 = b2.size();
    int m = max(s1,s2);
    int padlength = pow(2,ceil(log2(m)));
    b1.resize(padlength);
    b2.resize(padlength);
    // vector<Complex> a1 = FFT_2n(c1);
    // vector<Complex> a2 = FFT_2n(c2);
    // vector<Complex> a3;
    // for(int x = 0 ; x < a1.size() ; x++) {
    //     a3.push_back(a1[x] * a2[x]);
    // }
    // // for(auto elem : a3)cout << elem << endl;
    // vector<Complex> a4 = FFT_n(a3);
    // vector<T> a5;
    // for(int i = 0 ; i <= deg ; i++) {
    //     Complex elem = a4[i];
    //     a5.push_back(static_cast<T>(elem.get_real()) / a4.size());
    // }
    // return Polynomial<T>(a5);
    vector<T> res = Karatsuba(b1,b2);
    res.resize(deg);
    Polynomial<T> resp(res);
    return resp;
}

string operator* (string a, long long int b) {
    string res = "";
    for(int x = 0 ; x < b ; x++) {
        res += a;
    }
    return res;
}

int main(){
    // vector<long long int> p = {1,2};
    // vector<long long int> f = {3,4};
    // Polynomial<long long int> p1(p);
    // Polynomial<long long int> p2(f);
    // Multiplication<long long int> x;
    // Polynomial<long long int> a = x.multiply(p1,p2);
    // a.print();
    // vector<string> a1 = {"acs","-4","B"};
    // Polynomial<string> l(a1);
    // l.print();
    // Evaluation<string> c;
    // cout << c.evaluate(l,3) << endl;
    // // Polynomial<long long int> p2(f);
    // // Multiplication<long long int> p3;
    // // Polynomial<long long int> p4 = p3.multiply(p1,p2);
    // // p4.print();
    // // vector<long double> a = {1.0,1.0,1.0};
    // // vector<long double> b = {1.0,1.0,1.0};
    // // Polynomial<long double> a1(a);
    // // Polynomial<long double> a2(b);
    // // Multiplication<long double> l;
    // // Polynomial<long double> a3 = l.multiply(a1,a2);
    // // a3.print();
    int queries;
    cin >> queries;
    while(queries--){
        int query;
        cin >> query;
        string type;
        cin >> type;
        int degree;
        cin >> degree;
        if(type == "complex") {
            vector<Complex> vec;
            for(int x = 0 ; x < degree ; x++) {
                long double r,i;
                cin >> r >> i;
                Complex c(r,i);
                vec.push_back(c);
            }
            cin >> degree;
            vector<Complex> vec1;
            for(int x = 0 ; x < degree ; x++) {
                long double r,i;
                cin >> r >> i;
                Complex c(r,i);
                vec1.push_back(c);
            }
            Polynomial<Complex> a1(vec);
            Polynomial<Complex> a2(vec1);
            Multiplication<Complex> m;
            Polynomial<Complex> a3 = m.multiply(a1,a2);
            a3.print();
        }
        else if(type == "integer") {
            vector<long long int> vec;
            for(int x = 0 ; x < degree ; x++) {
                long long int num;
                cin >> num;
                vec.push_back(num);
            }
            Polynomial<long long int> p1(vec);
            if(query == 1) {
                vector<long long int> vec1;
                cin >> degree;
                for(int x = 0 ; x < degree ; x++) {
                    long long int num;
                    cin >> num;
                    vec1.push_back(num);
                }
                Polynomial<long long int> p2(vec1);
                Multiplication<long long int> m;
                Polynomial<long long int> p3 = m.multiply(p1,p2);
                vector<long long int> v1 = p3.get_vec();
                for(auto elem : v1) cout << fixed << setprecision(0) << elem << " ";
                cout << endl;
            }
            else if(query == 2) {
                Evaluation<long long int> e;
                long long int x;
                cin >> x;
                long long int a = e.evaluate(p1,x);
                cout << a << endl;
            }
            else if(query == 3) {
                Differentiation<long long int> d;
                Polynomial<long long int> p = d.differentiate(p1);
                vector<long long int> v1 = p.get_vec();
                for(auto elem : v1) cout << fixed << setprecision(0) << floor (elem) << " ";
                cout << endl;
            }
        }
        else if(type == "float") {
            vector<long double> vec;
            for(int x = 0 ; x < degree ; x++) {
                long double num;
                cin >> num;
                vec.push_back(num);
            }
            Polynomial<long double> p1(vec);
            if(query == 1) {
                vector<long double> vec1;
                cin >> degree;
                for(int x = 0 ; x < degree ; x++) {
                    long double num;
                    cin >> num;
                    vec1.push_back(num);
                }
                Polynomial<long double> p2(vec1);
                Multiplication<long double> m;
                Polynomial<long double> p3 = m.multiply(p1,p2);
                p3.print();
            }
            else if(query == 2) {
                Evaluation<long double> e;
                long long int x;
                cin >> x;
                long double a = e.evaluate(p1,x);
                cout << fixed << setprecision(6) << a << endl;
            }
            else if(query == 3) {
                Differentiation<long double> d;
                Polynomial<long double> p = d.differentiate(p1);
                p.print();
            }
        }
        else if(type == "string") {
            vector<string> s1;
            for(int x = 0 ; x < degree ; x++) {
                string temp;
                cin >> temp;
                s1.push_back(temp);
            }
            Polynomial<string> s2(s1);
            Evaluation<string> e;
            long long int k;
            cin >> k;
            string res = e.evaluate(s2,k);
            cout << res << endl;
        }
        else cout << type <<"wa?" << endl;
    }
}