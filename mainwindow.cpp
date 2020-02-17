#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

#include <iostream>
#include <vector>
using std::vector;
#include <fstream>
using std::ifstream;
#include <string>
using std::string;
#include <sstream>
using std::istringstream;

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTextChanged()
{
    if(ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty()) ui->pushButton->setEnabled(false);
        else ui->pushButton->setEnabled(true);
}

vector<vector<uint>> parse(QString const& path) {
    vector<vector<uint>> res;
    auto input = ifstream(path.toLatin1().constData());
    string row;

    while (getline(input, row, '\n')) {
        vector<uint> vrow;
        auto rowstream = istringstream(row);
        string temp;
        while (getline(rowstream, temp, ','))
            vrow.emplace_back(stoi(temp));
        res.push_back(vrow);
    }

    input.close();
    return res;
}

void MainWindow::on_toolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "D:/Agrail/GitHub/MMatrix/",
                                                    tr("CSV Files (*.csv)"));
    ui->lineEdit->setText(fileName);
}

void MainWindow::on_toolButton_2_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "D:/Agrail/GitHub/MMatrix/",
                                                    tr("CSV Files (*.csv)"));
    ui->lineEdit_2->setText(fileName);
}

void MainWindow::on_pushButton_clicked()
{
    auto matrix1 = parse(ui->lineEdit->text());
    auto matrix2 = parse(ui->lineEdit_2->text());

    uint N1 = matrix1.size();
    uint M1 = matrix1[0].size();
    uint N2 = matrix2.size();
    uint M2 = matrix2[0].size();

    if (M1 != N2) {
        QMessageBox::critical(this, "Ошибка!", "Проверьте матрицы! Умножение происходит по формуле NxM * MxK = NxK!");
    } else {

    }
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    MainWindow::onTextChanged();
}

void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    MainWindow::onTextChanged();
}
