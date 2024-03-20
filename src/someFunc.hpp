#pragma once
#include <string>
#include <muParser.h>
using namespace std;

void eqRebuild(string& s) {
    int equalSign = s.find('=');
    string left = s.substr(0, equalSign);
    string right = s.substr(equalSign + 1);
    s.clear();
    s = left + "-" + right;
}

double func(string& fun, double x){
    mu::Parser parser;
    parser.SetExpr(fun);
    parser.DefineVar("x", &x);
    return parser.Eval();
}
double mulFunc(string& fun, double x, double y) {
    mu::Parser parser;
    parser.SetExpr(fun);
    parser.DefineVar("x", &x);
    parser.DefineVar("y", &y);
    return parser.Eval();
}

double findDerivative(string& funcStr, double x, double eps = 1e-6){
    return ((func(funcStr, (x+eps)) - func(funcStr, (x-eps)))/(2*eps));
}
