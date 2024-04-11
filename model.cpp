#include "model.h"
#include <vector>
#include "validityFunctions.h"
#include <iostream>

Model::Model(QObject *parent) : QObject{parent} {
    for(int i = 0; i < NUM_OF_SECTIONS; i++) sections->push_back(buildSection(i)); //create each section

    codeStrings = new std::string[NUM_OF_SECTIONS];
    for(int i = 0; i < NUM_OF_SECTIONS; i++) codeStrings[i] = "";
}

Model::~Model(){
    delete sections;
    delete[] codeStrings;
}

void Model::changeSection(int index){
    nextSection = index;
    emit requestSaveCurrentCode();
    std::cout << "save requested" << std::endl;
}

void Model::finalizeSectionChange(){
    currSection = nextSection;
    nextSection = 0;
    QString str = codeStrings[currSection].c_str();
    std::cout << str.toStdString() << std::endl;
    emit codeUpdated(str);
}

void Model::saveCodeToCurrentIndex(std::string code){
    codeStrings[currSection] = code;
    finalizeSectionChange();
}










//logic for building each section with proper validity function
Section Model::buildSection(int sectionID){
    switch(sectionID){
    case 1:
        return Section();
    case 2:
        return Section(
            Challenge(
                "", //before Code
                "", //after Code
                ValidityFunctions::section2ValidityFunction));
    case 3:
        return Section(
            Challenge(
                "", //before Code
                "", //after Code
                ValidityFunctions::section3ValidityFunction));
    case 4:
        return Section(
            Challenge(
                "", //before Code
                "", //after Code
                ValidityFunctions::section4ValidityFunction));
    case 5:
        return Section(
            Challenge(
                "", //before Code
                "", //after Code
                ValidityFunctions::section5ValidityFunction));
    case 6:
        return Section(
            Challenge(
                "", //before Code
                "", //after Code
                ValidityFunctions::section6ValidityFunction));
    case 7:
        return Section(
            Challenge(
                "", //before Code
                "", //after Code
                ValidityFunctions::section7ValidityFunction));
    case 8:
        return Section(
            Challenge(
                "", //before Code
                "", //after Code
                ValidityFunctions::section8ValidityFunction));
    case 9:
        return Section(
            Challenge(
                "", //before Code
                "", //after Code
                ValidityFunctions::section9ValidityFunction));
    case 10:
        return Section(
            Challenge(
                "", //before Code
                "", //after Code
                ValidityFunctions::section10ValidityFunction));
    case 11:
        return Section(
            Challenge(
            "", //before Code
            "", //after Code
            ValidityFunctions::section11ValidityFunction));
    case 12:
        return Section();
    default:
        return Section();
    }
}


