#pragma once
#include <box2d/box2d.h>
#include <SFML/System/Vector2.hpp>

struct PropPhysicsSettings {
    float density = 1.0f;
    float friction = 0.5f;
    float restitution = 0.0f; // Bounciness (set low for heavy thuds)
    float linearDamping = 2.0f; // Simulates air resistance
    float angularDamping = 0.5f; // Keep low for dramatic spinning
    bool useCCD = true; // Continuous Collision Detection
    float damageMultiplier = 1.0f;
};

class InteractiveProp2D {
public:
    enum class ShapeType {
        Box,
        Circle,
        Triangle,
        Star
    };

    void init(b2WorldId world, sf::Vector2f position, sf::Vector2f size, float P2M, const PropPhysicsSettings& settings, ShapeType shapeType = ShapeType::Box);
    
    // Method to calculate impact damage on collision
    float calculateImpactDamage(const b2Vec2& impactVelocity) const;

    b2BodyId mBody;
    PropPhysicsSettings mSettings;
};
