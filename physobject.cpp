/**
 * Description: cpp file for the model of the Learn Mips application which handles all of the logic for the app
 * Authors: Matthew Ashton Knochel, Carter Dean, Abdulla Alnuaimi, Logan Luker, Aidan Sheehan
*/

#include "physobject.h"

physObject::physObject(b2World* world, Shape shape, int x, int y, float32 hx, float32 hy, float32 density, float32 friction, float32 restitution) {
    worldPtr = world;

    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    physObjBody = world->CreateBody(&bodyDef);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;

    // Define a shape for our dynamic body fixture. Circles not working yet, only rects
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(hx, hy);
    // if(shape == rect) {
    //     b2PolygonShape dynamicBox;
    //     dynamicBox.SetAsBox(hx, hy);
    //     dynamicShape = &dynamicBox;
    // }
    // else if (shape == circle) {
    //     b2CircleShape dynamicCircle;
    //     dynamicCircle.m_radius = hx;
    //     dynamicShape = &dynamicCircle;
    // }

    fixtureDef.shape = &dynamicBox;
    //fixtureDef.shape = dynamicShape;

    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = density;

    // Override the default friction.
    fixtureDef.friction = friction;

    fixtureDef.restitution = restitution;

    // Add the shape to the body.
    physObjBody->CreateFixture(&fixtureDef);
}

physObject::~physObject() {
    worldPtr->DestroyBody(physObjBody);
}

b2Vec2 physObject::getPosition() {
    return physObjBody->GetPosition();
}

float32 physObject::getAngle() {
    return physObjBody->GetAngle();
}

void physObject::applyForce() {
    b2Vec2 force(-32, -32);
    b2Vec2 point = physObjBody->GetPosition();
    physObjBody->ApplyLinearImpulse(force, point, false);
}

