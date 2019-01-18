#ifndef SUDOKU_H
#define SUDOKU_H

#include <QWidget>

namespace Ui {
class sudoku;
}

class sudoku : public QWidget
{
    Q_OBJECT

public:
    explicit sudoku(QWidget *parent = nullptr);
    ~sudoku();

private slots:
    void on_generateButton_clicked();

    void on_clearButton_clicked();

    void on_solveButton_clicked();

    void on_resetButton_clicked();

    void on_checkButton_clicked();

private:
    Ui::sudoku *ui;
    void pushdata(bool ps_dr = true);
    void pulldata(bool blocked = false);
};

#endif // SUDOKU_H
