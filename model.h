/**
 * Description: header file for the model of the Learn Mips application which handles all of the logic for the app
 * Authors: Matthew Ashton Knochel, Carter Dean, Abdulla Alnuaimi, Logan Luker, Aidan Sheehan
*/

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
    /**
     * @brief NUM_OF_SECTIONS defines how many sections the app has
     */
    int NUM_OF_SECTIONS = 12;

    /**
     * @brief codeStrings array of strings to temporarily store the code in the editor for each section
     */
    std::string* codeStrings;

    /**
     * @brief progressCheckBools an arroay that stores whether a section has been completed or not
     */
    bool* progressCheckBools;

    /**
     * @brief sections vector to store all the sections
     */
    std::vector<Section>* sections = new std::vector<Section>();

    /**
     * @brief buildSection function to build each section
     * @param sectionID the id of the section to build
     * @return the resulting section
     */
    Section buildSection(int sectionID);

    /**
     * @brief currSection int that stores the current section that is selected
     */
    int currSection = 0;

    /**
     * @brief nextSection the next section to switch to when switching sections
     */
    int nextSection = 0;

    /**
     * @brief currentConsoleText a string storing the current console output
     */
    std::string currentConsoleText;


    // Box2D
    /**
     * @brief screenWidth The width of the screen.
     */
    int screenWidth = 1323;
    /**
     * @brief screenHeight The height of the screen
     */
    int screenHeight = 881;
    /**
     * @brief gravity of the world
     */
    b2Vec2 gravity = b2Vec2(0.0f, 10.0f);
    /**
     * @brief world The Box2D world
     */
    b2World world = b2World(gravity);
    /**
     * @brief physObjBodies Stores the physObjects with their id as the key
     */
    std::map<int, physObject*> physObjBodies;

    /**
     * @brief timer timer to schedule events
     */
    QTimer timer;

    // Box2D
    /**
     * @brief setupWorld Set up the Box2D world on construction
     */
    void setupWorld();

    /**
     * @brief finalizeSectionChange the last step of changing the current section
     */
    void finalizeSectionChange();

    /**
     * @brief saveSectionASMFile saves a .asm file containing the code in the given section
     * @param sectionID Id of section to save
     * @param saveLocation location of where to save
     * @param fileName name of file to save
     */
    void saveSectionASMFile(int sectionID, std::string saveLocation, std::string fileName);

    /**
     * @brief saveProgressChecks saves the current completion states for all sections
     * @param saveLocation the location to save at
     */
    void saveProgressChecks(std::string saveLocation);

    /**
     * @brief loadSectionASMFile loads a .asm file into the editor
     * @param sectionID the id to load the file into
     * @param fileLocation the location to load from
     * @param fileName the name of the file to load
     */
    void loadSectionASMFile(int sectionID, std::string fileLocation, std::string fileName);

    /**
     * @brief loadProgressChecks loads all the completion states of the sections
     * @param fileLocation the location of the file to load
     */
    void loadProgressChecks(std::string fileLocation);


public:
    /**
     * @brief Model constructor for the model
     * @param parent parent widget
     */
    Model(QObject *parent = nullptr);

    /**
     * @brief deconstructor for the model
     */
    ~Model();

signals:
    /**
     * @brief requestSaveCurrentCode signal to request the code currently in the editor
     */
    void requestSaveCurrentCode();

    /**
     * @brief codeFinishedSaving signal to emit when the code has finished being saved into the string array
     */
    void codeFinishedSaving();

    /**
     * @brief codeUpdated signal to emit when the code editor should be updated
     * @param newCode the new code to be displayed
     */
    void codeUpdated(QString newCode);

    /**
     * @brief consoleTextUpdated signal to emit when the console should be updated
     * @param text the new text
     */
    void consoleTextUpdated(QString text);

    /**
     * @brief progressCheckUpdated signal to emit when a completion state should be updated
     * @param ID the id of the section to update
     * @param state the new completion state
     */
    void progressCheckUpdated(int ID, bool state);

    /**
     * @brief makeTabVisible signal to emit when a section's tab should become visible
     * @param ID the id to change
     * @param state the new state
     */
    void makeTabVisible(int ID, bool state);

    // Box2D signals
    /**
     * @brief newPosition Tell the view to give the QLabel physObj with the given id a new position
     * @param id of the physicsObj label to move
     * @param x the horizontal component in pixels
     * @param y the vertical component in pixels
     */
    void newPosition(int id, int x, int y);
    /**
     * @brief newPhysObj Creates a new QLabel with physics as a child of the physicsObjects QWidget
     * @param id of the physicsObj label
     * @param x the horizontal component in pixels
     * @param y the vertical component in pixels
     */
    void newPhysObj(int id, int x, int y);
    /**
     * @brief deletePhysObj Tell the view to delete the physics label with the given id
     * @param id of the physicsObj label
     */
    void deletePhysLabel(int id);

public slots:
    /**
     * @brief changeSection the slot to handle changing to a new section
     * @param index
     */
    void changeSection(int index);

    /**
     * @brief saveCodeToCurrentIndex slot that saves a stirng to the currSection index in the string array
     * @param code the code to save
     */
    void saveCodeToCurrentIndex(std::string code);

    // Box2D slots
    /**
     * @brief updateWorld Starts an event loop that updates Box2D dynamic and kenetic objects
     * at approximately 24 frames per second.
     */
    void updateWorld();
    /**
     * @brief spawnPhysObject
     * @param id of the physicsObj label to spawn
     * @param shape Only supports rect. Circles aren't implemented but could be in the future.
     * @param x the horizontal component in pixels
     * @param y the vertical component in pixels
     */
    void spawnPhysObject(int id, Shape shape, int x, int y);
    /**
     * @brief destroyPhysObject Delete all physics objects currently spawned
     */
    void destroyPhysObject(int id);
    void destroyAllPhysObjects();
    void spawnStars();

    /**
     * @brief clearConsole slot that clears the console
     */
    void clearConsole();

    /**
     * @brief executeCode slot that handles executing the code or running the validity check for a solution
     * @param code the code to run
     * @param checkSolutionValidity whether or not to check the solution
     */
    void executeCode(QString code, bool checkSolutionValidity);

    /**
     * @brief saveAllProgress slot to save all the progress
     */
    void saveAllProgress();

    /**
     * @brief loadAllProgress slot to load all the progress
     */
    void loadAllProgress();

    /**
     * @brief pushTabVisibilities slot to respond to tab visibility requests from the main window
     */
    void pushTabVisibilities();

};

#endif // MODEL_H
