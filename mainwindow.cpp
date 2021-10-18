#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    term = new pterm::PseudoTerm();
    ui->setupUi(this);
    ui->label->installEventFilter(this);
    //connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onTextChanged()));
    //connect(ui->textEdit, SIGNAL(textChanged(QString)), ui->label, SLOT(setText(QString)));
    //ui->textEdit->append("a");
    //ui->label->setText(ui->textEdit->toPlainText());
}

MainWindow::~MainWindow()
{
    delete term;
    delete ui;
}

void MainWindow::onTextChanged(){
    //ui->label->setText(ui->textEdit->toPlainText());
}

void MainWindow::keyPressEvent(QKeyEvent* event){
    this->keyPressEvent(event->key());
}

void MainWindow::keyPressEvent(int key){
    term->keyPressed(key);
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    //if (object == ui->textEdit && event->type() == QEvent::KeyPress) {
    if (event->type() == QEvent::KeyPress) {
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
