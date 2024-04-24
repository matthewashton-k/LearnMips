#include "tabbutton.h"

TabButton::TabButton(QWidget *parent)
    : QPushButton{parent}
{
    connect(this, &TabButton::clicked, this, &TabButton::tabButtonClicked);
}

void TabButton::tabButtonClicked() {
    std::string name = this->objectName().toStdString();
    std::tuple<int,int> id = std::tuple<int, int>{std::stoi(name.substr(2, name.find("_")-2)), std::stoi(name.substr(name.find("_")+1))};
    emit updateTab(id);
    std::cout << "tabButtonClicked" << std::get<0>(id) << "," << std::get<1>(id) << std::endl;
}
