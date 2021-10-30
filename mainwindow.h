#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "pseudoterm.h"
#include "mstring.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setTerm(pterm::PseudoTerm*);

public slots:
    void timer();

private:
    Ui::MainWindow *ui;
    pterm::PseudoTerm *term;
    int pip;
    std::basic_string<uchar> log;
    pterm::mstring windowBuffer[1000];
    int offset = 0, row = 0, col = 0, WINDOWBUFFER_MAX = 1000, winWidth = 80, winHeight = 30;
    char BELL = 3;
    char bell = 0;
    void append(std::basic_string<uchar> input, unsigned long *index);
    void parseEscapeSequence(std::basic_string<uchar> input, unsigned long *index);

protected:
    void keyPressEvent(QKeyEvent* event);
    void keyPress(int key);
    bool eventFilter(QObject* obj, QEvent* event);
    void paintEvent(QPaintEvent* event);

};

#endif // MAINWINDOW_H
