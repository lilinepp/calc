# calc
// calculate.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.


```
#include <iostream>
#include <string>
#include <stack>
#include <cmath>

using namespace std;

class Calculate
{
public:
    string calculate(string infix)
    {
        string postfix = InfixToPostfix(infix);
        return postfix;
    }

    int priority(char c)
    {
        switch (c)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return 0;

        case '+':
        case '-':
            return 1;

        case '*':
        case '/':
            return 2;

        case '^': // степень
        case 'r': // корень
        case '%': // процент
            return 3;

        case '(': // открывающая скобка
        case ')': // закрывающая скобка
            return -1;

        case 's': // синус
        case 'c': // косинус
        case 't': // тангенс
        case 'g'://котангенс
            return 4;

        case 'u': // унарный минус
            return 5;

        default:
            return -1;
        }
    }

    string InfixToPostfix(string infix)
    {
        string postfix;
        stack<char> symbol;

        for (int i = 0; i < infix.size(); i++)
        {
            switch (priority(infix[i]))
            {
            case 0: // Цифра
                postfix.push_back(infix[i]);
                break;
            case 1: // Операторы с приоритетом 1
                while (!symbol.empty() && priority(symbol.top()) >= 1)
                {
                    postfix.push_back(symbol.top());
                    symbol.pop();
                }
                symbol.push(infix[i]);
                break;
            case 2: // Операторы с приоритетом 2
                while (!symbol.empty() && priority(symbol.top()) == 2)
                {
                    postfix.push_back(symbol.top());
                    symbol.pop();
                }
                symbol.push(infix[i]);
                break;
            case 3: // Операторы с приоритетом 3 
                while (!symbol.empty() && priority(symbol.top()) >= 3)
                {
                    postfix.push_back(symbol.top());
                    symbol.pop();
                }
                symbol.push(infix[i]);
                break;
            case -1: // Скобки
                if (infix[i] == '(')
                {
                    symbol.push(infix[i]);
                }
                else if (infix[i] == ')')
                {
                    while (!symbol.empty() && symbol.top() != '(')
                    {
                        postfix.push_back(symbol.top());
                        symbol.pop();
                    }
                    if (!symbol.empty() && symbol.top() == '(')
                    {
                        symbol.pop(); // Удаляем открывающую скобку из стека
                    }
                }
                break;
            }
        }

        while (!symbol.empty())
        {
            postfix.push_back(symbol.top());
            symbol.pop();
        }

        return postfix;
    }

    double Postfix_value(string postfix)
    {
        stack<double> operands;

        for (int i = 0; i < postfix.size(); i++)
        {
            if (postfix[i] >= '0' && postfix[i] <= '9') // Проверяем, является ли символ цифрой
            {
                operands.push(static_cast<double>(postfix[i] - '0')); // Преобразуем символ цифры в число с плавающей точкой и помещаем в стек
            }
            else if (postfix[i] == '+' || postfix[i] == '-' || postfix[i] == '*' || postfix[i] == '/'  || postfix[i] == '^' || postfix[i] == 'r')
            {
                double operand2 = operands.top();
                operands.pop();
                double operand1 = operands.top();
                operands.pop();
                double result;

                switch (postfix[i])
                {
                case '+':
                    result = operand1 + operand2;
                    break;
                case '-':
                    result = operand1 - operand2;
                    break;
                case '*':
                    result = operand1 * operand2;
                    break;
                case '/':
                   // if (operand2 != 0)
                  //  {
                        result = operand1 / operand2;
                  //  }
                  //  else
                   // {
                     //   cout << "деление на ноль" << endl;
                      // return 0.0; // Вернуть 0.0 в случае деления на ноль
                   // }
                    break;
                case '^':
                    result = pow(operand1, operand2); 
                    break;
              //  case 'r':                     
                 //  result = sqrt(operand2);
                  //  break;
                }

                operands.push(result);
            }
        }

        return operands.top();
    }
                

    
};

int main()
{
    setlocale(LC_ALL, "RUSSIAN");
    Calculate my_calc;
    
    string infix = "r49";
    string postfix = my_calc.calculate(infix);
    cout << "Результат преобразования в постфиксную запись: " << postfix << endl;
    double result = my_calc.Postfix_value(postfix);
    cout << "Результат вычисления постфиксного выражения: " << result << endl;
  
}
//string infix = "2+6-5*5/8-7/4*3+7*2-8-6^2-√49";```
