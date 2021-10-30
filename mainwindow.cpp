#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "keyutil.h"
#include <iostream>
#include <QTimer>
#include <sys/fcntl.h>
#include <stdio.h>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    ui->setupUi(this);
    ui->centralwidget->installEventFilter(this);
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
    this->setWindowTitle(term->getTname());
    if((pip=term->getPipe())==-1){
        perror("open");
        exit(1);
   }
}

void MainWindow::timer(){
    unsigned char buffer[128];
    int nread;
    unsigned char header;
    std::basic_string<uchar> buf;
    if(!term->isPipeExists()){
        exit(0);
    }
    while((::read(pip, &header, 1)) > 0){
        if((nread = ::read(pip, buffer, header)) != header)
            exit(1);
        buffer[nread] = 0;
        log.append(buffer);
        buf.append(buffer);
    }

        for(unsigned long i = 0; i < buf.length(); i++){
            switch(unsigned char c = buf.at(i)) {
            case 0x07:
                //ring a bell
                bell = BELL;
                continue;
            case 0x08:
                //BS
                if(i+2 < buf.length() &&  buf.at(i+1) == 0x20 && buf.at(i+2) == 0x08){
                    windowBuffer.getCurrent()->erase(windowBuffer.getCurrent()->letterLength() - 1 + windowBuffer.getOffset());
                    i+=2;
                } else{
                    windowBuffer.offsetBack();
                }
                continue;
            case 0x0a:
                //LF
                windowBuffer.LF();
                //windowBuffer.getCurrent()->push(c);
                continue;
            case 0x0d:
                //CR
                windowBuffer.CR();
                continue;
            case 0x1b:
                //ESC
                //parseEscapeSequence(buf, &i);
                continue;
            default:
                append(buf, &i);
                continue;
            }
        }
    update();
}

/*
void MainWindow::parseEscapeSequence(std::basic_string<uchar> input, unsigned long *i){
    //input ends with ESC
    if(*i + 1 > input.length()){
        if(offset < 0){
            str.replace(str.length() + offset, 1, 1, input.at(*i));
            offset++;
        } else{
            str += input.at(*i);
            col++;
        }
    }

    //
    switch(input.at(*i += 1)){
    case 'n':
       //Single Shift Two
    return;
    case 'o':
       //Single Shift Three
    return;
    case 'p':
       //Device Control String
    return;
    case 0x5c:
       //String Terminator
    return;
    case ']':
       //Operation System Command
    return;
    case 'x':
       //Start of String
    return;
    case '^':
       //Privacy Message
    return;
    case '_':
       //Application Program Command
    return;

    case '[':
        //very zatu statement
        if(*i + 1 > input.length()){
            if(offset < 0){
                str.replace(str.length() + offset, 1, 1, 0x07);
                offset++;
            } else{
                str += 0x07;
                col++;
            }
            return;
        }else{
            int arg1;
            unsigned char tmp;
            for(arg1 = 0; *i + 1 > input.length();){
                tmp = input.at(*i += 1);
                if(tmp >= 0x30 && tmp <= 0x39){
                    arg1 += tmp - 0x30;
                } else{
                    break;
                }
            }
            switch(tmp){
            case 'A':
                //cursor up
                return;
            case 'B':
                //cursor down
                return;
            case 'C':
                //cursor f
                return;
            case 'D':
                //cursor b
                return;
            case 'E':
                //cursor nl
                return;
            case 'F':
                //cursor prev line
                return;
            case 'G':
                //Moves the cursor to column n (default 1).
                //資料少なくてよくわからなかった
                return;
            case 'J':
                //Erase in Display
                return;
            case 'K':
                //Erase in Line
                return;
            case 'S':
                //Scroll Up
                return;
            case 'T':
                //Scroll Down
                return;
            case 'm':
                //Select Graphic Rendition
                //色設定
                //セミコロンが使用できるという説も見つけたものの、とりあえず放置
                return;
            case ';':
                if(*i + 1 > input.length()){
                    if(offset < 0){
                        str.replace(str.length() + offset, 1, 1, 0x07);
                        offset++;
                    } else{
                        str += 0x07;
                        col++;
                    }
                    return;
                }else{
                    int arg2;
                    unsigned char tmp2;
                    for(arg2 = 0; *i + 1 > input.length();){
                        tmp2 = input.at(*i += 1);
                        if(tmp2 >= 0x30 && tmp2 <= 0x39){
                            arg2 += tmp2 - 0x30;
                        } else{
                            break;
                        }
                    }
                    switch(tmp2){
                    case 'H':
                        //Cursor Position
                        return;
                    case 'f':
                        //Horizontal Vertical Position
                        return;
                    default:
                        if(offset < 0){
                            str.replace(str.length() + offset, 1, 1, 0x07);
                            offset++;
                        } else{
                            str += 0x07;
                            col++;
                        }
                        return;
                    }
                }
                return;
            default:
                if(offset < 0){
                    str.replace(str.length() + offset, 1, 1, 0x07);
                    offset++;
                } else{
                    str += 0x07;
                    col++;
                }
                return;
            }
        }
    return;

    default:
       //Single Shift Two
        if(offset < 0){
            str.replace(str.length() + offset, 1, 1, 0x07);
            offset++;
        } else{
            str += 0x07;
            col++;
        }
    return;
    }
}
*/

