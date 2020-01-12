#include "calculator.h"
#include "QString"
#include "QPushButton"
#include <cmath>


double Calculator::get_in_memory()
{
    return in_memory;
}


void Calculator::set_in_memory(double number)
{
    in_memory = number;
}


char Calculator::get_operation()
{
    return operation;
}


void Calculator::set_operation(QString new_operation, QString str_number)
{
     operation = (char)new_operation[0].unicode();
     in_memory = str_number.toDouble();
}


double Calculator::decide(double number)
{
    double result = 0;
    switch (operation)
    {
        case '+': result = in_memory + number;break;
        case '-': result = in_memory - number;break;
        case '*': result = in_memory * number;break;
        case '/': result = in_memory / number;break;
        case '^': result = pow(in_memory,number);break;
        default:
            break;
    }

    return result;
}


void Calculator::equal(QString str_number)
{
    double number = str_number.toDouble();
    if (number == 0 && operation == '/')
        emit div_by_zero();
    else
    {
        double result = decide(number);
        emit show_result(result);
    }
}
