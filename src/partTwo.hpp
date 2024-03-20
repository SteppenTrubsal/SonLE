#include <iostream>
#include <thread>
#include <mutex>
#include "someFunc.hpp"
using namespace std;

//mutex mtx;

struct mulRoots {
    double ax = 0, ay = 0;
    double rootX = 0, rootY = 0;
    int counter = 0;
};
struct diff_var {
    double diff = 0;
    double var = 0;
};
vector<vector<diff_var>> findPole(string&, string&, double, double, double);
vector<mulRoots> findRootSpaces(string& func1, string& func2, double a = -10, double b = 10, double eps = 1);
void NewtonMethod(string&, string&, mulRoots&, double eps = 1e-4);

void subMain2(){
    string func1;
    string func2;

    cout << "Enter a system of equations (separated by new line):" << endl;
    getline(cin, func1);
    getline(cin, func2);
    eqRebuild(func1);
    eqRebuild(func2);

    /*vector<thread> t;

    for (int i = -100; i <= 75; i += 25) {
        t.emplace_back(thread(findRootSpaces(func1, func2, i, i + 25, 1)));
    }

    for (auto& tr : t) {
        tr.join();
    }*/

    vector<mulRoots> roots = findRootSpaces(func1, func2);
    for (int i = 0; i < roots.size(); i++) {
        NewtonMethod(func1, func2, roots[i], 1e-4);
        cout << roots[i].rootX << " " << roots[i].rootY << endl;
    }
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
vector<mulRoots> findRootSpaces(string& func1, string& func2, double a, double b, double eps) {
    vector<vector<diff_var>> pole = findPole(func1, func2, a, b, eps);
    vector<mulRoots> roots;

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
                mulRoots temp;
                temp.ax = i+a;
                temp.ay = j+a;
                roots.push_back(temp);
            }
        }
    }
    pole.clear();
    return roots;
}

void NewtonMethod(string& func1, string& func2, mulRoots& root, double eps) {
    double diff;
    double x0 = root.ax;
    double y0 = root.ay;
    do {
        double x1;
        double y1;
        
        double a11 = findDerivativeByX(func1, x0, y0);
        double a12 = findDerivativeByY(func1, x0, y0);
        double a21 = findDerivativeByX(func2, x0, y0);
        double a22 = findDerivativeByY(func2, x0, y0);

        double b1 = -mulFunc(func1, x0, y0) + a11 * x0 + a12 * y0;
        double b2 = -mulFunc(func2, x0, y0) + a21 * x0 + a22 * y0;

        x1 = (b1 * a22 - b2 * a12) / (a11 * a22 - a21 * a12);
        y1 = (b2 * a11 - b1 * a21) / (a11 * a22 - a21 * a12);
        diff = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
        x0 = x1;
        y0 = y1;
        root.counter++;
    } while (diff > eps);
    root.rootX = x0;
    root.rootY = y0;
}