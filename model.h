#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <vector>
#include "section.h"

class Model : public QObject
{
    Q_OBJECT
private:
    int NUM_OF_SECTIONS = 12;

    std::string* codeStrings;
    //vector to store all the sections
    std::vector<Section>* sections = new std::vector<Section>();

    //functions to build each section
    Section buildSection(int sectionID);

    int currSection = 0;
    int nextSection = 0;


    void finalizeSectionChange();
public:
    Model(QObject *parent = nullptr);
    ~Model();

signals:
    void requestSaveCurrentCode();
    void codeFinishedSaving();
    void codeUpdated(QString newCode);
public slots:
    void changeSection(int index);
    void saveCodeToCurrentIndex(std::string code);
};

#endif // MODEL_H
