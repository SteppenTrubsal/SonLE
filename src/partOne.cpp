#include <iostream>
#include <muParser.h>

void test(){
    std::string expression;
    double x;

    mu::Parser parser;
    parser.DefineVar("x", &x);

    std::cout << "Введите математическое выражение: ";
    std::getline(std::cin, expression);

    parser.SetExpr(expression);

    std::cout << "Введите значение переменной x: ";
    std::cin >> x;

    double result = parser.Eval();

    std::cout << "Значение функции: " << result << std::endl;

}