#ifndef PHYSOBJECT_H
#define PHYSOBJECT_H

#include "Box2D/Box2D.h"

enum Shape {
    rect,
    circle,
};

/**
 * @brief The physObject class is a wrapper for b2Body that get spawned in our app. It handles everything that a physics body does all in a neat little package.
 */
class physObject
{
public:
    physObject(b2World* world, Shape shape, int x, int y, float32 hx = 1.0f, float32 hy = 1.0f, float32 density = 1.0f, float32 friction = 0.3f, float32 restitution = 0.9f);
    ~physObject();
    b2Vec2 getPosition();
    float32 getAngle();
    int getID();

private:
    b2Body* physObjBody;
    Shape shape = rect;
    //int id = 0;
    //static int id_counter;

    //b2FixtureDef setShape(float32 boxHX, float32 boxHY, float32 density, float32 friction, float32 restitution);
};

#endif // PHYSOBJECT_H
