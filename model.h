#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <vector>
#include <map>
#include <QTimer>
#include "Box2D/Box2D.h"
#include "section.h"
#include "physobject.h"

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

    // Box2D
    b2Vec2 gravity = b2Vec2(0.0f, 10.0f);
    b2World world = b2World(gravity);
    /**
     * @brief physObjBodies Stores the physObjects with their id as the key
     */
    std::map<int, physObject*> physObjBodies;
    QTimer timer;


    void finalizeSectionChange();
    /**
     * @brief setupWorld Set up the Box2D world on construction
     */
    void setupWorld();

public:
    Model(QObject *parent = nullptr);
    ~Model();

signals:
    void requestSaveCurrentCode();
    void codeFinishedSaving();
    void codeUpdated(QString newCode);

    /**
     * @brief newPosition Tell the view to give the QLabel physObj a new position
     * @param x
     * @param y
     */
    void newPosition(int id, int x, int y);
    void newPhysObj(int x, int y);

public slots:
    void changeSection(int index);
    void saveCodeToCurrentIndex(std::string code);

    void updateWorld();
    void spawnPhysBox(int x, int y);

};

#endif // MODEL_H
