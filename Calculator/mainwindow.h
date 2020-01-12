#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QString"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool is_short(QString str);

private slots:
    void on_button_digit_clicked();
    void on_b_clear_clicked();
    void on_b_dot_clicked();
    void on_b_del_clicked();
    void on_operation_clicked();

    void on_b_equal_clicked();

    void div_by_zero();
    void show_result(double result);

signals:
    void change_operation(QString operation, QString number);
    void calc_equal(QString number);

private:
    Ui::MainWindow *ui;
    static const unsigned char max_digits = 8;

};
#endif // MAINWINDOW_H
