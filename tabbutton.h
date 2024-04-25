#ifndef TABBUTTON_H
#define TABBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <iostream>

class TabButton : public QPushButton
{
    Q_OBJECT
public:
    explicit TabButton(QWidget *parent = nullptr);

signals:
    void tabButtonClicked(std::tuple<int, int> id);

private slots:
    void handleClick();

private:
    //std::tuple<int,int> getIdFromObjectName();
};

#endif // TABBUTTON_H
