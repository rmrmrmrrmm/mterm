#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    term = new pterm::PseudoTerm();

    ui->setupUi(this);
    installEventFilter(this);
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
    term->keyPressed(event->key());
}

