#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <functional>
#include <optional>

class DevConsole {
public:
  DevConsole();

  void init(const sf::Font& font);
  
  void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
  void update(float dt);
  void render(sf::RenderWindow& window);

  void toggle();
  bool isOpen() const { return mIsOpen; }

  void addLog(const std::string& text);
  void clearLog();

  // Callback for when a command is entered
  void setCommandCallback(std::function<void(const std::string&)> callback);

private:
  bool mIsOpen;
  
  sf::Font mFont;
  
  std::string mInputBuffer;
  std::vector<std::string> mLog;

  sf::RectangleShape mBackground;
  sf::RectangleShape mTitleBar;
  sf::RectangleShape mInputBackground;
  
  std::optional<sf::Text> mInputText;
  std::optional<sf::Text> mLogText;

  std::function<void(const std::string&)> mCommandCallback;
  
  float mCursorBlinkTimer;
  bool mShowCursor;
  
  bool mIsDragging;
  sf::Vector2f mDragOffset;
  
  void updateLayout();
};
