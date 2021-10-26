#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "pseudoterm.h"

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
    std::string str;//多分0x80以降が紛れ込むことはないので、ucharじゃなくても大丈夫......?
    int offset = 0, row = 0, col = 0;

    void parseEscapeSequence(std::basic_string<uchar> input, unsigned long *index);

protected:
    void keyPressEvent(QKeyEvent* event);
    void keyPress(int key);
    bool eventFilter(QObject* obj, QEvent* event);

};

#endif // MAINWINDOW_H
