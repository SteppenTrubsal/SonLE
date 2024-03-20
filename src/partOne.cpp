#include <iostream>
#include "someFunc.cpp"
using namespace std;

struct rootRange {
    double a = 0;
    double b = 0;
    double root = 0;
    int counter = 0;
};
vector<rootRange> findRootSpaces(string&, double, double, double);
void NewtonMethod(string&, rootRange&, double);

void subMain1(){
    cout << "Enter an equation: ";
    string func;
    getline(cin, func);
    eqRebuild(func);

    vector<rootRange> res = findRootSpaces(func);
    cout << res.size() << " root(s) was found:" << endl;

    for(int i = 0; i < res.size(); i++){
        NewtonMethod(func, res[i], 1e-4);
    }
    for(int i = 0; i < res.size(); i++){
        cout << i+1 << " root on [" << res[i].a << ", " << res[i].b << "]" << " is " << res[i].root << ", calculation taked " << res[i].counter << " iterations" << endl;
    }
}

vector<rootRange> findRootSpaces(string& funcStr, double a = -100, double b = 100, double eps = 0.5) {
    vector<rootRange> spaces;
    for (a; a < b; a += eps) {
        if (func(funcStr, a) * func(funcStr, a + eps) <= 0) {
            rootRange temp;
            temp.a = a;
            temp.b = a + eps;
            spaces.push_back(temp);
        }
    }
    return spaces;
}

void NewtonMethod(string& funcStr, rootRange& root, double eps = 1e-6) {
    double diff;
    double x0 = (root.a + root.b) / 2;
    do {
        double x1;
        x1 = x0 - func(funcStr, x0) / findDerivative(funcStr, x0);
        diff = abs(x1 - x0);
        x0 = x1;
        root.counter++;
    } while (diff > eps);
    root.root = x0;
}