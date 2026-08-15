#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class HUD {
public:
  HUD();
  
  void update(sf::Time dt);
  void render(sf::RenderWindow& window);

  void toggleHitbox();
  void toggleFPS();
  
  bool isHitboxVisible() const;

private:
  bool mShowHitbox;
  bool mShowFPS;

  sf::Font mFPSFont;
  bool mFPSFontLoaded;
  sf::Clock mFPSClock;
  int mFrameCount;
  int mCurrentFPS;
};
