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

vector<vector<int>> parse(QString const& path) {
    vector<vector<int>> res;
    auto input = ifstream(path.toLatin1().constData());
    string row;

    while (getline(input, row, '\n')) {
        vector<int> vrow;
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

vector<vector<int>> multiplication(uint N1, uint M1, uint M2, vector<vector<int>> const&matrix1, vector<vector<int>> const&matrix2)
{
    vector<vector<int>> res(N1, vector<int>(M2, 0));

    for (uint i = 0; i < N1; i++) {
        for (uint j = 0; j < M2; j++) {
            for (uint k = 0; k < M1; k++) {
                res[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    return res;
}

void write_file(uint N, uint M, vector<vector<int>> const&res)
{
    ofstream fout;
    fout.open("result.csv");
    for (uint i = 0; i < N; i++) {
        for (uint j = 0; j < M; j++) {
            fout << res[i][j];
            if (j != M - 1) fout << ',';
        }
        fout << endl;
    }
    fout.close();
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
        QMessageBox::critical(this, "Ошибка!", "Проверьте матрицы! Количество столбцов первой матрицы должно совпадать с количеством строк второй матрицы!");
    } else {
        vector<vector<int>> res = multiplication(N1, M1, M2, matrix1, matrix2);
        write_file(N1, M2, res);
        QMessageBox::information(this, "Выполнено", "Результат записан в файл result.csv");
    }
}

void MainWindow::on_lineEdit_textChanged()
{
    MainWindow::onTextChanged();
}

void MainWindow::on_lineEdit_2_textChanged()
{
    MainWindow::onTextChanged();
}
