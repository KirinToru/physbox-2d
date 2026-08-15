#include <Game/Entities/InteractiveProp2D.hpp>

void InteractiveProp2D::init(b2WorldId world, sf::Vector2f position, sf::Vector2f size, float P2M, const PropPhysicsSettings& settings, bool isCircle) {
    mSettings = settings;

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = {position.x / P2M, position.y / P2M};
    bodyDef.linearDamping = settings.linearDamping;
    bodyDef.angularDamping = settings.angularDamping;
    bodyDef.isBullet = settings.useCCD; // Continuous Collision Detection
    
    mBody = b2CreateBody(world, &bodyDef);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = settings.density;
    shapeDef.material.friction = settings.friction;
    shapeDef.material.restitution = settings.restitution;

    if (isCircle) {
        b2Circle circle = {{0.0f, 0.0f}, size.x / 2.0f / P2M};
        b2CreateCircleShape(mBody, &shapeDef, &circle);
    } else {
        b2Polygon box = b2MakeBox(size.x / 2.f / P2M, size.y / 2.f / P2M);
        b2CreatePolygonShape(mBody, &shapeDef, &box);
    }
}

float InteractiveProp2D::calculateImpactDamage(const b2Vec2& impactVelocity) const {
    float mass = b2Body_GetMass(mBody);
    float velocityMag = std::sqrt(impactVelocity.x * impactVelocity.x + impactVelocity.y * impactVelocity.y);
    
    // Impact force proxy: (Mass * Velocity) / CollisionTime
    // We assume a normalized collision time for simplification
    float impactForce = mass * velocityMag;
    
    return impactForce * mSettings.damageMultiplier;
}
