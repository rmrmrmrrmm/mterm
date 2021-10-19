#include "pseudoterm.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    pterm::PseudoTerm *pterm = new pterm::PseudoTerm();
    QApplication a(argc, argv);
    MainWindow w;
    w.setTerm(pterm);
    w.show();
    return a.exec();
}
