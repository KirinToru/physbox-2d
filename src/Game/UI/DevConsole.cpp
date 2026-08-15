#include <Game/UI/DevConsole.hpp>
#include <iostream>

DevConsole::DevConsole() : mIsOpen(false), mCursorBlinkTimer(0.f), mShowCursor(true), mIsDragging(false) {
  // Wait for init to set up shapes
}

void DevConsole::init(const sf::Font& font) {
  mFont = font;

  mBackground.setFillColor(sf::Color(0, 0, 0, 220));
  mBackground.setSize({800.f, 400.f});
  mBackground.setPosition({240.f, 160.f});
  mBackground.setOutlineColor(sf::Color(255, 150, 0));
  mBackground.setOutlineThickness(2.0f);

  mTitleBar.setFillColor(sf::Color(60, 60, 60, 255));
  mTitleBar.setSize({800.f, 30.f});
  mTitleBar.setOutlineColor(sf::Color(255, 150, 0));
  mTitleBar.setOutlineThickness(2.0f);

  mInputBackground.setFillColor(sf::Color(50, 50, 50, 255));
  mInputBackground.setSize({800.f, 30.f});

  mInputText.emplace(mFont, "", 16);
  mInputText->setFillColor(sf::Color::White);

  mLogText.emplace(mFont, "", 14);
  mLogText->setFillColor(sf::Color(200, 200, 200));
  
  updateLayout();
  
  addLog("PhysBox 2D Developer Console initialized.");
  addLog("Type 'help' for a list of commands.");
}

void DevConsole::updateLayout() {
  auto pos = mBackground.getPosition();
  mTitleBar.setPosition({pos.x, pos.y - 30.f});
  mInputBackground.setPosition({pos.x, pos.y + 400.f - 30.f});
  mInputText->setPosition({pos.x + 10.f, pos.y + 400.f - 25.f});
  mLogText->setPosition({pos.x + 10.f, pos.y + 10.f});
}

void DevConsole::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
  if (!mIsOpen) return;

  if (const auto *mouseMove = event.getIf<sf::Event::MouseMoved>()) {
    if (mIsDragging) {
      sf::Vector2f mousePos = window.mapPixelToCoords({mouseMove->position.x, mouseMove->position.y}, window.getDefaultView());
      mBackground.setPosition(mousePos + mDragOffset);
      updateLayout();
    }
  }

  if (const auto *mouseClick = event.getIf<sf::Event::MouseButtonPressed>()) {
    if (mouseClick->button == sf::Mouse::Button::Left) {
      sf::Vector2f mousePos = window.mapPixelToCoords({mouseClick->position.x, mouseClick->position.y}, window.getDefaultView());
      if (mTitleBar.getGlobalBounds().contains(mousePos)) {
        mIsDragging = true;
        mDragOffset = mBackground.getPosition() - mousePos;
      }
    }
  }

  if (const auto *mouseClick = event.getIf<sf::Event::MouseButtonReleased>()) {
    if (mouseClick->button == sf::Mouse::Button::Left) {
      mIsDragging = false;
    }
  }

  if (auto* textEvent = event.getIf<sf::Event::TextEntered>()) {
    char32_t unicode = textEvent->unicode;
    
    // Ignore tilde (grave) as it toggles the console
    if (unicode == '`' || unicode == '~') return;

    if (unicode == '\b') { // Backspace
      if (!mInputBuffer.empty()) {
        mInputBuffer.pop_back();
      }
    } else if (unicode == '\r' || unicode == '\n') { // Enter
      if (!mInputBuffer.empty()) {
        std::string cmd = mInputBuffer;
        addLog("] " + cmd);
        mInputBuffer.clear();
        
        if (mCommandCallback) {
          mCommandCallback(cmd);
        }
      }
    } else if (unicode >= 32 && unicode < 128) {
      mInputBuffer += static_cast<char>(unicode);
    }
  }
}

void DevConsole::update(float dt) {
  if (!mIsOpen) return;

  mCursorBlinkTimer += dt;
  if (mCursorBlinkTimer > 0.5f) {
    mShowCursor = !mShowCursor;
    mCursorBlinkTimer = 0.f;
  }

  std::string displayStr = mInputBuffer;
  if (mShowCursor) displayStr += "_";
  if (mInputText) mInputText->setString(displayStr);

  // Build log string (showing last 15 lines)
  std::string logStr;
  size_t start = mLog.size() > 15 ? mLog.size() - 15 : 0;
  for (size_t i = start; i < mLog.size(); ++i) {
    logStr += mLog[i] + "\n";
  }
  if (mLogText) mLogText->setString(logStr);
}

void DevConsole::render(sf::RenderWindow& window) {
  if (!mIsOpen) return;
  
  // Need to save original view to draw UI properly over game view
  sf::View oldView = window.getView();
  window.setView(window.getDefaultView());

  window.draw(mBackground);
  window.draw(mTitleBar);
  window.draw(mInputBackground);
  if (mLogText) window.draw(*mLogText);
  if (mInputText) window.draw(*mInputText);

  window.setView(oldView);
}

void DevConsole::toggle() {
  mIsOpen = !mIsOpen;
}

void DevConsole::addLog(const std::string& text) {
  mLog.push_back(text);
}

void DevConsole::clearLog() {
  mLog.clear();
}

void DevConsole::setCommandCallback(std::function<void(const std::string&)> callback) {
  mCommandCallback = callback;
}
