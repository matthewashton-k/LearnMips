#ifndef PHYSOBJECT_H
#define PHYSOBJECT_H

#include "Box2D/Box2D.h"

/**
 * @brief The physObject class is a wrapper for b2Body that get spawned in our app. It handles everything that a physics body does all in a neat little package.
 */
class physObject
{
public:
    physObject(b2World* world, int x, int y, float32 boxHX = 1.0f, float32 boxHY = 1.0f, float32 density = 1.0f, float32 friction = 0.3f, float32 restitution = 0.9f);
    \
    b2Vec2 getPosition();
    float32 getAngle();

private:
    b2Body* physObjBody;



};

#endif // PHYSOBJECT_H
