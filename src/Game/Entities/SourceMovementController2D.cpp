#include <Game/Entities/SourceMovementController2D.hpp>
#include <cmath>
#include <algorithm>

SourceMovementController2D::SourceMovementController2D() {}

SourceMovementController2D::SourceMovementController2D(MovementSettings settings) : settings(settings) {}

void SourceMovementController2D::applyFriction(b2Vec2& velocity, float dt, bool isGrounded, bool jumpedThisFrame) {
    if (!isGrounded || jumpedThisFrame) return;

    // In side-view, we only apply friction to the horizontal axis for standard movement
    // Vertical is handled by gravity and collisions
    float speed = std::abs(velocity.x);
    if (speed < 0.1f) {
        velocity.x = 0.0f;
        return;
    }

    float control = (speed < settings.stopSpeed) ? settings.stopSpeed : speed;
    float drop = control * settings.friction * dt;

    float newSpeed = speed - drop;
    if (newSpeed < 0) newSpeed = 0;
    newSpeed /= speed;

    velocity.x *= newSpeed;
}

void SourceMovementController2D::accelerate(b2Vec2& velocity, b2Vec2 wishDir, float wishSpeed, float accel, float dt) {
    // currentspeed = DotProduct(Velocity, WishDirection)
    // We only care about horizontal for 2D side-view air strafing along X, but if wishDir has Y it applies there too.
    // In a pure 2D platformer, wishDir is usually (1, 0) or (-1, 0).
    float currentSpeed = velocity.x * wishDir.x + velocity.y * wishDir.y;
    
    // addspeed = WishSpeed - currentspeed
    float addSpeed = wishSpeed - currentSpeed;
    if (addSpeed <= 0) return;

    // accelspeed = AccelerationRate * DeltaTime * WishSpeed
    float accelSpeed = accel * dt * wishSpeed;
    
    // Clamp accelspeed so it does not exceed addspeed
    if (accelSpeed > addSpeed) {
        accelSpeed = addSpeed;
    }

    velocity.x += accelSpeed * wishDir.x;
    velocity.y += accelSpeed * wishDir.y;
}

void SourceMovementController2D::airMove(b2Vec2& velocity, b2Vec2 wishDir, float dt) {
    float wishSpeed = settings.maxAirSpeed;
    // We can allow wishDir to be arbitrary if mouse aiming is used, but for now it's just X
    accelerate(velocity, wishDir, wishSpeed, settings.airAcceleration, dt);
}

void SourceMovementController2D::groundMove(b2Vec2& velocity, b2Vec2 wishDir, float dt, bool jumpedThisFrame) {
    applyFriction(velocity, dt, true, jumpedThisFrame);
    accelerate(velocity, wishDir, settings.maxSpeed, settings.groundAcceleration, dt);
}
