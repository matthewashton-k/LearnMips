/**
 * Description: cpp file for the Main Window of the Learn Mips application
 * Authors: Matthew Ashton Knochel, Carter Dean, Abdulla Alnuaimi, Logan Luker, Aidan Sheehan
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
// #include "Box2D/Box2D.h"
#include <highligher.h>
#include "tabbutton.h"
#include <QStyleFactory>
#include <QIcon>
#include <QShortcut>
#include <QCloseEvent>
#include <QMessageBox>

using std::cout;
using std::endl;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) , modelPtr(new Model()), tabButton(new TabButton())
{
    ui->setupUi(this);

    highlighter = new Highlighter(ui->codeEdit->document());

    //connect ui
    connect(ui->runButton, &QPushButton::clicked, this, &MainWindow::runButtonClicked);
    connect(ui->submitButton, &QPushButton::clicked, this, &MainWindow::submitButtonClicked);
    connect(ui->clearConsoleButton, &QPushButton::clicked, modelPtr, &Model::clearConsole);
    connect(ui->showHideMenuButton, &QPushButton::clicked, this, &MainWindow::showHideMenuButtonClicked);
    connect(ui->nextButton, &QPushButton::clicked, this, &MainWindow::nextButtonClicked);
    connect(ui->previousButton, &QPushButton::clicked, this, &MainWindow::previousButtonClicked);
    connect(ui->pageWidget, &QStackedWidget::currentChanged, this, &MainWindow::currentIndexChanged);

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

    //connecting all of the tab buttons
    connect(ui->tb10_1, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate); //welcome page
    connect(ui->tb1_1, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb1_2, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb1_3, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb1_4, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb2_1, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb2_2, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb2_3, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb3_1, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb3_2, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb3_3, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb4_1, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb4_2, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb4_3, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb5_1, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb5_2, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb5_3, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb6_1, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb6_2, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb6_3, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb7_1, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb7_2, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb7_3, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb8_1, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb8_2, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    //connect(ui->tb8_3, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb9_1, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb9_2, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb9_3, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);
    connect(ui->tb12_1, &TabButton::tabButtonClicked, this, &MainWindow::handleTabUpdate);


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
    ui->pageWidget->setCurrentIndex(1);
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
    delete tabButton;
    delete highlighter;
    physObjLabels.clear();
}

void MainWindow::handleTabUpdate(std::tuple<int,int> tabID) {
    if(tabID == std::tuple<int,int>{10,1})
        ui->pageWidget->setCurrentIndex(1);
    if(std::get<0>(tabID) == 0)
        tabID = std::tuple<int,int>{10,1};
    int row = std::get<0>(tabID);
    int column = std::get<1>(tabID);
    ui->sectionTabs->setCurrentIndex(row-1);
    QTabWidget *nestedTab = ui->sectionTabs->findChild<QTabWidget *>(QString::fromStdString("s" + std::to_string(row) + "Widget"));
    nestedTab->setCurrentIndex(column-1);
    std::string output = "page" + std::to_string(row) + "_" + std::to_string(column);
    //std::cout << output << std::endl;
    QWidget *currentPage = ui->pageWidget->findChild<QWidget *>(QString::fromStdString("page" + std::to_string(row) + "_" + std::to_string(column)));
    if(std::get<0>(tabID) != 10)
        ui->pageWidget->setCurrentWidget(currentPage);
}

void MainWindow::nextButtonClicked(){
    ui->pageWidget->setCurrentIndex(ui->pageWidget->currentIndex() + 1);

    std::string name = ui->pageWidget->currentWidget()->objectName().toStdString();
    std::tuple<int,int> tabID = std::tuple<int, int>{std::stoi(name.substr(4, name.find("_")-4)), std::stoi(name.substr(name.find("_")+1))};
    handleTabUpdate(tabID);
}

void MainWindow::previousButtonClicked(){
    ui->pageWidget->setCurrentIndex(ui->pageWidget->currentIndex() - 1);

    std::string name = ui->pageWidget->currentWidget()->objectName().toStdString();
    std::tuple<int,int> tabID = std::tuple<int, int>{std::stoi(name.substr(4, name.find("_")-4)), std::stoi(name.substr(name.find("_")+1))};
    handleTabUpdate(tabID);
}

void MainWindow::currentIndexChanged(){
    switch (ui->pageWidget->currentIndex()) {
    case 5:
        ui->codingGroupBox->setEnabled(true);
        ui->codingGroupBox->show();
        doAnimation(ui->codingGroupBox, 300, QRect(620,740,81,21), QRect(420,340,601,171));
        break;
    case 8:
        ui->codingGroupBox->setEnabled(true);
        ui->codingGroupBox->show();
        doAnimation(ui->codingGroupBox, 300, QRect(620,740,81,21), QRect(660,0,650,670));
        break;
    case 11:
        ui->codingGroupBox->setEnabled(true);
        ui->codingGroupBox->show();
        doAnimation(ui->codingGroupBox, 300, QRect(620,740,81,21), QRect(660,0,650,670));
        break;
    case 14:
        ui->codingGroupBox->setEnabled(true);
        ui->codingGroupBox->show();
        doAnimation(ui->codingGroupBox, 300, QRect(620,740,81,21), QRect(660,0,650,670));
        break;
    case 17:
        ui->codingGroupBox->setEnabled(true);
        ui->codingGroupBox->show();
        doAnimation(ui->codingGroupBox, 300, QRect(620,740,81,21), QRect(660,0,650,670));
        break;
    case 20:
        ui->codingGroupBox->setEnabled(true);
        ui->codingGroupBox->show();
        doAnimation(ui->codingGroupBox, 300, QRect(620,740,81,21), QRect(660,0,650,670));
        break;
    case 23:
        ui->codingGroupBox->setEnabled(true);
        ui->codingGroupBox->show();
        doAnimation(ui->codingGroupBox, 300, QRect(620,740,81,21), QRect(660,0,650,670));
        break;
    case 25:
        ui->codingGroupBox->setEnabled(true);
        ui->codingGroupBox->show();
        doAnimation(ui->codingGroupBox, 300, QRect(620,740,81,21), QRect(660,0,650,670));
        break;
    case 28:
        ui->codingGroupBox->setEnabled(true);
        ui->codingGroupBox->show();
        doAnimation(ui->codingGroupBox, 300, QRect(620,740,81,21), QRect(660,0,650,670));
        break;
    case 29:
        ui->codingGroupBox->setEnabled(true);
        ui->codingGroupBox->show();
        doAnimation(ui->codingGroupBox, 300, QRect(620,740,81,21), QRect(660,0,650,670));
        break;
    default:
        doAnimation(ui->codingGroupBox, 300, ui->codingGroupBox->geometry(), QRect(620,740,81,21));
        ui->codingGroupBox->setEnabled(false);
        QTimer::singleShot(300, this, &MainWindow::hideCodeBox);
        break;
    }
    if(ui->pageWidget->currentIndex() == 29)
        ui->nextButton->setEnabled(false);
    else
        ui->nextButton->setEnabled(true);

    if(ui->pageWidget->currentIndex() == 1)
        ui->previousButton->setEnabled(false);
    else
        ui->previousButton->setEnabled(true);

    fixCurrentPage();
}

void MainWindow::doAnimation(QObject *target, int duration, QRect start, QRect end){
    animation.setTargetObject(target);
    animation.setPropertyName("geometry");
    animation.setDuration(duration);
    animation.setStartValue(start);
    animation.setEndValue(end);
    animation.start();
}

void MainWindow::hideCodeBox(){
    if(!ui->codingGroupBox->isEnabled())
        ui->codingGroupBox->hide();
}

/**
 * @brief fixCurrentPage helper method to make sure the menu is on the right page
 */
