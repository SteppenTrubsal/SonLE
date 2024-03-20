#include <iostream>
#include "someFunc2.cpp"
using namespace std;

void subMain2(){
    string func1;
    string func2;

    /*cout << "Enter a system of equations (separated by new line):" << endl;
    getline(cin, func1);
    getline(cin, func2);
    eqRebuild2(func1);
    eqRebuild2(func2);*/

    vector<vector<double>> solPole = findPole(func1, func2);
    /*for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            cout << solPole[i][j] << " ";
        }
        cout << endl;
    }*/
}