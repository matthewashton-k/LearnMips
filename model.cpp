#include "model.h"
#include <vector>
#include "validityFunctions.h"
#include "Box2D/Box2D.h"
#include "physobject.h"
#include "Interpreter.h"
#include "section.h"
#include <iostream>
#include <QFile>
#include <QTextStream>

Model::Model(QObject *parent) : QObject{parent} {
    for(int i = 0; i < NUM_OF_SECTIONS; i++) sections->push_back(buildSection(i)); //create each section

    codeStrings = new std::string[NUM_OF_SECTIONS];
    for(int i = 0; i < NUM_OF_SECTIONS; i++) codeStrings[i] = "";

    progressCheckBools = new bool[NUM_OF_SECTIONS];
    for(int i = 0; i < NUM_OF_SECTIONS; i++) progressCheckBools[i] = false;

    progressCheckBools[0] = true;
    progressCheckBools[11] = true;

    // Box2D
    setupWorld();
}

Model::~Model(){
    delete sections;
    delete[] codeStrings;
    delete[] progressCheckBools;
}

void Model::executeCode(QString code, bool checkSolutionValidity){
    //run code
    Challenge* chal = sections->at(currSection).getChallenge();

    std::tuple<std::string, bool> output = chal->executeCode(code.toStdString(), checkSolutionValidity);
    std::string outputText = std::get<0>(output);
    bool isValidSolution = std::get<1>(output);

    currentConsoleText.append(outputText + "\n");
    emit consoleTextUpdated(currentConsoleText.c_str());

    if(checkSolutionValidity){
        if(isValidSolution){
            //challenge succeeded, confetti or whatever

            //set progress check and section completed bool
            sections->at(currSection).setCompleted(true);
            progressCheckBools[currSection] = true;
            emit progressCheckUpdated(currSection, true);
        }
        else{
            //challenge failed, smoke, sound, rick role, whatever
        }
    }
}

void Model::changeSection(int index){
    nextSection = index;
    emit requestSaveCurrentCode();
}

void Model::finalizeSectionChange(){
    currSection = nextSection;
    nextSection = 0;
    QString str = codeStrings[currSection].c_str();
    emit codeUpdated(str);
    for(int i = 0; i < NUM_OF_SECTIONS; i++) emit progressCheckUpdated(i, progressCheckBools[i]); //update check marks
}

void Model::saveCodeToCurrentIndex(std::string code){
    codeStrings[currSection] = code;
    finalizeSectionChange();
}

void Model::clearConsole(){
    currentConsoleText = "";
    QString str = currentConsoleText.c_str();
    emit consoleTextUpdated(str);
}

void Model::setupWorld() {
    // TODO [Box2D]:

    // Define the gravity vector.
    //gravity = b2Vec2(0.0f, -10.0f);

    // Construct a world object, which will hold and simulate the rigid bodies.
    //world = b2World(gravity);


    // TODO [Box2D]: set walls around the window, preferably scaled to the window size
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 700.0f);

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2PolygonShape groundBox;

    // The extents are the half-widths of the box.
    groundBox.SetAsBox(1000.0f, 50.0f);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 0.0f);


    // TODO [Box2D]: Convert to physObject and handle spawning them with a given position

    // Define the dynamic body. We set its position and call the body factory.
    physObject body1(&world, 560, 105);
    physObjBodies.push_back(body1);

    timer.singleShot(2000, this, &Model::updateWorld);
}

void Model::updateWorld() {
    // TODO [Box2D]:

    //qDebug("--start update");
    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    // This is our little game loop.
    for (int32 i = 0; i < 60; ++i)
    {
        // Instruct the world to perform a single step of simulation.
        // It is generally best to keep the time step and iterations fixed.
        world.Step(timeStep, velocityIterations, positionIterations);
    }


    // TODO [Box2D]: This will need to be generalized to work with many different objects

    // Now print the position and angle of the body.
    for(physObject physObjBody : physObjBodies) {
        b2Vec2 position = physObjBody.getPosition();
        float32 angle = physObjBody.getAngle();

        emit newPosition(position.x, position.y);
    }

    //qDebug() << position.x << position.y << angle;
    //qDebug("--finished update");
    // When the world destructor is called, all bodies and joints are freed. This can
    // create orphaned pointers, so be careful about your world management.

    timer.singleShot(42, this, &Model::updateWorld);
}

