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
        if (ui->display->text().indexOf('.') == -1)
            ui->display->setText(ui->display->text() + ".");
}
