#include <Game/Entities/Player.hpp>
#include <Game/World/Map.hpp>
#include <iostream>
#include <cmath>

Player::Player() : sprite(texture) {
  if (!texture.loadFromFile("assets/player/spritesheet.png")) {
    std::cerr << "Failed to load player texture!" << std::endl;
  }
  sprite.setTexture(texture, true);
  sprite.setTextureRect(sf::IntRect({0, 0}, {32, 32}));
  sprite.setOrigin({16.f, 32.f});

  facingRight = true;
  isGrounded = false;
  
  // Clear old dash stuff
  dashSpeed = 0.f;
  dashDuration = 0.f;
  dashTimer = 0.f;
  dashCooldown = 0.f;
  dashCooldownTimer = 0.f;
  isDashing = false;
  hasAirDash = false;
}

void Player::init(b2WorldId world, sf::Vector2f position, float P2M) {
  mP2M = P2M;
  mWorld = world;
  
  b2BodyDef bodyDef = b2DefaultBodyDef();
  bodyDef.type = b2_dynamicBody;
  bodyDef.position = {position.x / P2M, position.y / P2M};
  bodyDef.motionLocks.angularZ = true; // Player shouldn't tip over
  mBody = b2CreateBody(world, &bodyDef);
  
  // Use a box shape for the player (capsule causes jitter on flat ground)
  b2Polygon playerBox = b2MakeRoundedBox(12.f / 2.f / P2M, 28.f / 2.f / P2M, 2.f / P2M);
  
  b2ShapeDef shapeDef = b2DefaultShapeDef();
  shapeDef.density = 2.0f;
  shapeDef.material.friction = 0.0f; // Zero friction to prevent wall climbing
  shapeDef.material.restitution = 0.0f;
  
  b2CreatePolygonShape(mBody, &shapeDef, &playerBox);
  
  shape.setSize({14.f, 30.f});
  shape.setOrigin({7.f, 15.f});
}

void Player::update(float dtSec) {
  b2Vec2 velocity = b2Body_GetLinearVelocity(mBody);
  
  bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ||
              sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
  bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
  bool jumpPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
  
  // Ground check via 3 raycasts downward from feet (left, center, right)
  b2Vec2 pos = b2Body_GetPosition(mBody);
  float halfHeight = 28.f / 2.f / mP2M;
  float halfWidth = 10.f / 2.f / mP2M; // Slightly less than actual width (12.f) to not catch walls
  
  b2Vec2 rayTranslation = {0.0f, 4.0f / mP2M}; // Cast 4 pixels down
  b2QueryFilter filter = b2DefaultQueryFilter();
  
  b2Pos origins[3] = {
    {pos.x - halfWidth, pos.y + halfHeight},
    {pos.x, pos.y + halfHeight},
    {pos.x + halfWidth, pos.y + halfHeight}
  };
  
  isGrounded = false;
  for (int i = 0; i < 3; ++i) {
    b2RayResult rayResult = b2World_CastRayClosest(mWorld, origins[i], rayTranslation, filter);
    if (rayResult.hit && !B2_ID_EQUALS(b2Shape_GetBody(rayResult.shapeId), mBody)) {
      isGrounded = true;
      break;
    }
  }
  
  jumpedThisFrame = false;
  if (jumpPressed && isGrounded && !mJumpHeld) {
    velocity.y = -movementController.settings.jumpImpulse;
    jumpedThisFrame = true;
  }
  mJumpHeld = jumpPressed;

  // Source-style movement
  b2Vec2 wishDir = {0.0f, 0.0f};
  if (left && !right) {
    wishDir.x = -1.0f;
    facingRight = false;
  } else if (right && !left) {
    wishDir.x = 1.0f;
    facingRight = true;
  }
  
  if (isGrounded) {
    movementController.groundMove(velocity, wishDir, dtSec, jumpedThisFrame);
  } else {
    movementController.airMove(velocity, wishDir, dtSec);
  }

  b2Body_SetLinearVelocity(mBody, velocity);
  
  // Update SFML graphics position from raycast pos (already fetched above)
  shape.setPosition({pos.x * mP2M, pos.y * mP2M});
  
  sf::Vector2f bottomCenter = {shape.getPosition().x,
                               shape.getPosition().y + shape.getSize().y / 2.f};
  sprite.setPosition(bottomCenter);
  
  if (facingRight) {
    sprite.setScale({1.5f, 1.5f});
  } else {
    sprite.setScale({-1.5f, 1.5f});
  }
  
  // Dummy animation for now
  sprite.setTextureRect(sf::IntRect({0, 0}, {32, 32}));
}

void Player::render(sf::RenderWindow &window, bool showHitbox) {
  window.draw(sprite);
  if (showHitbox) {
    sf::RectangleShape hitboxVis = shape;
    hitboxVis.setFillColor(sf::Color(0, 255, 0, 100));
    hitboxVis.setOutlineColor(sf::Color::Green);
    hitboxVis.setOutlineThickness(1.f);
    window.draw(hitboxVis);
  }
}

void Player::reset(b2WorldId world, sf::Vector2f position, float P2M) {
  b2Rot rot = {1.0f, 0.0f}; // Identity rotation
  b2Body_SetTransform(mBody, {position.x / P2M, position.y / P2M}, rot);
  b2Body_SetLinearVelocity(mBody, {0.0f, 0.0f});
}

sf::Vector2f Player::getPosition() const {
  b2Vec2 pos = b2Body_GetPosition(mBody);
  return {pos.x * mP2M, pos.y * mP2M};
}

sf::Vector2f Player::getVelocity() const {
  b2Vec2 vel = b2Body_GetLinearVelocity(mBody);
  return {vel.x * mP2M, vel.y * mP2M};
}

void Player::applyForce(sf::Vector2f force) {
  b2Body_ApplyForceToCenter(mBody, {force.x, force.y}, true);
}
