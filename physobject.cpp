#include "physobject.h"
#include "Box2D/Box2D.h"

int physObject::id_counter = 0;

physObject::physObject(b2World* world, int x, int y, float32 boxHX, float32 boxHY, float32 density, float32 friction, float32 restitution) {

    id = id_counter++;

    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    physObjBody = world->CreateBody(&bodyDef);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(boxHX, boxHY);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = density;

    // Override the default friction.
    fixtureDef.friction = friction;

    fixtureDef.restitution = restitution;

    // Add the shape to the body.
    physObjBody->CreateFixture(&fixtureDef);
}

b2Vec2 physObject::getPosition() {
    return physObjBody->GetPosition();
}

float32 physObject::getAngle() {
    return physObjBody->GetAngle();
}

int physObject::getID() {
    return id;
}
