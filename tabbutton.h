#ifndef TABBUTTON_H
#define TABBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <iostream>

class TabButton : public QPushButton
{
    Q_OBJECT
private:

public:
    TabButton(QWidget *parent = nullptr);

signals:
    void updateTab(std::tuple<int,int> tabID);

public slots:
    void tabButtonClicked();
};

#endif // TABBUTTON_H
