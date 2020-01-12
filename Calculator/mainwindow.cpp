#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->b_d0, SIGNAL(clicked()), this, SLOT(on_button_digit_clicked()));
    connect(ui->b_d1, SIGNAL(clicked()), this, SLOT(on_button_digit_clicked()));
    connect(ui->b_d2, SIGNAL(clicked()), this, SLOT(on_button_digit_clicked()));
    connect(ui->b_d3, SIGNAL(clicked()), this, SLOT(on_button_digit_clicked()));
    connect(ui->b_d4, SIGNAL(clicked()), this, SLOT(on_button_digit_clicked()));
    connect(ui->b_d5, SIGNAL(clicked()), this, SLOT(on_button_digit_clicked()));
    connect(ui->b_d6, SIGNAL(clicked()), this, SLOT(on_button_digit_clicked()));
    connect(ui->b_d7, SIGNAL(clicked()), this, SLOT(on_button_digit_clicked()));
    connect(ui->b_d8, SIGNAL(clicked()), this, SLOT(on_button_digit_clicked()));
    connect(ui->b_d9, SIGNAL(clicked()), this, SLOT(on_button_digit_clicked()));

    connect(ui->b_plus, SIGNAL(clicked()), this, SLOT(on_operation_clicked()));
    connect(ui->b_minus, SIGNAL(clicked()), this, SLOT(on_operation_clicked()));
    connect(ui->b_mult, SIGNAL(clicked()), this, SLOT(on_operation_clicked()));
    connect(ui->b_div, SIGNAL(clicked()), this, SLOT(on_operation_clicked()));
    connect(ui->b_pow, SIGNAL(clicked()), this, SLOT(on_operation_clicked()));

}

MainWindow::~MainWindow()
{
    qDebug("Exit");
    delete ui;
}

bool MainWindow::is_short(QString str)
{
    if (str.length() < max_digits)
        return true;
    else
        return false;
}


void MainWindow::on_button_digit_clicked()
{
    if (is_short(ui->display->text()))
    {
        QString digit = ((QPushButton *)sender())->text();
        if (ui->display->text() == "0")
            ui->display->setText(digit);
        else
            ui->display->setText(ui->display->text() + digit);
    }
}


void MainWindow::on_b_clear_clicked()
{
    ui->display->setText("0");
}


void MainWindow::on_b_dot_clicked()
{
    if (is_short(ui->display->text()))
        if (!ui->display->text().contains('.'))
            ui->display->setText(ui->display->text() + ".");
}


void MainWindow::on_b_del_clicked()
{
    if (ui->display->text().length() > 1)
        ui->display->setText(ui->display->text().chopped(1));
    else
        ui->display->setText("0");
}


void MainWindow::on_operation_clicked()
{
     QString operation = ((QPushButton *)sender())->text();
     ui->operation->setText((QString)operation[0]);
     ui->label_memory->setText(ui->display->text());
     emit change_operation(operation, ui->display->text());
     ui->display->setText("0");
}

void MainWindow::on_b_equal_clicked()
{
    emit calc_equal(ui->display->text());
}


void MainWindow::div_by_zero()
{
    ui->statusbar->showMessage("Dividing by zero!!!");
}


void MainWindow::show_result(double result)
{
    QString str_result = QString::number(result,'f', 6);
    ui->display->setText(str_result);
}

void MainWindow::on_b_save_clicked()
{
    if (ui->saved_values->count() < 10)
        ui->saved_values->addItem(ui->display->text());
    else
        ui->statusbar->showMessage("Max number of saved numbers!!!");
}


void MainWindow::on_saved_values_itemDoubleClicked(QListWidgetItem *item)
{
    ui->display->setText(item->text());
}

void MainWindow::on_pl_mi_clicked()
{
    if (ui->display->text()[0] == '-')
        ui->display->setText(ui->display->text().remove(0,1));
    else
        ui->display->setText(ui->display->text().insert(0, '-'));
}
