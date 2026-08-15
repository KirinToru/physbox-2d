#pragma once

#include <Engine/GUI/Button.hpp>
#include <Engine/States/State.hpp>
#include <vector>

class MenuState : public State {
public:
  MenuState(Game *game);
  ~MenuState();

  void handleInput(sf::Event &event) override;
  void update(sf::Time dt) override;
  void render(sf::RenderWindow &window) override;

private:
  sf::Font mFont;
  std::vector<Button> mButtons;
  int mSelectedOptionIndex;

  enum class PopupType { None, Options, LoadGame };
  PopupType mPopupType = PopupType::None;

  bool mPopupOpen = false;
  sf::RectangleShape mPopupRect;
  sf::RectangleShape mPopupTitleBar;
  sf::Text mPopupText;
  Button* mPopupCloseButton = nullptr;
  Button* mPopupWindowModeButton = nullptr;
  
  bool mIsDragging = false;
  sf::Vector2f mDragOffset;

  sf::Texture mBackgroundTexture;
  sf::Sprite mBackgroundSprite;
  sf::Vector2f mBackgroundOffset;
  sf::Vector2u mLastWindowSize;

  void updateLayout();
  void updatePopupLayout();
};
