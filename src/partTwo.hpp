#include <iostream>
#include "someFunc.hpp"
using namespace std;

struct mulRoots {
    double a = 0, b = 0;
    double rootX = 0, rootY = 0;
    int counter = 0;
};
struct diff_var {
    double diff = 0;
    double var = 0;
};
vector<vector<diff_var>> findPole(string&, string&, double, double, double);
//vector<mulRoots> 
void findRootSpaces(string& func1, string& func2, double a = -100, double b = 100, double eps = 1);
//void NewtonMethod(string&, string&, rootRange&, double);

void subMain2(){
    string func1;
    string func2;

    cout << "Enter a system of equations (separated by new line):" << endl;
    getline(cin, func1);
    getline(cin, func2);
    eqRebuild(func1);
    eqRebuild(func2);

    findRootSpaces(func1, func2);

    //vector<vector<double>> solPole = findPole(func1, func2);
    /*for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            cout << solPole[i][j] << " ";
        }
        cout << endl;
    }*/
}

vector<vector<diff_var>> findPole(string& func1, string& func2, double a, double b, double eps) {
    vector<vector<diff_var>> pole;
    for (double i = a; i < b; i += eps) {
        vector<diff_var> slice;
        for (double j = a; j < b; j += eps) {
            diff_var temp;
            temp.diff = abs(mulFunc(func1, i, j) - mulFunc(func2, i, j));
            temp.var = abs(mulFunc(func1, i, j));
            slice.push_back(temp);
        }
        pole.push_back(slice);
        slice.clear();
    }
    return pole;
}
//vector<mulRoots> 
void findRootSpaces(string& func1, string& func2, double a, double b, double eps) {
    vector<vector<diff_var>> pole = findPole(func1, func2, a, b, eps);

    double max_diff = 0;
    double max_var = 0;
    for (int i = 0; i < pole.size(); i++) {
        for (int j = 0; j < pole[0].size(); j++) {
            max_diff = (max_diff < pole[i][j].diff && !isnan(pole[i][j].diff)) ? pole[i][j].diff : max_diff;
            max_var = (max_var < pole[i][j].var && !isnan(pole[i][j].var)) ? pole[i][j].var : max_var;
        }
    }
    for (int i = 0; i < pole.size(); i++) {
        for (int j = 0; j < pole[0].size(); j++) {
            pole[i][j].diff = (isnan(pole[i][j].diff)) ? max_diff : pole[i][j].diff;
            pole[i][j].var = (isnan(pole[i][j].var)) ? max_var : pole[i][j].var;
        }
    }

    for (int i = 1; i < pole.size()-1; i++) {
        for (int j = 1; j < pole[0].size()-1; j++) {
            if (
                (
                    pole[i][j].diff < pole[i-1][j].diff &&
                    pole[i][j].diff < pole[i+1][j].diff &&
                    pole[i][j].diff < pole[i][j-1].diff &&
                    pole[i][j].diff < pole[i][j+1].diff
                ) && (
                    pole[i][j].var < pole[i - 1][j].var &&
                    pole[i][j].var < pole[i + 1][j].var &&
                    pole[i][j].var < pole[i][j - 1].var &&
                    pole[i][j].var < pole[i][j + 1].var
                )
               ) {
                cout << i+a << " " << j+a << endl;
            }
        }
    }

    pole.clear();
}