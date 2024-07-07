#ifndef CALCULATOR_H
#define CALCULATOR_H


#include <string>
#include <stack>
#include <iostream>
#include <cmath>

using big = long double;

// ----- Класс калькулятора
class Calculator
{
private:
    // стек для хранения постфиксного выражения: 2 2 +
    std::stack<std::string> postfix;

    // алгоритм
    void Deijkstra(std::string& __inf);
    // последний результат
    big last_result;

public:
    // приоритет операций
    short Priority(char& __oper);

    // конструктор по умолчанию
    Calculator();

    // метод вычисления выражения
    big Calculate(std::string& __inf);

    // метод возврата последнего результата
    big Get_Last();

    // метод сброса полей калькулятора
    void Clear();
};
#endif // CALCULATOR_H
