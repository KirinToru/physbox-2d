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
    // If the player changes direction in the air, flip the velocity to simulate a 180 degree air-strafe U-turn.
    // This preserves their absolute speed instead of slowing them down.
    if (wishDir.x != 0 && velocity.x * wishDir.x < 0) {
        velocity.x = -velocity.x;
    }

    // In Source, wishSpeed for air is capped to a small value (e.g. 30 units) compared to maxSpeed (320)
    // This allows the acceleration to work properly when turning. 
    // In 2D, we cap it similarly (about 10% of max speed)
    float wishSpeed = settings.maxAirSpeed;
    float airCap = settings.maxSpeed * 0.1f; 
    wishSpeed = std::min(wishSpeed, airCap);

    float currentSpeed = velocity.x * wishDir.x;
    float addSpeed = wishSpeed - currentSpeed;

    if (addSpeed <= 0) {
        // 2D Bhop hack: since we can't turn the mouse to bypass the projection cap, 
        // we artificially grant a small speed boost if they hold the direction they are moving.
        // This simulates perfect air-strafing. Constant 5.0 m/s^2 boost.
        if (std::abs(velocity.x) < settings.bhopSpeedLimit) {
            velocity.x += wishDir.x * 5.0f * dt;
        }
        return;
    }

    float accelSpeed = settings.airAcceleration * dt * wishSpeed;
    if (accelSpeed > addSpeed) {
        accelSpeed = addSpeed;
    }

    velocity.x += accelSpeed * wishDir.x;
}

void SourceMovementController2D::groundMove(b2Vec2& velocity, b2Vec2 wishDir, float dt, bool jumpedThisFrame) {
    applyFriction(velocity, dt, true, jumpedThisFrame);
    accelerate(velocity, wishDir, settings.maxSpeed, settings.groundAcceleration, dt);
}
