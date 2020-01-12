#include "mainwindow.h"
#include "calculator.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Calculator calc;

    QObject::connect(&w, SIGNAL(change_operation(QString, QString)), &calc, SLOT(set_operation(QString, QString)));
    QObject::connect(&w, SIGNAL(calc_equal(QString)), &calc, SLOT(equal(QString)));

    QObject::connect(&calc, SIGNAL(div_by_zero()), &w, SLOT(div_by_zero()));
    QObject::connect(&calc, SIGNAL(show_result(double)), &w, SLOT(show_result(double)));

    w.show();
    return a.exec();
}
