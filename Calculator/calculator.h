#ifndef CALCULATOR_H
#define CALCULATOR_H

#endif // CALCULATOR_H

#include "QObject"
#include "QString"


class Calculator : public QObject
{
    Q_OBJECT
    double in_memory;


public:
    Calculator() {}
    virtual ~Calculator() {}

    double get_in_memory();

    void set_in_memory(double number);
    double decide(char operation, double number);


public slots:
    void operation();
    void equal();
};
