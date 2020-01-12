#ifndef CALCULATOR_H
#define CALCULATOR_H

#endif // CALCULATOR_H

#include "QObject"
#include "QString"


class Calculator : public QObject
{
    Q_OBJECT

    double in_memory = 0;
    char operation = '+';


public:
    Calculator() {};
    virtual ~Calculator() {};

    double get_in_memory();
    char get_operation();

    void set_in_memory(double number);

    double decide(double number);


public slots:
     void set_operation(QString new_operation, QString str_number);
     void equal(QString str_number);

signals:
     void div_by_zero();
     void show_result(double result);
};
