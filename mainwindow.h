#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer> // TODO [Box2D]: move to model
#include "model.h"
#include "Box2D/Box2D.h"


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

    // TODO [Box2D]: Remove later, just for testing
    b2Vec2 gravity = b2Vec2(0.0f, 10.0f);
    b2World world = b2World(gravity);
    b2Body* physObjBody;
    QTimer timer;

private:
    Ui::MainWindow *ui;
    Model* modelPtr;

signals:
    void answerCurrentCodeRequest(std::string);

private slots:
    void runButtonClicked();
    void currentCodeRequested();
    void setupWorld();

private slots:
    void updateWorld();
    void moveLabel(int x, int y);

signals:
    void newPosition(int x, int y);
};
#endif // MAINWINDOW_H
