#pragma once

#include <Engine/States/State.hpp>
#include <Game/Entities/Player.hpp>
#include <box2d/box2d.h>
#include <Game/Systems/MouseInteraction.hpp>
#include <Game/UI/SpawnMenu.hpp>
#include <Game/Systems/WindSystem.hpp>
#include <Game/Systems/ObjectManager.hpp>
#include <Game/UI/HUD.hpp>
#include <vector>
#include <memory>

class GameState : public State {
public:
  GameState(Game *game);

  void handleInput(sf::Event &event) override;
  void update(sf::Time dt) override;
  void render(sf::RenderWindow &window) override;

private:
  void initPhysics();

  Player mPlayer;
  b2WorldId mWorld;

  sf::View mCamera;
  sf::Texture mBackgroundTexture;
  sf::Sprite mBackgroundSprite;
  sf::Font mFont;

  sf::RectangleShape mGroundShape;
  std::vector<sf::RectangleShape> mStaticShapes;

  MouseInteraction mMouseInteraction;
  SpawnMenu mSpawnMenu;
  HUD mHUD;

  WindSystem mWindSystem;
  ObjectManager mObjectManager;
  
  float mPhysicsAccumulator = 0.0f;
};
