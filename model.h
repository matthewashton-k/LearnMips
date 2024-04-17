#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <vector>
#include <map>
#include <QTimer>
#include "Box2D/Box2D.h"
#include "section.h"
#include "physobject.h"
#include <filesystem>

class Model : public QObject
{
    Q_OBJECT
private:
    int NUM_OF_SECTIONS = 12;

    std::string* codeStrings;
    bool* progressCheckBools;
    //vector to store all the sections
    std::vector<Section>* sections = new std::vector<Section>();

    //functions to build each section
    Section buildSection(int sectionID);

    int currSection = 0;
    int nextSection = 0;

    std::string currentConsoleText;


    // Box2D
    b2Vec2 gravity = b2Vec2(0.0f, 10.0f);
    b2World world = b2World(gravity);
    /**
     * @brief physObjBodies Stores the physObjects with their id as the key
     */
    std::map<int, physObject*> physObjBodies;
    QTimer timer;

    // Box2D
    /**
     * @brief setupWorld Set up the Box2D world on construction
     */
    void setupWorld();

    void finalizeSectionChange();

    void saveSectionASMFile(int sectionID, std::string saveLocation, std::string fileName);
    void saveProgressChecks(std::string saveLocation);
    void loadSectionASMFile(int sectionID, std::string fileLocation, std::string fileName);
    void loadProgressChecks(std::string fileLocation);


public:
    Model(QObject *parent = nullptr);
    ~Model();

signals:
    void requestSaveCurrentCode();
    void codeFinishedSaving();
    void codeUpdated(QString newCode);
    void consoleTextUpdated(QString text);
    void progressCheckUpdated(int ID, bool state);

    // Box2D signals
    /**
     * @brief newPosition Tell the view to give the QLabel physObj a new position
     * @param x
     * @param y
     */
    void newPosition(int id, int x, int y);
    void newPhysObj(int id, int x, int y);

public slots:
    void changeSection(int index);
    void saveCodeToCurrentIndex(std::string code);

    // Box2D slots
    void updateWorld();
    void spawnPhysObject(int id, Shape shape, int x, int y);
    void spawnConfetti();

    void clearConsole();
    void executeCode(QString code, bool checkSolutionValidity);
    void saveAllProgress();
    void loadAllProgress();

};

#endif // MODEL_H
