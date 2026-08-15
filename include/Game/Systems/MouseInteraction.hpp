#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class MouseInteraction {
public:
  MouseInteraction();
  
  void init(b2WorldId world, float p2m);
  
  bool handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view);
  
  void update(float dt, const sf::RenderWindow& window, const sf::View& view);
  
  void render(sf::RenderWindow& window);

private:
  b2WorldId mWorld;
  float mP2M;
  
  bool mIsDragging;
  b2BodyId mDraggedBody;
  b2Vec2 mDragOffset;
  
  sf::CircleShape mCursorShape; // Visual indicator for dragging
};
