#include <Game/UI/HUD.hpp>

HUD::HUD()
    : mShowHitbox(false),
      mShowFPS(true),
      mFPSFontLoaded(false),
      mFrameCount(0),
      mCurrentFPS(0),
      mPlayerSpeed(0.0f) {
      
  if (mFPSFont.openFromFile("C:/Windows/Fonts/arial.ttf")) {
    mFPSFontLoaded = true;
  } else {
    std::cerr << "Failed to load Arial font for HUD." << std::endl;
  }
}

void HUD::update(sf::Time dt) {
  if (mShowFPS) {
    mFrameCount++;
    if (mFPSClock.getElapsedTime().asSeconds() >= 1.0f) {
      mCurrentFPS = mFrameCount;
      mFrameCount = 0;
      mFPSClock.restart();
    }
  }
}

void HUD::render(sf::RenderWindow& window) {
  sf::View oldView = window.getView();
  window.setView(window.getDefaultView()); // Draw HUD in screen space

  if (mShowFPS && mFPSFontLoaded) {
    sf::Text infoText(mFPSFont, "FPS: " + std::to_string(mCurrentFPS) + "\nSpeed: " + std::to_string((int)mPlayerSpeed), 16);
    infoText.setFillColor(sf::Color::Yellow);
    infoText.setPosition({10.f, 10.f});
    window.draw(infoText);
  }

  window.setView(oldView);
}

void HUD::toggleHitbox() {
  mShowHitbox = !mShowHitbox;
}

void HUD::toggleInfo() {
  mShowFPS = !mShowFPS;
}

bool HUD::isHitboxVisible() const {
  return mShowHitbox;
}

bool HUD::isInfoVisible() const {
  return mShowFPS;
}

void HUD::setPlayerSpeed(float speed) {
  mPlayerSpeed = speed;
}
