#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

signals:
    void answerCurrentCodeRequest(std::string);
    void runRequest(QString code, bool checkSolutionValidity);

private slots:
    void runButtonClicked();
    void submitButtonClicked();
    void currentCodeRequested();
    /**
     * @brief moveLabel Moves the physObj label to the given x y relative to the window
     * @param x
     * @param y
     */
    void moveLabel(int id, int x, int y);
    /**
     * @brief createPhysLabel creates a QLabel with physics at the given x y coordinate
     * @param x
     * @param y
     */
    void createPhysLabel(int x, int y);

    void updateConsole(QString text);
    void updateCheckBox(int ID, bool checked);
    void displayReferenceWindow();

};
#endif // MAINWINDOW_H
