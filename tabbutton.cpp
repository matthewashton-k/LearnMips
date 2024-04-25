#include "tabbutton.h"

TabButton::TabButton(QWidget *parent)
    : QPushButton{parent}
{
    connect(this, &QPushButton::clicked, this, &TabButton::handleClick);
}

void TabButton::handleClick() {
    std::string name = this->objectName().toStdString();
    std::tuple<int,int> id = std::tuple<int, int>{std::stoi(name.substr(2, name.find("_")-2)), std::stoi(name.substr(name.find("_")+1))};
    emit tabButtonClicked(id);
}
