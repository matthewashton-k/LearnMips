/**
 * Description: header file for the Main Window of the Learn Mips application
 * Authors: Matthew Ashton Knochel, Carter Dean, Abdulla Alnuaimi, Logan Luker, Aidan Sheehan
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "model.h"
#include "highligher.h"
#include "tabbutton.h"
#include <QPropertyAnimation>
#include <iostream>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow constructor
     * @param parent parent widget
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief deconstructor for Main Window
     */
    ~MainWindow();

private:
    /**
     * @brief ui pointer to ui
     */
    Ui::MainWindow *ui;

    /**
     * @brief modelPtr pointer to the model for connecting signals/slots
     */
    Model* modelPtr;

    /**
     * @brief highlighter text highlighter to add colors
     */
    Highlighter *highlighter;

    /**
     * @brief tabButton buttons to navigate tabs functionality
     */
    TabButton* tabButton;

    /**
     * @brief physObjBodies Stores physics QLabels with their id as the key
     */
    std::map<int, QLabel*> physObjLabels;

    /**
     * @brief closeEvent override of close event
     * @param event the event
     */
    void closeEvent (QCloseEvent *event);

    /**
     * @brief refreshTabVisibilities helper function that requests tab visibilities from the model
     */
    void refreshTabVisibilities();

    /**
     * @brief animation member variable to handle animations
     */
    QPropertyAnimation animation;
    QPropertyAnimation animation2;
    QPropertyAnimation animation3;
    QPropertyAnimation animation4;

    /**
     * @brief fixCurrentPage helper method to make sure current page is correct
     */
    void fixCurrentPage();
    /**
     * @brief doAnimation generic method to do a generic animation from start to end location
     * @param target
     * @param duration
     * @param start
     * @param end
     */
    void doAnimation(QObject *target, int duration, QRect start, QRect end);

    /**
     * @brief hideCodeBox hides the code box
     */
    void hideCodeBox();

signals:
    /**
     * @brief answerCurrentCodeRequest signal to send the current code in the editor
     */
    void answerCurrentCodeRequest(std::string);

    /**
     * @brief runRequest signal to run the code
     * @param code the user code to run
     * @param checkSolutionValidity determines if the solution should be checked or the code just run
     */
    void runRequest(QString code, bool checkSolutionValidity);

    /**
     * @brief requestTabVisibilities signal to trigger the model to push the updated visibilities of the tabs
     */
    void requestTabVisibilities();

private slots:
    /**
     * @brief currentIndexChanged for when the page changes
     */
    void currentIndexChanged();
    /**
     * @brief runButtonClicked slot for when the run button is pressed
     */
    void runButtonClicked();
    /**
     * @brief submitButtonClicked slot for when the check solution button is clicked
     */
    void submitButtonClicked();

    /**
     * @brief showHideMenuButtonClicked for when the show/hide menu button is clicked
     */
    void showHideMenuButtonClicked();

    /**
     * @brief nextButtonClicked goes to the next page
     */
    void nextButtonClicked();

    /**
     * @brief previousButtonClicked goes to the previous page
     */
    void previousButtonClicked();

    /**
     * @brief currentCodeRequested slot to respond to  current code requests from the model
     */
    void currentCodeRequested();

    /**
     * @brief moveLabel Moves the physObj label with the specified id to the given x y relative to the window
     * @param id of the physicsObj label to move
     * @param x the horizontal component in pixels
     * @param y the vertical component in pixels
     */
    void moveLabel(int id, int x, int y);

    /**
     * @brief createPhysLabel creates a QLabel with physics at the given x y coordinate
     * @param x the horizontal component in pixels
     * @param y the vertical component in pixels
     */
    void createPhysLabel(int id, int x, int y);

    /**
     * @brief deletePhysLabel Deletes the physics label with the given id.
     * @param id of the physicsObj to be deleted.
     */
    void deletePhysLabel(int id);

    /**
     * @brief updateConsole slot to update the text of the console to text
     * @param text the new console text
     */
    void updateConsole(QString text);

    /**
     * @brief updateCheckBox slot to set a section as completed visually
     * @param ID id of section to complete
     * @param checked whether to check or uncheck the section
     */
    void updateCheckBox(int ID, bool checked);

    /**
     * @brief displayReferenceWindow slot to display the reference list window
     */
    void displayReferenceWindow();

    /**
     * @brief displayReferenceWindow slot to display the op code reference list window
     */
    void displayOpCodeReferenceWindow();

    /**
     * @brief setSectionTabVisible sets a tab's visibility
     * @param ID the tab to set
     * @param state the new state to set the tab to
     */
    void setSectionTabVisible(int ID, bool state);

    /**
     * @brief handleUpdateTab updates the current tab to view
     * @param tabID
     */
    void handleTabUpdate(std::tuple<int,int> tabID);
};
#endif // MAINWINDOW_H
