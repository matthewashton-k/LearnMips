#include "model.h"
#include <vector>

Model::Model(QObject *parent) : QObject{parent} {
    sections->push_back(buildSection(1));
}

Model::~Model(){
    delete sections;
}

Section Model::buildSection(int){
    return Section();
}

void Model::changeSection(int index){

}
