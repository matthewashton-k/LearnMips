#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <vector>
#include "section.h"

class Model : public QObject
{
    Q_OBJECT
private:
    std::vector<Section> sections();
public:
    explicit Model(QObject *parent = nullptr);

signals:
};

#endif // MODEL_H
