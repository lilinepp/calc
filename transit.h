#ifndef TRANSIT_H
#define TRANSIT_H

#include <QMainWindow>
#include <QString>
#include <QPushButton>
#include <sstream>

#include "calculator.h"

class Transit
{
private:
    // калькулятор (программная часть)
    Calculator Calc;
    // буфер для хранения вставляемого символа
    QPushButton* buffer;
    // последний символ
    char ultimate;
    // предпоследний символ
    char penultimate;
    // строка удаления
    QString erase_buf;

    // ---- !
    // строка с инфиксным выражением
    std::string infix;
    // строка с инфиксным выражением
    QString input;

    // обработка чисел
    void Numeral_Insert(std::string __buf);
    // обработка операторов
    void Operator_Insert(std::string __buf);
    // обработка функций
    void Function_Insert(std::string __buf);

    // обработка скобок
    void Left_Bracket_Insert();
    void Right_Bracket_Insert();

    // обработка унарного минуса
    void Unary_Minus_Change();
    // обработка плавающей точки
    void Point_Insert();
    // ----- метод проверки баланса скобок
    bool BalancedBrackets() const;
public:
    // конструктор по умолчанию
       Transit();

       // метод вставки символа
       void Insert(std::string &__buf);

       // метод вывода символа
       QString Show();

       // методы стирания строк
       void Erase();
       void Clear();

       // метод вычисления с помощью калькулятора
       big Calculate();

    int open_brackets;  // добавление переменной для подсчета открытых скобок
    int close_brackets; // добавление переменной для подсчета закрытых скобок
};

#endif // TRANSIT_H

