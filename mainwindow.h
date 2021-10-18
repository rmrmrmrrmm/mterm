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

public slots:
    void onTextChanged();

private:
    Ui::MainWindow *ui;
    pterm::PseudoTerm *term;
    static bool isUnHandleKeysPressed(int key);

protected:
    void keyPressEvent(QKeyEvent* event);
    void keyPressEvent(int key);
    bool eventFilter(QObject* obj, QEvent* event);

};

#endif // MAINWINDOW_H
