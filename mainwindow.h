#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "model.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Model* modelPtr;
    /**
     * @brief physObjBodies Stores physics QLabels with their id as the key
     */
    std::map<int, QLabel*> physObjLabels;
    void closeEvent (QCloseEvent *event); //override of close event

signals:
    void answerCurrentCodeRequest(std::string);
    void runRequest(QString code, bool checkSolutionValidity);

private slots:
    void runButtonClicked();
    void submitButtonClicked();
    void currentCodeRequested();
    /**
     * @brief moveLabel Moves the physObj label with the specified id to the given x y relative to the window
     * @param id of the physicsObj label to move
     * @param x the horizontal component in pixels
     * @param y the vertical component in pixels
     */
    void moveLabel(int id, int x, int y);
    /**
     * @brief createPhysLabel creates a QLabel with physics at the given x y coordinate
     * @param x the horizontal component in pixels
     * @param y the vertical component in pixels
     */
    void createPhysLabel(int id, int x, int y);
    /**
     * @brief deletePhysLabel Deletes the physics label with the given id.
     * @param id of the physicsObj to be deleted.
     */
    void deletePhysLabel(int id);

    void updateConsole(QString text);
    void updateCheckBox(int ID, bool checked);
    void displayReferenceWindow();

};
#endif // MAINWINDOW_H
