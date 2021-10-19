#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QTimer>
#include <sys/fcntl.h>
#include <stdio.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    ui->setupUi(this);
    ui->label->installEventFilter(this);
    str = "";
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::timer));
    timer->start(32);
}

MainWindow::~MainWindow()
{
    delete term;
    delete ui;
}

void MainWindow::setTerm(pterm::PseudoTerm *pterm){
    term = pterm;
    if((pip=term->getPipe())==-1){
        perror("open");
        exit(1);
   }
}

void MainWindow::timer(){
    char buf[128];
    int nread;
    char header;
    while((::read(pip, &header, 1)) > 0){
        if((nread = ::read(pip, buf, header)) != header)
            exit(-1);
        buf[nread] = 0;
        str.append(buf);
        std::cout << nread << std::endl;
    }
    ui->label->setText(str.c_str());
}

void MainWindow::keyPressEvent(QKeyEvent* event){
    this->keyPressEvent(event->key());
}

void MainWindow::keyPressEvent(int key){
    term->keyPressed(key);
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->label && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();
        if (MainWindow::isUnHandleKeysPressed(key)) {
            this->keyPressEvent(key);
            return true;
        } else
            return false;
    }
    return false;
}

//textBrowserだとSpaceとかがMainWindowに到達してこなかったから作った
//labelに変えたので不要かもしれない
bool MainWindow::isUnHandleKeysPressed(int key){
    //if(key == Qt::Key_Space || (Qt::Key_Home <= key && key <= Qt::Key_PageDown))
    //return true;
    return false;
}
