#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "keyutil.h"
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
    ui->centralwidget->installEventFilter(this);
    ui->label->installEventFilter(this);
    ui->scrollArea->installEventFilter(this);
    ui->scrollAreaWidgetContents->installEventFilter(this);
    str = log = "";
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
        log.append(buf);
        for(int i = 0; i < nread; i++){
            switch(char c = buf[i]) {
            case 0x07:
                //ring a bell
                continue;
            case 0x08:
                //BS
                if(i+2 < nread && buf[i+1] == 0x20 && buf[i+2] == 0x08){
                    str.erase(str.length() - 1 + offset, 1);
                    i+=2;
                } else{
                    offset -= 1;
                }
                continue;
            case 0x0a:
                //LF
                row++;
                col = offset =0;
                str += c;
                continue;
            case 0x0d:
                //CR
                offset = -col;
                continue;
            default:
                if(offset < 0){
                    str.replace(str.length() + offset, 1, 1, c);
                    offset++;
                } else{
                    str += c;
                    col++;
                }
                continue;
            }
        }
    }
    ui->label->setText(str.c_str());
}

void MainWindow::keyPressEvent(QKeyEvent* event){
    int key = event->key();
    std::cout << "  press:" << key << std::endl;

    if(QApplication::queryKeyboardModifiers() & Qt::ControlModifier){
        if(key >= Qt::Key_At && key <= Qt::Key_Underscore){
            this->keyPressEvent(key - Qt::Key_At);
            return;
        } else if(key == Qt::Key_Question){
            this->keyPressEvent(127);
            return;
        }
    }

    if(QApplication::queryKeyboardModifiers() & Qt::ShiftModifier){
        if(key >= Qt::Key_A && key <= Qt::Key_Z){
            this->keyPressEvent(key);
            return;
        }
        if(key == Qt::Key_At){
            this->keyPressEvent(0x60);
            return;
        }
    }

    if(key >= Qt::Key_A && key <= Qt::Key_Z){
        this->keyPressEvent(::tolower(key));
        return;
    }

    if(key == Qt::Key_At){
        this->keyPressEvent(key);
        return;
    }
    if(key == Qt::Key_Space){
        this->keyPressEvent(0x20);
        return;
    }

    if(key == Qt::Key_Backspace){
        this->keyPressEvent(0x08);
        return;
    }
    if(key == Qt::Key_Return){
        this->keyPressEvent(0x0d);
        return;
    }
    if(key == Qt::Key_Escape){
        this->keyPressEvent(0x1B);
        return;
    }

    if(key == Qt::Key_Left){
        this->keyPressEvent(0x02);
        return;
    }

    if(key == Qt::Key_Right){
        this->keyPressEvent(0x06);
        return;
    }

    if(key == Qt::Key_Up){
        this->keyPressEvent(0x10);
        return;
    }

    if(key == Qt::Key_Down){
        this->keyPressEvent(0x0E);
        return;
    }

    if(key == Qt::Key_Home){
        this->keyPressEvent(0x01);
        return;
    }

    if(key == Qt::Key_End){
        this->keyPressEvent(0x05);
        return;
    }

    if(key == Qt::Key_Control){
        return;
    }

    if(key == Qt::Key_Alt){
        return;
    }

    if(key == Qt::Key_Shift){
        return;
    }

    if(key == Qt::Key_Zenkaku_Hankaku){
        return;
    }

    //others
    this->keyPressEvent(key);
    return;
}

void MainWindow::keyPressEvent(int key){
    std::cout << "  resolve:" << key << std::endl;
    term->keyPressed(key);
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    //if (object == ui->scrollArea && event->type() == QEvent::KeyPress) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();
        if (key == Qt::Key_Zenkaku_Hankaku) {
            return false;
        } else{
            this->keyPressEvent(keyEvent);
            return true;
        }
    }
    return false;
}