void MainWindow::fixCurrentPage() {
    std::string name = ui->pageWidget->currentWidget()->objectName().toStdString();
    int currentPage = std::stoi(name.substr(4, name.find("_")-4));
    if(currentPage == 12)
        currentPage = 10;
    if(ui->lessonMenu->currentIndex() != currentPage)
        ui->lessonMenu->setCurrentIndex(currentPage);
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
jal label:
jr $source:
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

void MainWindow::showHideMenuButtonClicked(){
    animation.setTargetObject(ui->lessonMenu);
    animation2.setTargetObject(ui->showHideMenuButton);
    animation3.setTargetObject(ui->menuLine);
    //animation4.setTargetObject(ui->pageWidget);
    animation.setPropertyName("geometry");
    animation2.setPropertyName("geometry");
    animation3.setPropertyName("geometry");
    //animation4.setPropertyName("geometry");
    animation.setDuration(200);
    animation2.setDuration(200);
    animation3.setDuration(200);
    //animation4.setDuration(200);
    if(ui->lessonMenu->isEnabled())
    {

        animation.setEndValue(QRect(0, 0, 0, 761));
        animation2.setEndValue(QRect(0, 370, 31, 51));
        animation3.setEndValue(QRect(0, 30, 21, 731));
        //animation4.setEndValue(QRect(31, 0, 1081, 671));
        //animation.setEasingCurve(QEasingCurve::OutBounce);

        ui->lessonMenu->setDisabled(true);
        ui->menuLine->hide();
        QPixmap pixmap(":/res/images/chevron-right-icon.png");
        QIcon icon(pixmap);
        ui->showHideMenuButton->setIcon(icon);
    }
    else
    {
        animation.setEndValue(QRect(0, 0, 211, 761));
        animation2.setEndValue(QRect(210, 370, 31, 51));
        animation3.setEndValue(QRect(200, 30, 21, 731));
        //animation4.setEndValue(QRect(240, 0, 1081, 671));
        //animation.setEasingCurve(QEasingCurve::OutBounce);

        ui->lessonMenu->setDisabled(false);
        ui->menuLine->show();
        QPixmap pixmap(":/res/images/chevron-left-icon.png");
        QIcon icon(pixmap);
        ui->showHideMenuButton->setIcon(icon);
    }
    animation.start();
    animation2.start();
    animation3.start();
    //animation4.start();

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
    if(checked){
        ui->sectionTabs->setTabIcon(ID, QIcon(":/res/images/checkmark_50x50.png"));
        ui->lessonMenu->setItemIcon(ID + 1, QIcon(":/res/images/checkmark_rotated_50x50.png"));
    }
    else {
        ui->sectionTabs->setTabIcon(ID, QIcon());
        ui->lessonMenu->setItemIcon(ID + 1, QIcon());
    }
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
