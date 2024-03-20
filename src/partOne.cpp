#include <iostream>
#include "someFunc1.cpp"
using namespace std;

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
