/**
 * Description: cpp file for the Main Window of the Learn Mips application
 * Authors: Matthew Ashton Knochel, Carter Dean, Abdulla Alnuaimi, Logan Luker, Aidan Sheehan
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
// #include "Box2D/Box2D.h"
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

    highlighter = new Highlighter(ui->codeEdit->document());

    //connect ui
    connect(ui->runButton, &QPushButton::clicked, this, &MainWindow::runButtonClicked);
    connect(ui->submitButton, &QPushButton::clicked, this, &MainWindow::submitButtonClicked);
    connect(ui->clearConsoleButton, &QPushButton::clicked, modelPtr, &Model::clearConsole);

    //connect model/main window communication signals/slots
    connect(modelPtr, &Model::consoleTextUpdated, this, &MainWindow::updateConsole);
    connect(modelPtr, &Model::progressCheckUpdated, this, &MainWindow::updateCheckBox);
    connect(this, &MainWindow::runRequest, modelPtr, &Model::executeCode);

    //connect save/load actions
    connect(ui->actionSave_All_Progress, &QAction::triggered, modelPtr, &Model::saveAllProgress);
    connect(ui->actionLoad_All_Progress, &QAction::triggered, modelPtr, &Model::loadAllProgress);
    connect(ui->actionInstruction_Reference, &QAction::triggered, this, &MainWindow::displayReferenceWindow);
    connect(ui->actionSyscall_Op_Code_Reference, &QAction::triggered, this, &MainWindow::displayOpCodeReferenceWindow);

    // switching section stuff
    connect(ui->sectionTabs, &QTabWidget::currentChanged, modelPtr, &Model::changeSection);
    connect(modelPtr, &Model::requestSaveCurrentCode, this, &MainWindow::currentCodeRequested);
    connect(this, &MainWindow::answerCurrentCodeRequest, modelPtr, &Model::saveCodeToCurrentIndex);
    connect(modelPtr, &Model::codeUpdated, ui->codeEdit, &QPlainTextEdit::setPlainText);
    connect(modelPtr, &Model::makeTabVisible, this, &MainWindow::setSectionTabVisible);
    connect(this, &MainWindow::requestTabVisibilities, modelPtr, &Model::pushTabVisibilities);

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

    //hide all tabs
    for(int i = 0; i < 12; i++){
        ui->sectionTabs->setTabVisible(i, false);
    }

    //load progress on startup
    ui->actionLoad_All_Progress->trigger();

    //get starting tab visibilities
    refreshTabVisibilities();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete modelPtr;
    delete highlighter;
    physObjLabels.clear();
}

void MainWindow::displayReferenceWindow(){
    //set up window
    QWidget* referenceWindow = new QWidget();
    QLabel* list = new QLabel(referenceWindow);
    QFont font("Arial", 12, 1, false);
    list->setWindowTitle("Instruction Reference");
    list->setFont(font);

    //the text to display
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

    //show window and text label
    list->show();
    referenceWindow->show();

}

void MainWindow::displayOpCodeReferenceWindow(){
    //set up window
    QWidget* referenceWindow = new QWidget();
    QLabel* list = new QLabel(referenceWindow);
    QFont font("Arial", 12, 1, false);
    list->setWindowTitle("Op Code Reference");
    list->setFont(font);

    //the text to display
    list->setText(
        R"($v0 should contain the code when the syscall is made

1: Print Integer ($a0 should countain the integer to print)
4: Print String ($a0 should contain the address to the string
to print. Best achieved by using a label)
10: Exit (Exits the program)
11: Print Character ($a0 should contain the character to print))");

    //show window and text label
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

void MainWindow::refreshTabVisibilities(){
    emit requestTabVisibilities();
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
    //create message box
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "LearnMips", tr("Save Progress?\n"), QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

    //check message box output
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

void MainWindow::setSectionTabVisible(int ID, bool state){
    ui->sectionTabs->setTabVisible(ID, state);
}

void MainWindow::moveLabel(int id, int x, int y) {
    if(!physObjLabels.count(id)) {
        qDebug() << "ID doesn't exist. MainWindow::moveLabel";
        return;
    }

    try {
        QLabel* physObjLabel = physObjLabels.at(id);
        physObjLabel->move(x,y);
    } catch (std::out_of_range) {
        qDebug() << "Invalid label ID: " << id << ". Cannot find label to move.";
    }
}

void MainWindow::createPhysLabel(int id, int x, int y) {
    if(physObjLabels.count(id)) {
        qDebug() << "ID already exists. MainWindow::createPhysLabel";
        return;
    }

    QLabel* physObjLabel = new QLabel(ui->centralwidget);
    physObjLabels[id] = physObjLabel;
    physObjLabel->setPixmap(QPixmap(":/res/images/star_50x50.png"));
    physObjLabel->setGeometry(x, y, 50, 50);
    physObjLabel->show();
}

void MainWindow::deletePhysLabel(int id) {
    if(!physObjLabels.count(id)) {
        qDebug() << "ID doesn't exist. MainWindow::deletePhysLabel";
        return;
    }

    try {
        delete physObjLabels[id];
        physObjLabels.erase(id);
    } catch (std::out_of_range) {
        qDebug() << "Invalid label ID: " << id << ". Cannot find label to delete.";
    }
}