//save stuff
void Model::saveAllProgress(){
    changeSection(currSection);
    for(int i = 0; i < NUM_OF_SECTIONS; i++){
        saveSectionASMFile(i, "./saveFiles/", "section" + std::to_string(i) + ".asm");
    }
    saveProgressChecks("./saveFiles/");
}

void Model::saveSectionASMFile(int sectionID, std::string saveLocation, std::string fileName){
    if(!std::filesystem::exists(saveLocation))
        std::filesystem::create_directory(saveLocation);

    std::string filename = saveLocation + fileName;
    QFile file(filename.c_str());
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << codeStrings[sectionID].c_str();
        file.close();
    }

}

void Model::saveProgressChecks(std::string saveLocation){
    if(!std::filesystem::exists(saveLocation))
        std::filesystem::create_directory(saveLocation);

    std::string filename = saveLocation + "progress.txt";
    QFile file(filename.c_str());
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        for(int i = 0; i < NUM_OF_SECTIONS; i++)
            stream << std::to_string(progressCheckBools[i]).c_str() << Qt::endl;
        file.close();
    }
}
//load stuff
void Model::loadAllProgress(){
    for(int i = 0; i < NUM_OF_SECTIONS; i++){
        loadSectionASMFile(i, "./saveFiles/", "section" + std::to_string(i) + ".asm");
    }
    loadProgressChecks("./saveFiles/");

    QString str = codeStrings[currSection].c_str();
    emit codeUpdated(str);
}
void Model::loadSectionASMFile(int sectionID, std::string saveLocation, std::string fileName){
    std::string filename = saveLocation + fileName;
    QFile file(filename.c_str());
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        QString section = stream.readAll();
        codeStrings[sectionID] = section.toStdString();
        file.close();
    }
}
void Model::loadProgressChecks(std::string saveLocation){
    std::string filename = saveLocation + "progress.txt";
    QFile file(filename.c_str());
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        for(int i = 0; i < NUM_OF_SECTIONS; i++){
            QString line = stream.readLine();
            if(line.toInt() == 0){
                progressCheckBools[i] = false;
                sections->at(currSection).setCompleted(false);
            }
            else{
                progressCheckBools[i] = true;
                sections->at(currSection).setCompleted(true);
            }

            emit progressCheckUpdated(i, progressCheckBools[i]);
        }
        file.close();
    }
}





//logic for building each section with proper validity function
Section Model::buildSection(int sectionID){
    switch(sectionID){
    case 0:
        return Section(Challenge(
            "addi $a0, $zero, 11",
            "",
            ValidityFunctions::section1ValidityFunction
            ));
    case 1:
        return Section(
            Challenge(
                "", //before Code
                "", //after Code
                ValidityFunctions::section2ValidityFunction));
    case 2:
        return Section(
            Challenge(
                "", //before Code
                "", //after Code
                ValidityFunctions::section3ValidityFunction));
    case 3:
        return Section(
            Challenge(
                "", //before Code
                "", //after Code
                ValidityFunctions::section4ValidityFunction));
    case 4:
        return Section(
            Challenge(
                "", //before Code
                "", //after Code
                ValidityFunctions::section5ValidityFunction));
    case 5:
        return Section(
            Challenge(
                "", //before Code
                "", //after Code
                ValidityFunctions::section6ValidityFunction));
    case 6:
        return Section(
            Challenge(
                "", //before Code
                "", //after Code
                ValidityFunctions::section7ValidityFunction));
    case 7:
        return Section(
            Challenge(
                "", //before Code
                "", //after Code
                ValidityFunctions::section8ValidityFunction));
    case 8:
        return Section(
            Challenge(
                "", //before Code
                "", //after Code
                ValidityFunctions::section9ValidityFunction));
    case 9:
        return Section(
            Challenge(
                "", //before Code
                "", //after Code
                ValidityFunctions::section10ValidityFunction));
    case 10:
        return Section(
            Challenge(
            "", //before Code
            "", //after Code
            ValidityFunctions::section11ValidityFunction));
    case 11:
        return Section();
    default:
        return Section();
    }
}


