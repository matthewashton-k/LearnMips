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
        ui->console->setText(QString::fromStdString(interpreter.run()));
    } catch (std::string err) {
        cout << "error";
        cout << err << endl;
    }
    //END TEMPORARY TEST!
}
