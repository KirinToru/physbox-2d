#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <Game/Entities/SourceMovementController2D.hpp>

class Player {
public:
  Player();

  void init(b2WorldId world, sf::Vector2f position, float P2M);

  void update(float dtSec);

  void render(sf::RenderWindow &window, bool showHitbox = false);

  void reset(b2WorldId world, sf::Vector2f position, float P2M);

  void applyForce(sf::Vector2f force);

  sf::Vector2f getPosition() const;
  sf::Vector2f getVelocity() const;

private:
  sf::RectangleShape shape;
  
  sf::Texture texture;
  sf::Sprite sprite;
  
  b2BodyId mBody;
  b2WorldId mWorld;
  float mP2M;
  bool mJumpHeld = false;

  bool facingRight;
  bool isGrounded;
  
  SourceMovementController2D movementController;
  bool jumpedThisFrame = false;

  // New Mechanics fields that were cleared out
  float dashSpeed;
  float dashDuration;
  float dashTimer;
  float dashCooldown;
  float dashCooldownTimer;
  bool isDashing;
  bool hasAirDash;
};