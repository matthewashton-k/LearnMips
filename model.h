#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <vector>
#include "section.h"

class Model : public QObject
{
    Q_OBJECT
private:
    std::vector<std::string> codes();
    //vector to store all the sections
    std::vector<Section>* sections = new std::vector<Section>();

    //functions to build each section
    Section buildSection(int sectionID);

    int currSection = 0;
public:
    Model(QObject *parent = nullptr);
    ~Model();

signals:

public slots:
    void changeSection(int index);
};

#endif // MODEL_H
