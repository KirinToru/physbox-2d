#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class SpawnMenu {
public:
  enum class ItemType { None, Box, Ball, Triangle, Star };

  SpawnMenu();
  void init(const sf::Font& font);
  
  void update(const sf::RenderWindow& window);
  void render(sf::RenderWindow& window);
  
  bool handleEvent(const sf::Event& event, const sf::RenderWindow& window);

  bool isOpen() const { return mIsOpen; }
  void setOpen(bool open) { mIsOpen = open; }

  ItemType getSelectedItem() const { return mSelectedItem; }

private:
  bool mIsOpen;
  ItemType mSelectedItem;

  sf::RectangleShape mBackground;
  
  struct MenuItem {
    ItemType type;
    int shapeType; // 0 = rect, 1 = circle, 2 = convex
    sf::RectangleShape bounds;
    sf::CircleShape circleBounds;
    sf::ConvexShape convexBounds;
    sf::Text label;
  };
  
  std::vector<MenuItem> mItems;
  sf::Font mFont;
};
