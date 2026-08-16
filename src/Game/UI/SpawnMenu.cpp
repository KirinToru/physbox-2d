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
    0, // rect
    sf::RectangleShape({100.f, 100.f}),
    sf::CircleShape(),
    sf::ConvexShape(),
    sf::Text(mFont, "Wooden Box", 16)
  };
  
  boxItem.bounds.setPosition({1280.f / 2.f - 300.f, 720.f / 2.f - 50.f});
  boxItem.bounds.setFillColor(sf::Color(139, 69, 19)); // SaddleBrown
  boxItem.bounds.setOutlineThickness(4.f);
  boxItem.label.setFillColor(sf::Color::White);
  boxItem.label.setPosition({boxItem.bounds.getPosition().x, boxItem.bounds.getPosition().y + 120.f});
  
  MenuItem ballItem{
    ItemType::Ball,
    1, // circle
    sf::RectangleShape(),
    sf::CircleShape(50.f, 60),
    sf::ConvexShape(),
    sf::Text(mFont, "Bouncy Ball", 16)
  };
  
  ballItem.circleBounds.setPosition({1280.f / 2.f - 150.f, 720.f / 2.f - 50.f});
  ballItem.circleBounds.setFillColor(sf::Color::Red);
  ballItem.circleBounds.setOutlineThickness(4.f);
  ballItem.label.setFillColor(sf::Color::White);
  ballItem.label.setPosition({ballItem.circleBounds.getPosition().x, ballItem.circleBounds.getPosition().y + 120.f});

  MenuItem triItem{
    ItemType::Triangle,
    2, // convex
    sf::RectangleShape(),
    sf::CircleShape(),
    sf::ConvexShape(3),
    sf::Text(mFont, "Steel Triangle", 16)
  };
  
  triItem.convexBounds.setPoint(0, {0.f, 0.f});
  triItem.convexBounds.setPoint(1, {100.f, 100.f});
  triItem.convexBounds.setPoint(2, {0.f, 100.f});
  triItem.convexBounds.setPosition({1280.f / 2.f + 50.f, 720.f / 2.f - 50.f});
  triItem.convexBounds.setFillColor(sf::Color(128, 128, 128));
  triItem.convexBounds.setOutlineThickness(4.f);
  triItem.label.setFillColor(sf::Color::White);
  triItem.label.setPosition({triItem.convexBounds.getPosition().x, triItem.convexBounds.getPosition().y + 120.f});

  MenuItem starItem{
    ItemType::Star,
    2, // convex
    sf::RectangleShape(),
    sf::CircleShape(),
    sf::ConvexShape(10),
    sf::Text(mFont, "Light Star", 16)
  };
  
  float outerRadius = 50.f;
  float innerRadius = 20.f;
  for (int i = 0; i < 10; ++i) {
      float angle = i * 3.14159265f / 5.f - 3.14159265f / 2.f;
      float radius = (i % 2 == 0) ? outerRadius : innerRadius;
      starItem.convexBounds.setPoint(i, {50.f + radius * std::cos(angle), 50.f + radius * std::sin(angle)});
  }
  starItem.convexBounds.setPosition({1280.f / 2.f + 200.f, 720.f / 2.f - 50.f});
  starItem.convexBounds.setFillColor(sf::Color::Yellow);
  starItem.convexBounds.setOutlineThickness(4.f);
  starItem.label.setFillColor(sf::Color::White);
  starItem.label.setPosition({starItem.convexBounds.getPosition().x, starItem.convexBounds.getPosition().y + 120.f});

  mItems.push_back(boxItem);
  mItems.push_back(ballItem);
  mItems.push_back(triItem);
  mItems.push_back(starItem);
}

bool SpawnMenu::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
  if (!mIsOpen) return false;

  if (const auto* mPress = event.getIf<sf::Event::MouseButtonPressed>()) {
    if (mPress->button == sf::Mouse::Button::Left) {
      sf::Vector2f mousePos(static_cast<float>(mPress->position.x), static_cast<float>(mPress->position.y));
      
      for (auto& item : mItems) {
        sf::FloatRect bounds;
        if (item.shapeType == 1) bounds = item.circleBounds.getGlobalBounds();
        else if (item.shapeType == 2) bounds = item.convexBounds.getGlobalBounds();
        else bounds = item.bounds.getGlobalBounds();

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
    sf::FloatRect bounds;
    if (item.shapeType == 1) bounds = item.circleBounds.getGlobalBounds();
    else if (item.shapeType == 2) bounds = item.convexBounds.getGlobalBounds();
    else bounds = item.bounds.getGlobalBounds();
    
    bool isHovered = bounds.contains(mousePos);
    
    sf::Color outlineColor = sf::Color::Transparent;
    if (isSelected) outlineColor = sf::Color::Green;
    else if (isHovered) outlineColor = sf::Color::Yellow;
    
    if (item.shapeType == 1) item.circleBounds.setOutlineColor(outlineColor);
    else if (item.shapeType == 2) item.convexBounds.setOutlineColor(outlineColor);
    else item.bounds.setOutlineColor(outlineColor);
  }
}

void SpawnMenu::render(sf::RenderWindow& window) {
  if (!mIsOpen) return;

  sf::View oldView = window.getView();
  window.setView(window.getDefaultView()); // Draw UI in screen space

  window.draw(mBackground);
  for (const auto& item : mItems) {
    if (item.shapeType == 1) {
      window.draw(item.circleBounds);
    } else if (item.shapeType == 2) {
      window.draw(item.convexBounds);
    } else {
      window.draw(item.bounds);
    }
    window.draw(item.label);
  }

  window.setView(oldView);
}
