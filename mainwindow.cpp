#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Interpreter.h"
#include <iostream>
using std::cout;
using std::endl;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) , modelPtr(new Model())
{
    ui->setupUi(this);
    connect(ui->runButton, &QPushButton::clicked, this, &MainWindow::runButtonClicked);

    //switching section stuff
    connect(ui->sectionTabs, &QTabWidget::currentChanged, modelPtr, &Model::changeSection);
    connect(modelPtr, &Model::requestSaveCurrentCode, this, &MainWindow::currentCodeRequested);
    connect(this, &MainWindow::answerCurrentCodeRequest, modelPtr, &Model::saveCodeToCurrentIndex);
    connect(modelPtr, &Model::codeUpdated, ui->codeEdit, &QTextEdit::setText);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete modelPtr;
}

void MainWindow::runButtonClicked(){
    //TEMPORARY TEST!
    std::string instructions = ui->codeEdit->toPlainText().toStdString();

    Interpreter interpreter(instructions);
    try {
        QString str = interpreter.run().c_str();
        ui->console->setText(str);
    } catch (std::string err) {
        ui->console->setText(QString(err.c_str()));
        cout << "Error: " << err << endl;
    }
    //END TEMPORARY TEST!
}

void MainWindow::currentCodeRequested(){
    emit answerCurrentCodeRequest(ui->codeEdit->toPlainText().toStdString());
}
