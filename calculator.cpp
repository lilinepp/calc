#include "calculator.h"

// конструктор по умолчанию
Calculator::Calculator()
{
    postfix = std::stack<std::string>();
}

// приоритет символов
short Calculator::Priority(char& __oper)
{
    switch (__oper)
    {
    // скобка во избежание ошибки
    case '(':
        return 0;
    // бинарные операции
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    // унарные операции
    case '~':
    case 'c':
    case 's':
    case 't':
    case 'g':
        return 4;
    // числа
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
    case '.':
        return 5;
    default:
        return -1;
    }
    throw std::invalid_argument("ERROR: unknown symbol");
}

// метод преобразования инфиксного выражения в постфиксное
void Calculator::Deijkstra(std::string& __inf)
{
    // стек развёрнутой постфиксной записью
    std::stack<std::string> postfix_reverse;
    // стек для операций
    std::stack<char> operations;

    // буфер для чисел
    std::string num_buf = "";
    // флаг наличия чисел
    bool availabilityOfNumbers = 0;

    // счётчики для проверки выражения
    unsigned num_count = 0;
    unsigned oper_count = 0;

    // ------ проверка наличия цифр в выражении
    // --
    // --
    for (auto i = __inf.begin(); i <= __inf.end(); i++)
        if ('0' <= *i && *i <= '9')
        {
            availabilityOfNumbers = 1;
            break;
        }
    if (availabilityOfNumbers == 0)
        throw std::invalid_argument("ERROR: number not found");

    // ----- проверка на лишнюю унарную операцию
    // --
    // --
    for (auto i = __inf.begin(); i <= __inf.end(); i++)
        if (Priority(*i) == 4 && Priority(*(i+1)) != 5 && Priority(*(i+1)) != 4 && Priority(*(i+1)) != 0)
            throw std::invalid_argument("ERROR: extra unary operator");

    // ----- преобразование инфиксного выражения в постфикное
    // --
    // --
    for (auto i = __inf.begin(); i <= __inf.end(); i++)
    {
        if (i == __inf.end())
        {
            if (num_buf.empty())
                break;
            else
            {
                // если буфер не пустой в конце прохода строки,
                // выталкиваем число из этого буфера в стек
                postfix_reverse.push(num_buf);
                // очищаем строку
                num_buf.clear();
                break;
            }
        }
        // если найдено число или плавающая точка
        if (Priority(*i) == 5)
        {
            // занести её в буфер
            num_buf += *i;
            continue;
            // если число кончилось или не найдено
            // проверяем буфер на заполненность
        }
        else if (!num_buf.empty())
        {
            // если буфер не пустой,
            // выталкиваем число в стек
            postfix_reverse.push(num_buf);
            // очищаем строку
            num_buf.clear();
        }
        // если на текущей позиции открывающаяся скобка
        if (*i == '(')
        {
            operations.push(*i);
            continue;
        }
        // если на текущей позиции закрывающаяся скобка
        if (*i == ')')
        {
            if (operations.empty())
                throw std::invalid_argument("ERROR: extra right bracket");
            // выталкиваем из стека все операторы
            // до открывающейся скобки
            while (operations.top() != '(')
            {
                num_buf = operations.top();
                postfix_reverse.push(num_buf);
                operations.pop();
                if (operations.empty())
                    throw std::invalid_argument("ERROR: extra right bracket");
            }
            // удаляем закрывающуюся скобку
            operations.pop();
            num_buf.clear();
            continue;
        }
        // если стек операций пуст
        if (operations.empty())
        {
            // просто закидываем операцию в стек
            operations.push(*i);
            continue;
        }
        // если стек операций не пуст
        // выталкиваем все операторы, имеющие приоритет выше
        else {
            // пока приоритет операции в стеке выше рассматриваемого
            while (!operations.empty())
            {
                if (Priority(operations.top()) >= Priority(*i))
                {   // выталкиваем оператор в выходной стек
                    num_buf = operations.top();
                    postfix_reverse.push(num_buf);
                    operations.pop();
                }
                else
                    break;
            }
            // записываем оператор в стек операций
            operations.push(*i);
            num_buf.clear();
        }
    }

    // ----- проверка соответствия количества операций и чисел
    // --
    // --
    if (!operations.empty() && postfix_reverse.empty())
        throw std::invalid_argument("ERROR: extra operation");

    // выталкиваем оставшиеся операторы
    while (!operations.empty())
    {
        // выталкиваем оператор в выходной стек
        num_buf = operations.top();
        postfix_reverse.push(num_buf);
        operations.pop();
    }

    // ----- заполняем стек с постфиксной записью
    // --
    // --
    while (!postfix_reverse.empty())
    {
        // заодно проверим, соответствуют ли операции числам
        switch (Priority(postfix_reverse.top()[0]))
        {
        // если найдена скобка (не дай Бог)
        case '0':
            throw std::invalid_argument("ERROR: extra right bracket");
            break;
        case '1':
        case '2':
        case '3':
        // если бинарная операция
            oper_count += 2;
            break;
        // если унарная операция
        case '4':
            oper_count += 1;
            break;
        // если число
        case '5':
            num_count += 2;
            break;
        }
        // если не соответствует, то ошибка
        if (oper_count > num_count)
            throw std::invalid_argument("ERROR: extra operations");

        this->postfix.push(postfix_reverse.top());
        postfix_reverse.pop();
    }
    postfix_reverse = std::stack<std::string>();
    operations = std::stack<char>();
}

