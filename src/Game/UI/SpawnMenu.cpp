#include <Game/UI/SpawnMenu.hpp>

SpawnMenu::SpawnMenu() : mIsOpen(false), mSelectedItem(ItemType::Box) {
}

void SpawnMenu::init(const sf::Font& font) {
  mFont = font;
  
  mBackground.setFillColor(sf::Color(30, 30, 30, 220));
  mBackground.setSize({800.f, 400.f});
  mBackground.setOrigin({400.f, 200.f});
  mBackground.setPosition({1280.f / 2.f, 720.f / 2.f});

  // Create items
  MenuItem boxItem{
    ItemType::Box,
    false,
    sf::RectangleShape({150.f, 150.f}),
    sf::CircleShape(),
    sf::Text(mFont, "Wooden Box", 16)
  };
  
  boxItem.bounds.setPosition({1280.f / 2.f - 200.f, 720.f / 2.f - 75.f});
  boxItem.bounds.setFillColor(sf::Color(139, 69, 19)); // SaddleBrown
  boxItem.bounds.setOutlineThickness(4.f);
  boxItem.label.setFillColor(sf::Color::White);
  boxItem.label.setPosition({boxItem.bounds.getPosition().x + 10.f, boxItem.bounds.getPosition().y + 160.f});
  
  MenuItem ballItem{
    ItemType::Ball,
    true,
    sf::RectangleShape(),
    sf::CircleShape(75.f, 60),
    sf::Text(mFont, "Bouncy Ball", 16)
  };
  
  ballItem.circleBounds.setPosition({1280.f / 2.f + 50.f, 720.f / 2.f - 75.f});
  ballItem.circleBounds.setFillColor(sf::Color(255, 69, 0)); // Red/Orange ball
  ballItem.circleBounds.setOutlineThickness(4.f);
  ballItem.label.setFillColor(sf::Color::White);
  ballItem.label.setPosition({ballItem.circleBounds.getPosition().x + 10.f, ballItem.circleBounds.getPosition().y + 160.f});

  mItems.push_back(boxItem);
  mItems.push_back(ballItem);
}

bool SpawnMenu::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
  if (!mIsOpen) return false;

  if (const auto* mPress = event.getIf<sf::Event::MouseButtonPressed>()) {
    if (mPress->button == sf::Mouse::Button::Left) {
      sf::Vector2f mousePos(static_cast<float>(mPress->position.x), static_cast<float>(mPress->position.y));
      
      for (auto& item : mItems) {
        sf::FloatRect bounds = item.isCircle ? item.circleBounds.getGlobalBounds() : item.bounds.getGlobalBounds();
        if (bounds.contains(mousePos)) {
          mSelectedItem = item.type;
          mIsOpen = false; // Close menu after selection
          return true; // Handled
        }
      }
      
      // If clicked outside items but on background, could do nothing or close
      if (!mBackground.getGlobalBounds().contains(mousePos)) {
        mIsOpen = false;
      }
      return true; // Absorb click if menu is open
    }
  }
  return false;
}

void SpawnMenu::update(const sf::RenderWindow& window) {
  if (!mIsOpen) return;
  
  sf::Vector2f mousePos(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y));
  
  for (auto& item : mItems) {
    bool isSelected = (item.type == mSelectedItem);
    sf::FloatRect bounds = item.isCircle ? item.circleBounds.getGlobalBounds() : item.bounds.getGlobalBounds();
    bool isHovered = bounds.contains(mousePos);
    
    sf::Color outlineColor = sf::Color::Transparent;
    if (isSelected) outlineColor = sf::Color::Green;
    else if (isHovered) outlineColor = sf::Color::Yellow;
    
    if (item.isCircle) item.circleBounds.setOutlineColor(outlineColor);
    else item.bounds.setOutlineColor(outlineColor);
  }
}

void SpawnMenu::render(sf::RenderWindow& window) {
  if (!mIsOpen) return;

  sf::View oldView = window.getView();
  window.setView(window.getDefaultView()); // Draw UI in screen space

  window.draw(mBackground);
  for (const auto& item : mItems) {
    if (item.isCircle) {
      window.draw(item.circleBounds);
    } else {
      window.draw(item.bounds);
    }
    window.draw(item.label);
  }

  window.setView(oldView);
}
