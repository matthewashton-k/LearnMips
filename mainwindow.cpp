#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Interpreter.h"
#include "Box2D/Box2D.h"
#include <iostream>
#include <highligher.h>
using std::cout;
using std::endl;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) , modelPtr(new Model())
{
    ui->setupUi(this);

    Highlighter *highlighter = new Highlighter(ui->codeEdit->document());

    connect(ui->runButton, &QPushButton::clicked, this, &MainWindow::runButtonClicked);
    connect(ui->submitButton, &QPushButton::clicked, this, &MainWindow::submitButtonClicked);
    connect(ui->clearConsoleButton, &QPushButton::clicked, modelPtr, &Model::clearConsole);
    connect(modelPtr, &Model::consoleTextUpdated, this, &MainWindow::updateConsole);
    connect(modelPtr, &Model::progressCheckUpdated, this, &MainWindow::updateCheckBox);
    connect(this, &MainWindow::runRequest, modelPtr, &Model::executeCode);
    connect(ui->actionSave_All_Progress, &QAction::triggered, modelPtr, &Model::saveAllProgress);
    connect(ui->actionLoad_All_Progress, &QAction::triggered, modelPtr, &Model::loadAllProgress);

    //switching section stuff
    connect(ui->sectionTabs, &QTabWidget::currentChanged, modelPtr, &Model::changeSection);
    connect(modelPtr, &Model::requestSaveCurrentCode, this, &MainWindow::currentCodeRequested);
    connect(this, &MainWindow::answerCurrentCodeRequest, modelPtr, &Model::saveCodeToCurrentIndex);
    connect(modelPtr, &Model::codeUpdated, ui->codeEdit, &QPlainTextEdit::setPlainText);

    // Box2D
    connect(modelPtr,
            &Model::newPosition,
            this,
            &MainWindow::moveLabel // TODO [Box2D]: make a custom promoted QLabel that handles displaying physics objects
        );

    //set active tab to ensure ui is synced with model
    ui->sectionTabs->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete modelPtr;
}

void MainWindow::runButtonClicked(){
    emit runRequest(ui->codeEdit->toPlainText(), false);
}

void MainWindow::submitButtonClicked(){
    emit runRequest(ui->codeEdit->toPlainText(), true);
}

void MainWindow::currentCodeRequested(){
    emit answerCurrentCodeRequest(ui->codeEdit->toPlainText().toStdString());
}

void MainWindow::updateConsole(QString text){
    ui->console->setText(text);
}

void MainWindow::updateCheckBox(int ID, bool checked){
    switch (ID) {
    case 0:
        ui->checkBox_1->setChecked(checked);
        break;
    case 1:
        ui->checkBox_2->setChecked(checked);
        break;
    case 2:
        ui->checkBox_3->setChecked(checked);
        break;
    case 3:
        ui->checkBox_4->setChecked(checked);
        break;
    case 4:
        ui->checkBox_5->setChecked(checked);
        break;
    case 5:
        ui->checkBox_6->setChecked(checked);
        break;
    case 6:
        ui->checkBox_7->setChecked(checked);
        break;
    case 7:
        ui->checkBox_8->setChecked(checked);
        break;
    case 8:
        ui->checkBox_9->setChecked(checked);
        break;
    case 9:
        ui->checkBox_10->setChecked(checked);
        break;
    case 10:
        ui->checkBox_11->setChecked(checked);
        break;
    case 11:
        ui->checkBox_12->setChecked(checked);
        break;
    default:
        break;
    }
}

void MainWindow::moveLabel(int x, int y) {
    // TODO [Box2D]: make a custom promoted QLabel that handles displaying physics objects
    ui->physObj->move(x,y);
}
