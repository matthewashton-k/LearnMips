/**
 * Description: header file for the class representing a Box2D physics object
 * Authors: Matthew Ashton Knochel, Carter Dean, Abdulla Alnuaimi, Logan Luker, Aidan Sheehan
*/
#ifndef PHYSOBJECT_H
#define PHYSOBJECT_H

#include "Box2D/Box2D.h"

/**
 * @brief The Shape enum is used to determine the shape of the physObject.
 * @note Circle isn't implemented yet but could be in the future along with other shapes.
 */
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
    /**
     * @brief physObject Constructs a physObject in the world at the provided x, y coordinate. The width, height, density, friction,
     * and restitution can be optionally specified when constructed, however, they cannot be changed afterwards.
     * @param world
     * @param shape
     * @param x the horizontal component in pixels
     * @param y the vertical component in pixels
     * @param hx the half width or width of the object from the center to its left or right edge. Default is 1.0f
     * @param hy the half height or height of the object from the center to its top or bottom edge. Default is 1.0f
     * @param density The density controls the mass of the Box2D. If set to 0, the Box2D object will be static. usually in kg/m^2. Default is 1.0f
     * @param friction The friction of the Box2D object. usually in the range [0,1]. Default is 0.3f.
     * @param restitution The restitution or elasticity of the Box2D object. usually in the range [0,1]. Default is 0.9f.
     */
    physObject(b2World* world, Shape shape, int x, int y, float32 hx = 1.0f, float32 hy = 1.0f, float32 density = 1.0f, float32 friction = 0.3f, float32 restitution = 1.0f);
    /**
     * @brief Destructor for physObject
     */
    ~physObject();


    /**
     * @brief getPosition Wrapper for b2Body's GetPosition method
     * @return b2Vec2 of this physObject's position
     */
    b2Vec2 getPosition();
    /**
     * @brief getAngle Wrapper for b2Body's GetAngle method
     * @return b2Vec2 of this physObject's angle
     */
    float32 getAngle();

    void applyForce();

private:
    b2World* worldPtr;
    /**
     * @brief physObjBody The backing b2Body
     */
    b2Body* physObjBody;
    /**
     * @brief shape Will always be rect (aka box) until different shapes are implemented.
     */
    Shape shape = rect;
};

#endif // PHYSOBJECT_H
