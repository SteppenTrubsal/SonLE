#include <iostream>
#include <thread>
#include <mutex>
#include <functional>
#include "someFunc.hpp"
using namespace std;
#define epsilont = 1e-6;
mutex mtx;
#define ACRED     "\x1b[31m"
#define ACGREEN   "\x1b[32m"
#define ACYELLOW  "\x1b[33m"
#define ACBLUE    "\x1b[34m"
#define ACMAGENTA "\x1b[35m"
#define ACCYAN    "\x1b[36m"
#define ACRESET   "\x1b[0m"
struct mulRoots {
    double a = 0, b = 0;
    double rootX = 0, rootY = 0;
    int counter = 0;
};
struct diff_var {
    double diff = 0;
    double var = 0;
};
vector<vector<diff_var>> findPole(string, string, double, double, double);
void newtonsMethodDouble(string func1, string func2, vector<mulRoots>& results, double a = -100, double b = 100, double eps = 1);
void find2DimRootSpaces(string func1, string func2, vector<mulRoots>& results, double a, double b, double eps);

void subMain2(){
    string func1;
    string func2;

    cout << "Enter a system of equations (separated by new line):" << endl;
    getline(cin, func1);
    getline(cin, func2);
    eqRebuild(func1);
    eqRebuild(func2);

    vector<thread> t;
    size_t numThreads = std::thread::hardware_concurrency();
    vector<vector<mulRoots>> results(numThreads);
    const double a = -50, b = 50;
    double primEps = 1;
    double secEps = 1e-4;
    const double range = (b - a) / numThreads;
    for (int i = 0, j = 0; i < numThreads; i++) {
        double subA = a + i * range;
        double subB = subA + range;
        t.emplace_back(find2DimRootSpaces, func1, func2, std::ref(results[i]), subA, subB, primEps);
    }

    for (auto& tr : t) {
        tr.join();
    }

    vector<mulRoots> roots;
    find2DimRootSpaces(func1, func2, std::ref(roots), a, b, primEps);
    newtonsMethodDouble(func1, func2, std::ref(roots), a, b, secEps);
    
    cout << roots.size() << " root(s) was found:" << endl;
    for (int i = 0; i < roots.size(); i++) {
        if (roots[i].counter != 0) {
            cout << i + 1 << " root on " << "X = [" << roots[i].a << ", " << roots[i].a + primEps << "], Y = [" << roots[i].b << ", " << roots[i].b + primEps <<
                "] is x = " << roots[i].rootX << ", y = " << roots[i].rootY << ". It took " << roots[i].counter << " iterations." << endl;
        }
        else {
            cout << "For " << i + 1 << " root on " << "X = [" << roots[i].a << ", " << roots[i].a + primEps << "], Y = [" << roots[i].b << ", " << roots[i].b + primEps <<
                "] Newton solution condition is not fulfilled." << endl;
        }
    }

}

void find2DimRootSpaces(string func1, string func2, vector<mulRoots>& results, double a, double b, double eps) {
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

    for (int i = 1; i < pole.size() - 1; i++) {
        for (int j = 1; j < pole.back().size() - 1; j++) {

            if (
                (
                    pole[i][j].diff < pole[i - 1][j].diff &&
                    pole[i][j].diff < pole[i + 1][j].diff &&
                    pole[i][j].diff < pole[i][j - 1].diff &&
                    pole[i][j].diff < pole[i][j + 1].diff
                    ) && (
                        pole[i][j].var < pole[i - 1][j].var &&
                        pole[i][j].var < pole[i + 1][j].var &&
                        pole[i][j].var < pole[i][j - 1].var &&
                        pole[i][j].var < pole[i][j + 1].var
                        )
                ) {
                mulRoots temp;
                temp.a = i*eps + a;
                temp.b = j*eps + a;
                roots.push_back(temp);

            }
        }
    }
    lock_guard<mutex> lock(mtx);
    results.insert(results.end(), roots.begin(), roots.end());
}
vector<vector<diff_var>> findPole(string func1, string func2, double a, double b, double eps) {
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

void newtonsMethodDouble(string func1, string func2, vector<mulRoots>& results, double a, double b, double eps1) {
    for (int i = 0; i < results.size(); i++) {
        double diff;
        double x0 = results[i].a;
        double y0 = results[i].b;
        do {
            double x1;
            double y1;

            double a11 = findDerivativeByX(func1, x0, y0);
            double a12 = findDerivativeByY(func1, x0, y0);
            double a21 = findDerivativeByX(func2, x0, y0);
            double a22 = findDerivativeByY(func2, x0, y0);

            double b1 = -mulFunc(func1, x0, y0) + a11 * x0 + a12 * y0;
            double b2 = -mulFunc(func2, x0, y0) + a21 * x0 + a22 * y0;

            if (a11 * a22 - a21 * a12 == 0) { break; }

            x1 = (b1 * a22 - b2 * a12) / (a11 * a22 - a21 * a12);
            y1 = (b2 * a11 - b1 * a21) / (a11 * a22 - a21 * a12);
            diff = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
            x0 = x1;
            y0 = y1;
            results[i].counter++;
        } while (diff > eps1);
        results[i].rootX = x0;
        results[i].rootY = y0;
    }
}