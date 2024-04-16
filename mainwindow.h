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

private slots:
    void runButtonClicked();
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

};
#endif // MAINWINDOW_H
