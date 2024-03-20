#include <string>
#include <muParser.h>
using namespace std;

//#################     Obsolete file   ##################

struct rootRange2{
    double ax = 0;
    double bx = 0;
    double ay = 0;
    double by = 0;
    double rootX = 0;
    double rootY = 0;
    int counter = 0;
    double diff = 0;
};

vector<rootRange2> findRootRange2(string& fun1Str, string& fun2Str, double a = -100, double b = 100, double eps = 1){
    vector<rootRange2> useless;
    vector<rootRange2> roots;
    for(int i = a; i < b; i+=eps){
        for(int j = a; j < b; j+=eps){
            rootRange2 temp;
            temp.ax = i;
            temp.ay = j;
            temp.diff = absol2(func2(fun1Str, i,j) - func2(fun2Str, i, j));
            useless.push_back(temp);
            cout << "new vec: " << i << " " << j << endl;
        }
    }
    for(int i = 1; i < useless.size()-1; i++){
        if(useless[i].diff < useless[i-1].diff and useless[i].diff < useless[i+1].diff){
            roots.push_back(useless[i]);
            cout << "yes: " << useless[i].ax << " " << useless[i].ay << endl;
        }
    }
    useless.clear();
    return roots;
}
double fuck1(double x, double y){
    return(sqrt(x+y)-sqrt(2*y-5*x)-x);
}
double fuck2(double x, double y){
    return(sqrt(x+y)+sqrt(2*y-5*x)-y);
}
vector<vector<double>> findPole(string& fun1Str, string& fun2Str, double a = -9, double b = 10, double eps = 1){
    vector<vector<double>> pole;
    for(double i = a; i < b; i+=eps){
        vector<double> spline;
        for(double j = -1; j < b; j+=eps){
            cout << "(" << i << "," << j << ")" <<(round((fuck1(i,j) - fuck2(i,j))*10))/10 << " ";
            spline.push_back(fuck1(i,j) - fuck2(i,j));
        }
        cout << endl;
        pole.push_back(spline);
        spline.clear();
    }
    cout << "once" << endl;
    for(int i = 0; i < pole.size(); i++){
        for(int j = 0; j < pole[0].size(); j++){
            if(isnan(pole[i][j])){pole[i][j] = 1;}
        }
    }
    for(int i = 1; i < pole.size()-1; i++){
        for(int j = 1; j < pole[0].size()-1; j++){
            if(
                pole[i][j] <= pole[i-1][j] and
                pole[i][j] <= pole[i+1][j] and
                pole[i][j] <= pole[i][j-1] and
                pole[i][j] <= pole[i][j+1]
            ){
                cout << i << " " << j << "yes" << endl;
            }
            //cout << i+a << " " << j-1 << endl;
        }
    }
    return pole;
}