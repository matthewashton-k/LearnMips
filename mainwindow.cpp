#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Interpreter.h"
#include "Box2D/Box2D.h"
#include <iostream>
#include <highligher.h>
#include <QStyleFactory>
#include <QIcon>
#include <QShortcut>
#include <QCloseEvent>
#include <QMessageBox>

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
    connect(ui->actionInstruction_Reference, &QAction::triggered, this, &MainWindow::displayReferenceWindow);

    // switching section stuff
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

    connect(modelPtr,
            &Model::deletePhysLabel,
            this,
            &MainWindow::deletePhysLabel);

    // set active tab to ensure ui is synced with model
    ui->sectionTabs->setCurrentIndex(0);
    ui->sectionTabs->setIconSize(QSize(12, 10));

    // set Box2D physics label container attribute to ignore mouse clicks
    //ui->physicsObjects->setAttribute(Qt::WA_TransparentForMouseEvents);

    //use Fusion style to allow for dark mode support on windows
    qApp->setStyle(QStyleFactory::create("Fusion"));

    //create hotkeys for save/load
    ui->actionLoad_All_Progress->setShortcut(Qt::Key_L | Qt::CTRL);
    ui->actionSave_All_Progress->setShortcut(Qt::Key_S | Qt::CTRL);

    //load progress on startup
    ui->actionLoad_All_Progress->trigger();


}

MainWindow::~MainWindow()
{
    delete ui;
    delete modelPtr;
    physObjLabels.clear();
}

void MainWindow::displayReferenceWindow(){
    QWidget* referenceWindow = new QWidget();
    QLabel* list = new QLabel(referenceWindow);
    QFont font("Arial", 12, 1, false);
    list->setWindowTitle("Instruction Reference");
    list->setFont(font);
    //TODO: finish writing all the text and descriptions
    list->setText(
R"(addi $dest, $source, immediate:
subi $dest, $source, immediate:
xori $dest, $source, immediate:
xor $dest, $source, $source:
add $dest, $source, $source:
sub $dest, $source, $source:
sll $dest, $source, immediate:
srl $dest, $source, immediate:
lw $dest, offset($source address):
sw $source, offset($source address):
lb $dest, offset($source address):
sb $source, offset($source address):
la $dest, label:
beq $source, $source, label:
bne $source, $source, label:
j label:
blt $source, $source, label:
bgt $source, $source, label:
syscall: uses the value in $v0 and $a0-3 to tell the system what to do)");

    list->show();
    referenceWindow->show();

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
    if(checked) ui->sectionTabs->setTabIcon(ID, QIcon(":/res/images/checkmark_50x50.png"));
    else ui->sectionTabs->setTabIcon(ID, QIcon());
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "LearnMips", tr("Save Progress?\n"), QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

    if (resBtn == QMessageBox::Yes) {
        ui->actionSave_All_Progress->trigger();
        event->accept();
    }
    else if(resBtn == QMessageBox::No) {
        event->accept();
    }
    else {
        event->ignore();
    }
}

void MainWindow::moveLabel(int id, int x, int y) {
    if(!physObjLabels.count(id)) {
        qDebug() << "ID doesn't exist. MainWindow::moveLabel";
        return;
    }
    // TODO [Box2D]: make a custom promoted QLabel that handles displaying physics objects
    try {
        QLabel* physObjLabel = physObjLabels.at(id);
        physObjLabel->move(x,y);
    } catch (std::out_of_range e) {
        qDebug() << "Invalid label ID: " << id << ". Cannot find label to move.";
    }
}

void MainWindow::createPhysLabel(int id, int x, int y) {
    if(physObjLabels.count(id)) {
        qDebug() << "ID already exists. MainWindow::createPhysLabel";
        return;
    }
    // TODO [Box2D]: make a custom promoted QLabel that handles displaying physics objects
    // TODO [Box2d]: destroy labels to prevent memory leaks
    QLabel* physObjLabel = new QLabel(ui->centralwidget);
    physObjLabels[id] = physObjLabel;
    physObjLabel->setPixmap(QPixmap(":/res/images/star_50x50.png"));
    //physObj->setText("physObject");
    physObjLabel->setGeometry(x, y, 50, 50);
    physObjLabel->show();
    // Might need to take in the id and save the label in a map here. otherwise should be
    // able to do ui->physicsObjects->findChildren
}

void MainWindow::deletePhysLabel(int id) {
    if(!physObjLabels.count(id)) {
        qDebug() << "ID doesn't exist. MainWindow::deletePhysLabel";
        return;
    }

    try {
        //QLabel* physObjLabel = physObjLabels.at(id);
        //physObjLabel->hide();
        delete physObjLabels[id];
        physObjLabels.erase(id);
    } catch (std::out_of_range e) {
        qDebug() << "Invalid label ID: " << id << ". Cannot find label to delete.";
    }
}