big Calculator::Calculate(std::string& __inf)
{
    // преобразуем инфиксную строку в постфиксную
    Deijkstra(__inf);
    // создаём стек-буфер для вычисления
    std::stack<big> buf_calc;
    // ... переменную для верхнего числа из стека
    big ultimate = 0.00;
    // ... переменную для предверхнего числа из стека
    big penultimate = 0.00;

    // пока постфиксная запись не будет пройдена
    while (!postfix.empty())
    {
        // если текущий элемент является числом
        if (Priority(postfix.top()[0]) == 5)
        {
            // заносим его в стек вычисления
            buf_calc.push(stold(postfix.top()));
            // удаляем из постфиксной записи
            postfix.pop();
        }
        // если текущий элемент является операцией
        else
        {
            // берём последнее число из стека
            ultimate = buf_calc.top();
            buf_calc.pop();
            // если есть предпоследнее число
            if (!buf_calc.empty())
            {
                // то его тоже берём
                penultimate = buf_calc.top();
            }
            // обрабатываем операцию
            switch (postfix.top()[0])
            {
            case '(':
                throw std::invalid_argument("ERROR: extra left breacket");
                break;
            // ... сложения
            case '+':
                buf_calc.pop();
                buf_calc.push(penultimate + ultimate);
                break;
            // ... вычитания
            case '-':
                buf_calc.pop();
                buf_calc.push(penultimate - ultimate);
                break;
            // ... умножения
            case '*':
                buf_calc.pop();
                buf_calc.push(penultimate * ultimate);
                break;
            // ... деления
            case '/':
                buf_calc.pop();
                if (ultimate == 0)
                    throw std::invalid_argument("ERROR: division by zero");
                buf_calc.push(penultimate / ultimate);
                break;
            case '^':
                buf_calc.pop();
                buf_calc.push(pow(penultimate, ultimate));
                break;
            // ... унарного минуса
            case '~':
                buf_calc.push(ultimate * -1);
                break;
            // ... косинуса
            case 'c':
                buf_calc.push(cos(ultimate));
                break;
            // ... синуса
            case 's':
                buf_calc.push(sin(ultimate));
                break;
            // ... тангенса
            case 't':
                buf_calc.push(tan(ultimate));
                break;
            // ... котангенса
            case 'g':
                buf_calc.push(1 / tan(ultimate));
                break;
            // если найден какой-то инородный символ
            default:
                throw std::invalid_argument("ERROR: unknown symbol");
                return -1;
                break;
            }
            postfix.pop();
        }
    }
    last_result = buf_calc.top();
    buf_calc = std::stack<big>();
    return last_result;
}

void Calculator::Clear()
{
    postfix = std::stack<std::string>();
}

big Calculator::Get_Last()
{
    return last_result;
}
