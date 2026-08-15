#include <Game/Game.hpp>
#include <Game/States/GameState.hpp>
#include <Game/States/MenuState.hpp>
#include <iostream>

MenuState::MenuState(Game *game)
    : State(game), mSelectedOptionIndex(-1),
      mBackgroundSprite(mBackgroundTexture), mBackgroundOffset({0.f, 0.f}),
      mPopupText(mFont) {
  if (!mFont.openFromFile("assets/fonts/trebuc.ttf"))
    std::cerr << "Failed to load font in MenuState!" << std::endl;

  if (!mBackgroundTexture.loadFromFile("assets/backgrounds/bg.png"))
    std::cerr << "Failed to load bg.png in MenuState!" << std::endl;
  mBackgroundTexture.setRepeated(true);
  mBackgroundSprite.setTexture(mBackgroundTexture);
  mBackgroundSprite.setColor(sf::Color(100, 100, 100));

  mButtons.emplace_back(mFont, "New Game", sf::Vector2f{0, 0});
  mButtons.emplace_back(mFont, "Load Game", sf::Vector2f{0, 0});
  mButtons.emplace_back(mFont, "Options", sf::Vector2f{0, 0});
  mButtons.emplace_back(mFont, "Quit", sf::Vector2f{0, 0});

  mPopupRect.setFillColor(sf::Color(40, 40, 40, 220));
  mPopupRect.setOutlineColor(sf::Color(255, 150, 0));
  mPopupRect.setOutlineThickness(2.0f);
  
  mPopupTitleBar.setFillColor(sf::Color(60, 60, 60, 255));
  mPopupTitleBar.setOutlineColor(sf::Color(255, 150, 0));
  mPopupTitleBar.setOutlineThickness(2.0f);
  
  mPopupText.setFont(mFont);
  mPopupText.setCharacterSize(24);
  mPopupText.setFillColor(sf::Color::White);
  mPopupText.setString("Video Options");
  
  mPopupCloseButton = new Button(mFont, "Close", {0, 0});
  mPopupWindowModeButton = new Button(mFont, "Cycle Window Mode", {0, 0});

  mLastWindowSize = mGame->getWindow().getSize();
  updateLayout();
}

MenuState::~MenuState() {
    delete mPopupCloseButton;
    delete mPopupWindowModeButton;
}

void MenuState::updateLayout() {
  sf::Vector2f viewSize = mGame->getWindow().getDefaultView().getSize();

  float startY = viewSize.y - 250.f; // Bottom-left corner
  float xPos = 50.f; // Slightly closer to the edge

  for (size_t i = 0; i < mButtons.size(); ++i) {
    mButtons[i].setPosition({xPos, startY + (i * 45.f)});
  }
  
  mPopupRect.setSize({550.f, 300.f});
  mPopupRect.setOrigin({275.f, 150.f});
  mPopupRect.setPosition({viewSize.x / 2.f, viewSize.y / 2.f});
  
  mPopupTitleBar.setSize({550.f, 40.f});
  mPopupTitleBar.setOrigin({275.f, 0.f});
  
  updatePopupLayout();
}

void MenuState::updatePopupLayout() {
  auto pos = mPopupRect.getPosition();
  mPopupTitleBar.setPosition({pos.x, pos.y - 150.f});
  mPopupText.setPosition({pos.x - 250.f, pos.y - 90.f});
  mPopupWindowModeButton->setPosition({pos.x - 200.f, pos.y - 10.f});
  mPopupCloseButton->setPosition({pos.x - 200.f, pos.y + 70.f});
}

