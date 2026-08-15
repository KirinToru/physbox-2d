#pragma once
#include <box2d/box2d.h>
#include <SFML/System/Vector2.hpp>

struct MovementSettings {
    float maxSpeed = 5.5f;
    float maxAirSpeed = 5.5f; // Same as ground speed for full air strafing in 2D
    float groundAcceleration = 75.0f; // Snappier ground control
    float airAcceleration = 200.0f; // High to allow snappy air turns
    float friction = 4.0f;
    float stopSpeed = 2.0f;
    float jumpImpulse = 7.9f; // Higher impulse to counteract higher gravity while keeping the same height
};

class SourceMovementController2D {
public:
    SourceMovementController2D();
    SourceMovementController2D(MovementSettings settings);

    // Core Source Engine Math Functions
    void applyFriction(b2Vec2& velocity, float dt, bool isGrounded, bool jumpedThisFrame);
    void accelerate(b2Vec2& velocity, b2Vec2 wishDir, float wishSpeed, float accel, float dt);
    void airMove(b2Vec2& velocity, b2Vec2 wishDir, float dt);
    void groundMove(b2Vec2& velocity, b2Vec2 wishDir, float dt, bool jumpedThisFrame);

    MovementSettings settings;
};
