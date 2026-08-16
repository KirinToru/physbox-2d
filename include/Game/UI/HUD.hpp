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
  void toggleInfo();
  
  bool isHitboxVisible() const;
  bool isInfoVisible() const;

  void setPlayerSpeed(float speed);
  void setEntityCount(int count);
  void setFrameTime(float ms);

private:
  bool mShowHitbox;
  bool mShowFPS;

  sf::Font mFPSFont;
  bool mFPSFontLoaded;
  sf::Clock mFPSClock;
  int mFrameCount;
  int mCurrentFPS;
  float mPlayerSpeed;
  int mEntityCount;
  float mFrameTimeMs;
};
