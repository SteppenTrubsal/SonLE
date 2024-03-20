#include <string>
#include <muParser.h>
using namespace std;

string& eqRebuild(string& s) {
    int equalSign = s.find('=');
    string left = s.substr(0, equalSign);
    string right = s.substr(equalSign + 1);
    s.clear();
    s = left + "-" + right;
    return s;
}

struct rootRange {
    double a = 0;
    double b = 0;
    double root = 0;
    int counter = 0;
};

double func(string& fun, double x){
    mu::Parser parser;
    parser.SetExpr(fun);
    parser.DefineVar("x", &x);
    return parser.Eval();
}
vector<rootRange> findRootSpaces(string& funcStr, double a = -100, double b = 100, double eps = 0.5){
    vector<rootRange> spaces;
    for(a; a < b; a+=eps){
        if(func(funcStr, a)*func(funcStr, a+eps) <= 0){
            rootRange temp;
            temp.a = a;
            temp.b = a+eps;
            spaces.push_back(temp);
        }
    }
    return spaces;
}

double findDerivative(string& funcStr, double x, double eps = 1e-6){
    return ((func(funcStr, (x+eps)) - func(funcStr, (x-eps)))/(2*eps));
}

double absol(double x){
    return x < 0? -x : x;
}

void NewtonMethod(string& funcStr, rootRange& root, double eps = 1e-6){
    double diff;
    double x0 = (root.a + root.b)/2;
    do{
        double x1;
        x1 = x0 - func(funcStr, x0)/findDerivative(funcStr, x0);
        diff = absol(x1 - x0);
        x0 = x1;
        root.counter++;
    }while(diff > eps);
    root.root = x0;
}