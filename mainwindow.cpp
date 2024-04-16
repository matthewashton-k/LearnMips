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

    //switching section stuff
    connect(ui->sectionTabs, &QTabWidget::currentChanged, modelPtr, &Model::changeSection);
    connect(modelPtr, &Model::requestSaveCurrentCode, this, &MainWindow::currentCodeRequested);
    connect(this, &MainWindow::answerCurrentCodeRequest, modelPtr, &Model::saveCodeToCurrentIndex);
    connect(modelPtr, &Model::codeUpdated, ui->codeEdit, &QPlainTextEdit::setPlainText);

    // Box2D
    connect(modelPtr,
            &Model::newPosition,
            this,
            &MainWindow::moveLabel);

    connect(modelPtr,
            &Model::newPhysObj,
            this,
            &MainWindow::createPhysLabel);

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

void MainWindow::moveLabel(int id, int x, int y) {
    // TODO [Box2D]: make a custom promoted QLabel that handles displaying physics objects
    //qDebug() << id << ": (" << x << ", " << y << ")";
    QLabel* physObjLabel = ui->physicsObjects->findChildren<QLabel*>().at(id);
    physObjLabel->move(x,y);
    //qDebug() << id << ": " << physObjLabel->pos();
}

void MainWindow::createPhysLabel(int x, int y) {
    // TODO [Box2D]: make a custom promoted QLabel that handles displaying physics objects
    // TODO [Box2d]: destroy labels to prevent memory leaks
    QLabel* physObj = new QLabel(ui->physicsObjects);
    physObj->setText("physObject");
    physObj->setGeometry(x, y, 49, 16);
    physObj->show();
    // Might need to take in the id and save the label in a map here. otherwise should be
    // able to do ui->physicsObjects->findChildren

}
