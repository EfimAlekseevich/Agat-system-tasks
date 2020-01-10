#include "calculator.h"
#include "QString"
#include <cmath>


double Calculator::get_in_memory()
{
    return in_memory;
}


void Calculator::set_in_memory(double number)
{
    in_memory = number;
}


double Calculator::decide(char operation, double number)
{
    double result = 0;
    switch (operation)
    {
    case '+': result = number + in_memory;break;
    case '-': result = number - in_memory;break;
    case '*': result = number * in_memory;break;
    case '/': result = number / in_memory;break;
    case '^': result = pow(number,in_memory);break;
    default:
        break;
    }

    return result;
}


void Calculator::operation()
{

}


void Calculator::equal()
{

}
