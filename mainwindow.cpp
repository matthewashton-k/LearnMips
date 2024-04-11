#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Interpreter.h"
#include "Box2D/Box2D.h"
#include <iostream>
using std::cout;
using std::endl;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) , modelPtr(new Model())
{
    ui->setupUi(this);

    connect(ui->runButton, &QPushButton::clicked, this, &MainWindow::runButtonClicked);

    //switching section stuff
    connect(ui->sectionTabs, &QTabWidget::currentChanged, modelPtr, &Model::changeSection);
    connect(modelPtr, &Model::requestSaveCurrentCode, this, &MainWindow::currentCodeRequested);
    connect(this, &MainWindow::answerCurrentCodeRequest, modelPtr, &Model::saveCodeToCurrentIndex);
    connect(modelPtr, &Model::codeUpdated, ui->codeEdit, &QTextEdit::setText);

    // Box2D
    connect(this,
            &MainWindow::newPosition,
            this,
            &MainWindow::moveLabel // TODO [Box2D]: make a custom promoted QLabel that handles displaying physics objects
        );

    setupWorld(); // TODO [Box2D]: move to model constructor
}

MainWindow::~MainWindow()
{
    delete ui;
    delete modelPtr;
}

void MainWindow::runButtonClicked(){
    //TEMPORARY TEST!
    std::string instructions = ui->codeEdit->toPlainText().toStdString();

    Interpreter interpreter(instructions);
    try {
        QString str = interpreter.run().c_str();
        ui->console->setText(str);
    } catch (std::string err) {
        ui->console->setText(QString(err.c_str()));
        cout << "Error: " << err << endl;
    }
    //END TEMPORARY TEST!
}

void MainWindow::currentCodeRequested(){
    emit answerCurrentCodeRequest(ui->codeEdit->toPlainText().toStdString());
}

void MainWindow::setupWorld() {
    // TODO [Box2D]: move to model, should go in the model

    // Define the gravity vector.
    //gravity = b2Vec2(0.0f, -10.0f);

    // Construct a world object, which will hold and simulate the rigid bodies.
    //world = b2World(gravity);

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

    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    int labelPosX = ui->physObj->x();
    int labelPosY = ui->physObj->y();
    bodyDef.position.Set(labelPosX, labelPosY);
    physObjBody = world.CreateBody(&bodyDef);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;

    // Override the default friction.
    fixtureDef.friction = 0.3f;

    fixtureDef.restitution = 0.9f;

    // Add the shape to the body.
    physObjBody->CreateFixture(&fixtureDef);

    timer.singleShot(2000, this, &MainWindow::updateWorld);
}

void MainWindow::updateWorld() {
    // TODO [Box2D]: move to model, should go in the model

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

    // Now print the position and angle of the body.
    b2Vec2 position = physObjBody->GetPosition();
    float32 angle = physObjBody->GetAngle();

    emit newPosition(position.x, position.y);

    //qDebug() << position.x << position.y << angle;
    //qDebug("--finished update");
    // When the world destructor is called, all bodies and joints are freed. This can
    // create orphaned pointers, so be careful about your world management.

    timer.singleShot(80, this, &MainWindow::updateWorld);
}

void MainWindow::moveLabel(int x, int y) {
    // TODO [Box2D]: make a custom promoted QLabel that handles displaying physics objects
    ui->physObj->move(x,y);
}
