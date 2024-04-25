/**
 * Description: cpp file for the model of the Learn Mips application which handles all of the logic for the app
 * Authors: Matthew Ashton Knochel, Carter Dean, Abdulla Alnuaimi, Logan Luker, Aidan Sheehan
*/

#include "model.h"
#include <vector>
#include "validityFunctions.h"
#include "physobject.h"
#include "section.h"
#include <QFile>
#include <QTextStream>

Model::Model(QObject *parent) : QObject{parent} {

    for(int i = 0; i < NUM_OF_SECTIONS; i++) sections->push_back(buildSection(i)); //create each section

    codeStrings = new std::string[NUM_OF_SECTIONS];
    for(int i = 0; i < NUM_OF_SECTIONS; i++) codeStrings[i] = "";

    progressCheckBools = new bool[NUM_OF_SECTIONS];
    for(int i = 0; i < NUM_OF_SECTIONS; i++) progressCheckBools[i] = false;

    // Box2D
    setupWorld();
}

Model::~Model(){
    delete sections;
    delete[] codeStrings;
    delete[] progressCheckBools;
}

void Model::executeCode(QString code, bool checkSolutionValidity) {
    //run code
    Challenge* chal = sections->at(currSection).getChallenge();

    std::tuple<std::string, bool> output = chal->executeCode(code.toStdString(), checkSolutionValidity);
    std::string outputText = std::get<0>(output);
    bool isValidSolution = std::get<1>(output);

    if(outputText.size() > 0) currentConsoleText.append(outputText + "\n");
    emit consoleTextUpdated(currentConsoleText.c_str());

    if(checkSolutionValidity){
        if(isValidSolution){
            //challenge succeeded stars
            spawnStars();

            //set progress check and section completed bool
            sections->at(currSection).setCompleted(true);
            progressCheckBools[currSection] = true;
            emit progressCheckUpdated(currSection, true);
            if(currSection < NUM_OF_SECTIONS - 1) emit makeTabVisible(currSection+1, true);
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

void Model::pushTabVisibilities(){
    emit makeTabVisible(0, true);
    emit makeTabVisible(11, true);

    for(int i = 0; i < NUM_OF_SECTIONS - 1; i++){
        if(sections->at(i).isComplete()){
            emit makeTabVisible(i, true);
            emit makeTabVisible(i + 1, true);
        }
    }
}

void Model::setupWorld() {
    // TODO [Box2D]: set walls around the window, preferably scaled to the window size


    b2BodyDef boundingBoxBodyDef;
    boundingBoxBodyDef.position.Set(0.0f, 0.0f);
    b2Body* boundingBoxBody = world.CreateBody(&boundingBoxBodyDef);

    // Set a bounding box around the window
    int numOfVerticies = 4;
    b2Vec2 vs[numOfVerticies];
    vs[0].Set(0.0f, -screenHeight);
    vs[1].Set(screenWidth, -screenHeight);
    vs[2].Set(screenWidth, screenHeight);
    vs[3].Set(0.0f, screenHeight);
    b2ChainShape boundingBoxChain;
    boundingBoxChain.CreateLoop(vs, numOfVerticies);

    boundingBoxBody->CreateFixture(&boundingBoxChain, 0.0f);

    // Start update loop
    timer.singleShot(1500, this, &Model::updateWorld);
}

void Model::updateWorld() {
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

    // Check for objects to update
    for(auto const& [id, physObjBody] : physObjBodies) {
        b2Vec2 position = physObjBody->getPosition();
        emit newPosition(id, position.x, position.y);
    }
    // When the world destructor is called, all bodies and joints are freed. This can
    // create orphaned pointers, so be careful about your world management.

    timer.singleShot(42, this, &Model::updateWorld);
}

void Model::spawnPhysObject(int id, Shape shape, int x, int y) {
    // Define the dynamic body. We set its position and call the body factory.
    physObject* physObj = new physObject(&world, shape, x, y);
    physObjBodies[id] = physObj;
    physObj->applyForce();

    emit newPhysObj(id, x, y);
}

void Model::destroyPhysObject(int id) {
    if(!physObjBodies.count(id)) {
        qDebug("ID doesn't exist");
        return;
    }

    delete physObjBodies[id];
    physObjBodies.erase(id);

    emit deletePhysLabel(id);
}

void Model::destroyAllPhysObjects() {
    for(auto const& [id, physObjBody] : physObjBodies) {
        delete physObjBodies[id];
        emit deletePhysLabel(id);
    }
    physObjBodies.clear();
}

void Model::spawnStars() {
    if(physObjBodies.size() > 0)
        return;

    int numOfStars = 100;
    // Spawn numOfStars stars randomly across the top
    // TODO [Box2D]: create a global width and height for physics objects + the b2World
    for(int id = 0; id < numOfStars; id++) {
        int x = std::rand() % screenWidth;
        int y = std::rand() % screenWidth;
        timer.singleShot(0, this, [this, id, x, y] {spawnPhysObject(id, rect, x, -y);});
    }

    // After some time, delete them
    for(int id = 0; id < numOfStars; id++) {
        timer.singleShot(8000, this, [this, id] {destroyPhysObject(id);});
    }
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
    //ensure save folder exists and create otherwise
    if(!std::filesystem::exists(saveLocation))
        std::filesystem::create_directory(saveLocation);

    //save the string to a file
    std::string filename = saveLocation + fileName;
    QFile file(filename.c_str());
    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        QTextStream stream(&file);
        stream << codeStrings[sectionID].c_str();
        file.close();
    }
}

void Model::saveProgressChecks(std::string saveLocation){
    //ensure save folder exists, create otherwise
    if(!std::filesystem::exists(saveLocation))
        std::filesystem::create_directory(saveLocation);

    //save progress
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

        //loop through each line and update the temporary storage and view as needed
        for(int i = 0; i < NUM_OF_SECTIONS; i++){
            QString line = stream.readLine();
            if(line.toInt() == 1){
                progressCheckBools[i] = true;
                sections->at(i).setCompleted(true);
            }
            else{
                progressCheckBools[i] = false;
                sections->at(i).setCompleted(false);
            }

            emit progressCheckUpdated(i, progressCheckBools[i]);

            if(progressCheckBools[i]){
                emit makeTabVisible(i, true);
                if(i < NUM_OF_SECTIONS - 1) emit makeTabVisible(i+1, true);
            }

        }
        file.close();
    }
}


//logic for building each section with proper validity function
Section Model::buildSection(int sectionID){
    switch(sectionID){
        case 0:
            return Section(Challenge(
                "\n.text\naddi $a0, $zero, 11 \n",
                "",
                ValidityFunctions::section1ValidityFunction
                ));
        case 1:
            return Section( // srl/sll challenge
                Challenge(
                    "addi $a0, $zero, 3000\n", //before Code
                    "", //after Code
                    ValidityFunctions::section2ValidityFunction));
        case 2:
            return Section(
                Challenge( // syscalls/strings
                    "\n.text\naddi $s0, $zero, 32767 \n", //before Code
                    "", //after Code
                    ValidityFunctions::section3ValidityFunction));
        case 3:
            return Section(
                Challenge(//j, labels, challenge: make a for loop that does something and prints each loop Bne, beq, blt, bgt, some basic conditional logic stuff
                    "\n.text\naddi $a0, $zero, 12 \n", //before Code
                    "", //after Code
                    ValidityFunctions::section4ValidityFunction));
        case 4:
            return Section(
                Challenge( //Lw, sw, sp register, challenge: work on storing 32 bit integers
                    "addi $sp, $sp, -16\naddi $a0, $zero, 4\n sw $a0, 0($sp)\n sw $a0, 4($sp) \n sw $a0, 8($sp) \n", //before Code
                    "lw $v0, 12($sp)", //after Code
                    ValidityFunctions::section5ValidityFunction));
        case 5:
            return Section(
                Challenge( //Challenge - Lb, sb, la, edit a string, or character count, etc
                   R"(.data
                    myString: .asciiz "helloworld"
                    .text
)", //before Code
                    "", //after Code
                    ValidityFunctions::section6ValidityFunction));
        case 6: // XOR challenge
            return Section(
                Challenge(
                    R"(.data
                    plaintext: .asciiz "unencrypted"
                    key: .asciiz "qwertyuiopa"
                    .text
)", //before Code
                    "", //after Code
                    ValidityFunctions::section7ValidityFunction));
        case 7:
            return Section(
                Challenge(
                    R"(addi $s1, $zero, 1532
                    addi $a0, $zero, 5427
                    j skipFunction
                    addSecretNumber:
                    add $v1, $a0, $s1
                    jr $ra
                    skipFunction:
                    jal addFive)", //before Code
                    "", //after Code
                    ValidityFunctions::section8ValidityFunction));
        case 8:
            return Section(
                Challenge(
                    "addi $a0, $zero, 10", //before Code
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


