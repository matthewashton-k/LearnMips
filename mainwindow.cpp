#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Interpreter.h"
#include <iostream>
using std::cout;
using std::endl;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->runButton, &QPushButton::clicked, this, &MainWindow::runButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
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
