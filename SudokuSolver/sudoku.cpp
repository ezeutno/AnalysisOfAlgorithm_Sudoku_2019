#include "sudoku.h"
#include "ui_sudoku.h"
#include "sudoku_solver_v3.cpp"
//#include "sudoku_solver_v2.cpp"
#include <QString>
#include <QObject>
#include <QtCharts>


sudoku::sudoku(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sudoku)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(679, 552));
}


sudoku::~sudoku()
{
    delete ui;
}

void position_cruncer(int &a,int &b){
    if (a%3 == 2 && b == 8){
        a = (a/3+1)*3;
        b = 0;
    }else if (a%3 == 2 && b%3 == 2){
        b = (b/3+1)*3;
        a = (a/3)*3;
    }else if (b%3 == 2){
        b = (b/3)*3;
        a++;
    }else{
        b++;
    }
}

void sudoku::pushdata(bool ps_dr){
    int a = 0;
    int b = 0;
    //    int test = 1;
    foreach(QPlainTextEdit* pe, findChildren<QPlainTextEdit*>()) {
        setData(a,b,pe->toPlainText().toInt(),ps_dr);
        position_cruncer(a,b);
    }
}

void sudoku::pulldata(bool blocked){
    int a = 0;
    int b = 0;
    //    int test = 1;
    foreach(QPlainTextEdit* pe, findChildren<QPlainTextEdit*>()) {
        int data = getData(a,b);
        if (data != 0) pe->setPlainText(QString::number(data));
        else pe->setPlainText("");
        if (blocked && data != 0) pe->setReadOnly(true);
        else pe->setReadOnly(false);
        position_cruncer(a,b);
    }
}

void sudoku::on_generateButton_clicked()
{
    ui->generateButton->setDisabled(true);
    int lvl = ui->sudokulvlspinbox->value();
    generate_sudoku(lvl);
    pulldata(true);
    check();
    ui->generateButton->setDisabled(false);
}

void sudoku::on_clearButton_clicked()
{
    clearData();
    foreach(QPlainTextEdit* pe, findChildren<QPlainTextEdit*>()) {
        pe->clear();
        pe->setReadOnly(false);
        QPalette p = pe->palette();
        p.setColor(QPalette::Active, QPalette::Base, Qt::white);
        p.setColor(QPalette::Inactive, QPalette::Base, Qt::white);
        pe->setPalette(p);
        pe->show();
    }
    pushdata();
}

void sudoku::on_resetButton_clicked(){
    int a = 0;
    int b = 0;
    //    int test = 1;
    foreach(QPlainTextEdit* pe, findChildren<QPlainTextEdit*>()) {
        int data = getData(a,b,true);
        if (data != 0) pe->setPlainText(QString::number(data));
        else pe->setPlainText("");
        if (data != 0) pe->setReadOnly(true);
        else pe->setReadOnly(false);
        QPalette p = pe->palette();
        p.setColor(QPalette::Active, QPalette::Base, Qt::white);
        p.setColor(QPalette::Inactive, QPalette::Base, Qt::white);
        pe->setPalette(p);
        pe->show();
        position_cruncer(a,b);
    }
}

void sudoku::on_checkButton_clicked(){
    int a = 0;
    int b = 0;
    check_data_reset();
    pushdata(false);
    foreach(QPlainTextEdit* pe, findChildren<QPlainTextEdit*>()) {
        int data = getData(a,b,true);
        if (data == 0){
            if(re_check(a,b,false) || getData(a,b) == 0){
                if (getData(a,b) != 0)re_check(a,b,true);
                QPalette p = pe->palette();
                p.setColor(QPalette::Active, QPalette::Base, Qt::white);
                p.setColor(QPalette::Inactive, QPalette::Base, Qt::white);
                pe->setPalette(p);
                pe->show();
            }else{
                QPalette p = pe->palette();
                p.setColor(QPalette::Active, QPalette::Base, Qt::red);
                p.setColor(QPalette::Inactive, QPalette::Base, Qt::red);
                pe->setPalette(p);
                pe->show();
            }
        }
        position_cruncer(a,b);
    }
}


void sudoku::on_solveButton_clicked(){
    pushdata();
    QElapsedTimer timer;
    timer.start();
    solve_math();
    bool math_finish = isit_finish();
    double math_time = timer.nsecsElapsed()/1000;
    reset_sudoku();

    timer.start();
    solve_backtracking();
    double backtrack_time = timer.nsecsElapsed()/1000;
    reset_sudoku();

    timer.start();
    solve_backtracking_plus();
    double mbt_time = timer.nsecsElapsed()/1000;

    pulldata();

    if(!math_finish) math_time = -1;

    // CHART
    QBarSet *set1;
    if(math_time==-1) set1 = new QBarSet("Math error");
    else set1 = new QBarSet("Math: " + QString::number(math_time) + "ms");
    QBarSet *set2 = new QBarSet("Backtracking: " + QString::number(backtrack_time) + "ms");
    QBarSet *set3 = new QBarSet("Math+Backtrack: " + QString::number(mbt_time) + "ms");


    *set1 << math_time;
    *set2 << backtrack_time;
    *set3 << mbt_time;

    QBarSeries *series = new QBarSeries();
    series->append(set1);
    series->append(set2);
    series->append(set3);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Time in Computing Sudoku");
    chart->setAnimationOptions(QChart::AllAnimations);

    QStringList categories;
    categories << "Algorithm";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QMainWindow *chartWindow = new QMainWindow;
    chartWindow->setCentralWidget(chartView);
    chartWindow->resize(500, 500);
    chartWindow->setWindowTitle("Sudoku Time Chart");
    chartWindow->show();
}