void MainWindow::append(std::basic_string<uchar> input, unsigned long *index){
    if(windowBuffer.getOffset() < 0){
        windowBuffer.getCurrent()->replace(windowBuffer.getCursorX(), input, index);
        windowBuffer.offsetFront();
    } else{
        windowBuffer.getCurrent()->push(input, index);
        windowBuffer.incCol();
    }
}

void MainWindow::keyPressEvent(QKeyEvent* event){
    int key = event->key();
    std::cout << "  press:" << key << std::endl;

    if(QApplication::queryKeyboardModifiers() & Qt::ControlModifier){
        if(key >= Qt::Key_At && key <= Qt::Key_Underscore){
            this->keyPress(key - Qt::Key_At);
            return;
        } else if(key == Qt::Key_Question){
            this->keyPress(127);
            return;
        }
    }

    if(QApplication::queryKeyboardModifiers() & Qt::ShiftModifier){
        if(key >= Qt::Key_A && key <= Qt::Key_Z){
            this->keyPress(key);
            return;
        }
        if(key == Qt::Key_At){
            this->keyPress(0x60);
            return;
        }
    }

    if(key >= Qt::Key_A && key <= Qt::Key_Z){
        this->keyPress(::tolower(key));
        return;
    }

    if(key == Qt::Key_At){
        this->keyPress(key);
        return;
    }
    if(key == Qt::Key_Space){
        this->keyPress(0x20);
        return;
    }

    if(key == Qt::Key_Backspace){
        this->keyPress(0x08);
        return;
    }

    if(key == Qt::Key_Delete){
        //ctrl+dだと空欄時にexitしてしまう
        //xtermの挙動(bell)とは異なる
        this->keyPress(0x04);
        return;
    }

    if(key == Qt::Key_Return){
        this->keyPress(0x0d);
        return;
    }

    if(key == Qt::Key_Escape){
        this->keyPress(0x1B);
        return;
    }

    if(key == Qt::Key_Left){
        this->keyPress(0x02);
        return;
    }

    if(key == Qt::Key_Right){
        this->keyPress(0x06);
        return;
    }

    if(key == Qt::Key_Up){
        this->keyPress(0x10);
        return;
    }

    if(key == Qt::Key_Down){
        this->keyPress(0x0E);
        return;
    }

    if(key == Qt::Key_Home){
        this->keyPress(0x01);
        return;
    }

    if(key == Qt::Key_End){
        this->keyPress(0x05);
        return;
    }

    //modifier
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
    this->keyPress(key);
    return;
}

void MainWindow::keyPress(int key){
    //std::cout << "  resolve:" << key << std::endl;
    term->keyPressed(key);
}

bool MainWindow::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();
        if (key == Qt::Key_Zenkaku_Hankaku || key == Qt::Key_PageUp || key == Qt::Key_PageDown) {
            //ignore
            return false;
        } else{
            this->keyPressEvent(keyEvent);
            return true;
        }
    }
    return false;
}

void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QFont font = painter.font();
    font.setFamily("Segoe UI");
    //fixed pitch を使えば固定長になるらしい
    font.setPixelSize(16);
    font.setFixedPitch(true);
    painter.setFont(font);
    QFontMetrics metrics = QFontMetrics(font);

    if(bell <= 0){
        //painter.setPen(QPen(QColor(0, 0, 0, 192), 20, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        painter.setBrush(QBrush(QColor(0, 0, 0, 192), Qt::SolidPattern));
        painter.drawRect(0,0, 800, 600);
        painter.setPen(QPen(Qt::white, 0, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        painter.drawLine((windowBuffer.getCursorX()) * 8, windowBuffer.getCursorY() * 16, (windowBuffer.getCursorX()) * 8, (windowBuffer.getCursorY() + 1) * 16);


        QString qstr;
        for(int i = 0; i <= windowBuffer.getRow(); i++){
            qstr =  windowBuffer.at(i)->q_str();
            painter.drawText(QPoint(0, (metrics.height() - metrics.descent()) * (i+1)), qstr);
        }
    } else{
        painter.setBrush(QBrush(QColor(0, 0, 0, 255), Qt::SolidPattern));
        painter.drawRect(0, 0, 800, 600);
        bell--;
    }
}