void MenuState::handleInput(sf::Event &event) {
  if (event.is<sf::Event::Resized>())
    updateLayout();

  if (const auto *mouseMove = event.getIf<sf::Event::MouseMoved>()) {
    sf::Vector2f mousePos = mGame->getWindow().mapPixelToCoords(
        mouseMove->position, mGame->getWindow().getDefaultView());

    if (mPopupOpen) {
        if (mIsDragging) {
            mPopupRect.setPosition(mousePos + mDragOffset);
            updatePopupLayout();
        }
        
        if (mPopupCloseButton->contains(mousePos)) {
            mPopupCloseButton->select(true);
        } else {
            mPopupCloseButton->select(false);
        }
        
        if (mPopupType == PopupType::Options) {
            if (mPopupWindowModeButton->contains(mousePos)) {
                mPopupWindowModeButton->select(true);
            } else {
                mPopupWindowModeButton->select(false);
            }
        }
    }
    
    // Allow hovering background buttons
    bool hoveredAny = false;
    for (int i = 0; i < mButtons.size(); ++i) {
      if (mButtons[i].contains(mousePos)) {
        if (mSelectedOptionIndex != i) {
          if (mSelectedOptionIndex >= 0) mButtons[mSelectedOptionIndex].select(false);
          mSelectedOptionIndex = i;
          mButtons[mSelectedOptionIndex].select(true);
        }
        hoveredAny = true;
      }
    }
    if (!hoveredAny && mSelectedOptionIndex >= 0) {
        mButtons[mSelectedOptionIndex].select(false);
        mSelectedOptionIndex = -1;
    }
  }

  if (const auto *mouseClick = event.getIf<sf::Event::MouseButtonPressed>()) {
    if (mouseClick->button == sf::Mouse::Button::Left) {
      sf::Vector2f mousePos = mGame->getWindow().mapPixelToCoords(
          mouseClick->position, mGame->getWindow().getDefaultView());
      if (mPopupOpen) {
          if (mPopupTitleBar.getGlobalBounds().contains(mousePos)) {
              mIsDragging = true;
              mDragOffset = mPopupRect.getPosition() - mousePos;
          }
      }
    }
  }

  if (const auto *mouseClick = event.getIf<sf::Event::MouseButtonReleased>()) {
    if (mouseClick->button == sf::Mouse::Button::Left) {
      sf::Vector2f mousePos = mGame->getWindow().mapPixelToCoords(
          mouseClick->position, mGame->getWindow().getDefaultView());
      
      mIsDragging = false;

      if (mPopupOpen) {
          if (mPopupCloseButton->contains(mousePos)) {
              mPopupOpen = false;
          }
          if (mPopupType == PopupType::Options && mPopupWindowModeButton->contains(mousePos)) {
              mGame->cycleWindowMode();
          }
      }
      
      // Allow clicking background buttons
      if (mSelectedOptionIndex >= 0 && mButtons[mSelectedOptionIndex].contains(mousePos)) {
          if (mSelectedOptionIndex == 0)
            mGame->changeState(std::make_unique<GameState>(mGame));
          else if (mSelectedOptionIndex == 1) { // Load Game
            mPopupOpen = true;
            mPopupType = PopupType::LoadGame;
          }
          else if (mSelectedOptionIndex == 2) { // Options
            mPopupOpen = true;
            mPopupType = PopupType::Options;
          }
          else if (mSelectedOptionIndex == 3)
            const_cast<sf::RenderWindow &>(mGame->getWindow()).close();
      }
    }
  }

  if (const auto *keyPress = event.getIf<sf::Event::KeyPressed>()) {
    if (mPopupOpen) {
        if (keyPress->code == sf::Keyboard::Key::Escape || keyPress->code == sf::Keyboard::Key::Enter) {
            mPopupOpen = false;
        }
        return;
    }

    if (keyPress->code == sf::Keyboard::Key::Up ||
        keyPress->code == sf::Keyboard::Key::W) {
      if (mSelectedOptionIndex >= 0) mButtons[mSelectedOptionIndex].select(false);
      mSelectedOptionIndex--;
      if (mSelectedOptionIndex < 0)
        mSelectedOptionIndex = static_cast<int>(mButtons.size()) - 1;
      mButtons[mSelectedOptionIndex].select(true);
    } else if (keyPress->code == sf::Keyboard::Key::Down ||
               keyPress->code == sf::Keyboard::Key::S) {
      if (mSelectedOptionIndex >= 0) mButtons[mSelectedOptionIndex].select(false);
      mSelectedOptionIndex =
          (mSelectedOptionIndex + 1) % static_cast<int>(mButtons.size());
      mButtons[mSelectedOptionIndex].select(true);
    } else if (keyPress->code == sf::Keyboard::Key::Enter) {
      if (mSelectedOptionIndex == 0)
        mGame->changeState(std::make_unique<GameState>(mGame));
      else if (mSelectedOptionIndex == 1) {
        mPopupOpen = true;
        mPopupType = PopupType::LoadGame;
      }
      else if (mSelectedOptionIndex == 2) {
        mPopupOpen = true;
        mPopupType = PopupType::Options;
      }
      else if (mSelectedOptionIndex == 3)
        const_cast<sf::RenderWindow &>(mGame->getWindow()).close();
    }
  }
}

void MenuState::update(sf::Time dt) {
  sf::Vector2u currentSize = mGame->getWindow().getSize();
  if (currentSize != mLastWindowSize) {
    updateLayout();
    mLastWindowSize = currentSize;
  }
  
  if (mPopupOpen) {
      if (mPopupType == PopupType::Options) {
          mPopupText.setString("Video Options");
          std::string modes[] = {"Windowed", "Borderless", "Fullscreen"};
          mPopupWindowModeButton->setText("Cycle Window Mode: " + modes[mGame->getWindowMode()]);
      } else if (mPopupType == PopupType::LoadGame) {
          mPopupText.setString("Load Game");
      }
  }

  float speedX = 20.f;
  float speedY = -5.f;
  mBackgroundOffset.x += speedX * dt.asSeconds();
  mBackgroundOffset.y += speedY * dt.asSeconds();
}

void MenuState::render(sf::RenderWindow &window) {
  window.setView(window.getDefaultView());
  sf::Vector2f size = window.getDefaultView().getSize();
  window.clear(sf::Color::Black);

  mBackgroundSprite.setTextureRect(
      sf::IntRect({static_cast<int>(mBackgroundOffset.x),
                   static_cast<int>(mBackgroundOffset.y)},
                  {static_cast<int>(size.x), static_cast<int>(size.y)}));
  window.draw(mBackgroundSprite);

  for (auto &button : mButtons)
    button.render(window);
    
  if (mPopupOpen) {
    window.draw(mPopupRect);
    window.draw(mPopupTitleBar);
    window.draw(mPopupText);
    if (mPopupType == PopupType::Options) {
        mPopupWindowModeButton->render(window);
    }
    mPopupCloseButton->render(window);
  }
}
