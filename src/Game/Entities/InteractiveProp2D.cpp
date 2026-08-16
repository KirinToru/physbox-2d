#include <Game/Entities/InteractiveProp2D.hpp>

void InteractiveProp2D::init(b2WorldId world, sf::Vector2f position, sf::Vector2f size, float P2M, const PropPhysicsSettings& settings, ShapeType shapeType) {
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

    if (shapeType == ShapeType::Circle) {
        b2Circle circle = {{0.0f, 0.0f}, size.x / 2.0f / P2M};
        b2CreateCircleShape(mBody, &shapeDef, &circle);
    } else if (shapeType == ShapeType::Box) {
        b2Polygon box = b2MakeBox(size.x / 2.f / P2M, size.y / 2.f / P2M);
        b2CreatePolygonShape(mBody, &shapeDef, &box);
    } else if (shapeType == ShapeType::Triangle) {
        b2Vec2 vertices[3];
        float hw = size.x / 2.f / P2M;
        float hh = size.y / 2.f / P2M;
        vertices[0] = {-hw, -hh};
        vertices[1] = {hw, hh};
        vertices[2] = {-hw, hh};
        
        b2Hull hull = b2ComputeHull(vertices, 3);
        b2Polygon triangle = b2MakePolygon(&hull, 0.0f);
        b2CreatePolygonShape(mBody, &shapeDef, &triangle);
    } else if (shapeType == ShapeType::Star) {
        // A star can be approximated by a central pentagon and 5 outer triangles,
        // or a simple circle if we don't need complex collision.
        // For physics, let's just make it a circle, and the visual will be a star,
        // OR we can make it a pentagram-like polygon. Box2D only supports convex polygons.
        // To make a non-convex star we'd need to attach multiple convex shapes.
        // Let's create a 5-point star using 5 triangles attached to a central pentagon,
        // OR just a simple circle with a star graphic for performance and simplicity.
        // Actually, let's do a simple circle for the collision of a star to prevent snagging.
        b2Circle circle = {{0.0f, 0.0f}, size.x / 2.0f / P2M};
        b2CreateCircleShape(mBody, &shapeDef, &circle);
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
