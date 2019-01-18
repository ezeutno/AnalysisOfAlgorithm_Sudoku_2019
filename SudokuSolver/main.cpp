#include "sudoku.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication prog(argc, argv);

    sudoku w;
    w.show();

    return prog.exec();
}
