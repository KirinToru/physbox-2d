#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <Game/Entities/InteractiveProp2D.hpp>
#include <vector>
#include <memory>

class ObjectManager {
public:
  ObjectManager();
  
  void spawnBox(b2WorldId world, sf::Vector2f position, float P2M);
  void spawnBall(b2WorldId world, sf::Vector2f position, float P2M);
  void spawnTriangle(b2WorldId world, sf::Vector2f position, float P2M);
  void spawnStar(b2WorldId world, sf::Vector2f position, float P2M);
  
  void clear(b2WorldId world);
  
  void render(sf::RenderWindow& window, float P2M);
  
  const std::vector<std::unique_ptr<InteractiveProp2D>>& getProps() const { return mSpawnedProps; }
  std::vector<b2BodyId> getBodies() const;

private:
  std::vector<std::unique_ptr<InteractiveProp2D>> mSpawnedProps;
  std::vector<std::unique_ptr<sf::Shape>> mSpawnedShapes;
};
