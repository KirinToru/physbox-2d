#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>

class WindSystem {
public:
  WindSystem();
  
  void update(float dtSec, b2WorldId world, const std::vector<b2BodyId>& bodies);
  void render(sf::RenderWindow& window);
  
  void setWindForce(sf::Vector2f force);
  sf::Vector2f getWindForce() const;

private:
  bool isExposedToWind(b2WorldId world, b2Vec2 targetPos, b2Vec2 windDir);

  sf::Vector2f mWindForce;
  float mWindVisualTimer;
  float mP2M;
  b2WorldId mWorld;
};
