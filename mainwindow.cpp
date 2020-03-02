#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QtConcurrent>
#include <QFuture>

#include <iostream>
#include <fstream>
using std::ifstream;
#include <string>
using std::string;
#include <sstream>
using std::istringstream;

#include <chrono>

using namespace std;

static const int THREAD_COUNT = QThread::idealThreadCount();

struct MatrixMultiplication {
    QVector<QVector<int>> matrix1;
    QVector<QVector<int>> matrix2;
    uint MFrom;
    uint MTo;
    uint NFrom;
    uint NTo;
    uint MN;
    QVector<QVector<int>>* res;
};

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

void MainWindow::on_toolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"),
                                                    "/home/",
                                                    tr("CSV Files (*.csv)"));
    ui->lineEdit->setText(fileName);
}

void MainWindow::on_toolButton_2_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"),
                                                    "/home/",
                                                    tr("CSV Files (*.csv)"));
    ui->lineEdit_2->setText(fileName);
}

QVector<QVector<int>> parse(QString const& path, QString k) {
    QVector<QVector<int>> res;
    try {
        auto input = ifstream(path.toLatin1().constData());
        string row;

        while (getline(input, row, '\n')) {
            QVector<int> vrow;
            auto rowstream = istringstream(row);
            string temp;
            while (getline(rowstream, temp, ','))
                vrow.push_back(stoi(temp));
            res.push_back(vrow);
        }

        input.close();
        return res;
    } catch (...) {
        QMessageBox::critical(nullptr, "Ошибка!", "Проверьте матрицу " + k + "!");
        QVector<QVector<int>> neres;
        return neres;
    }
}

/*QVector<QVector<int>> multiplication(uint N1, uint M1, uint M2, QVector<QVector<int>> const&matrix1, QVector<QVector<int>> const&matrix2)
{
    QVector<QVector<int>> res(N1, QVector<int>(M2, 0));

    for (uint i = 0; i < N1; i++) {
        for (uint j = 0; j < M2; j++) {
            for (uint k = 0; k < M1; k++) {
                res[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    return res;
}*/

void multiplication(const MatrixMultiplication& task)
{
    for (uint i = task.NFrom; i < task.NTo; i++) {
        for (uint j = task.MFrom; j < task.MTo; j++) {
            for (uint k = 0; k < task.MN; k++) {
                (*task.res)[i][j] += task.matrix1[i][k] * task.matrix2[k][j];
            }
        }
    }
}

void write_file(uint N, uint M, QVector<QVector<int>> const&res)
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
    auto matrix1 = parse(ui->lineEdit->text(), "1");
    if (matrix1.size() == 0) return void();
    auto matrix2 = parse(ui->lineEdit_2->text(), "2");
    if (matrix2.size() == 0) return void();

    uint N1 = matrix1.size();
    uint M1 = matrix1[0].size();
    uint N2 = matrix2.size();
    uint M2 = matrix2[0].size();

    if (M1 != N2) {
        QMessageBox::critical(this, "Ошибка!", "Проверьте матрицы! Количество столбцов первой матрицы должно совпадать с количеством строк второй матрицы!");
    } else {
        auto start = chrono::high_resolution_clock::now();

        //QVector<QVector<int>> res = multiplication(N1, M1, M2, matrix1, matrix2);

        QVector<QVector<int>> res(N1, QVector<int>(M2, 0));
        QVector<MatrixMultiplication> tasks;

        uint tcount = THREAD_COUNT;
        if (N1 < tcount) {
            tcount = N1;
        }

        uint repeats = N1 / tcount;
        uint N = 0;
        for(; N < N1 - repeats; N += repeats ) {
                tasks << MatrixMultiplication { matrix1, matrix2, 0, M2, N, N + repeats, M1, &res };
        }
        QFuture< void > future = QtConcurrent::map( tasks, multiplication );
        multiplication( MatrixMultiplication{ matrix1, matrix2, 0, M2, N, N1, M1, &res } );
        future.waitForFinished();

        auto end = chrono::high_resolution_clock::now();
        write_file(N1, M2, res);
        QMessageBox::information(this, "Выполнено", "Результат записан в файл result.csv");
        auto time = chrono::duration_cast<chrono::microseconds>(end - start).count();
        qDebug() << time << "msc.";
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
