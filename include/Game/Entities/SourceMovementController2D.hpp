#pragma once
#include <box2d/box2d.h>
#include <SFML/System/Vector2.hpp>

struct MovementSettings {
    float maxSpeed = 7.0f;
    float maxAirSpeed = 7.0f;
    float bhopSpeedLimit = 15.0f; // Max absolute speed allowed
    float groundAcceleration = 6.5f; // Must be slightly > friction for smooth, slow wind-up (was 14.0)
    float airAcceleration = 100.0f; // High for snappy air turns
    float friction = 6.0f; // Higher friction to reduce sliding (was 4.0f)
    float stopSpeed = 4.0f; // Higher stop speed to snap to zero (was 2.0f)
    float jumpImpulse = 11.0f; // Jump higher (was 7.9f)
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
