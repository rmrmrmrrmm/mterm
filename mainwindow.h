#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "pseudoterm.h"
#include "mstring.h"
#include "windowbuffer.h"

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
    //pterm::mstring windowBuffer[256];
    pterm::WindowBuffer windowBuffer;
    //int offset = 0, row = 0, col = 0, WINDOWBUFFER_MAX = 256, winWidth = 80, winHeight = 30,
    //windowTopRow = 0;
    char BELL = 3;
    char bell = 0;
    void parseEscapeSequence(std::basic_string<uchar> input, unsigned long *index);
    void keyPress(int key);

protected:
    void keyPressEvent(QKeyEvent* event);
    bool eventFilter(QObject*, QEvent* event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent*);

};

#endif // MAINWINDOW_H
