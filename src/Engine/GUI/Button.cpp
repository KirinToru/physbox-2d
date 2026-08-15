#include <Engine/GUI/Button.hpp>
#include <cmath>

Button::Button(const sf::Font &font, const std::string &text,
               sf::Vector2f position)
    : mText(font), mIsSelected(false) {
  mText.setString(text);
  mText.setCharacterSize(22);
  // Center origin vertically, left align horizontally
  sf::FloatRect bounds = mText.getLocalBounds();
  mText.setOrigin({0.f, std::floor(bounds.size.y / 2.f)});
  mText.setPosition(position);

  // Default color
  mText.setFillColor(sf::Color::White);
  mText.setOutlineColor(sf::Color::Black);
  mText.setOutlineThickness(1.f);
}

void Button::select(bool selected) {
  mIsSelected = selected;
  if (mIsSelected) {
    mText.setFillColor(sf::Color(255, 150, 0));
    mText.setScale({1.2f, 1.2f});
  } else {
    mText.setFillColor(sf::Color::White);
    mText.setScale({1.0f, 1.0f});
  }
}

void Button::setText(const std::string& text) {
  mText.setString(text);
}

void Button::setPosition(sf::Vector2f position) { mText.setPosition(position); }

bool Button::contains(sf::Vector2f point) const {
  return mText.getGlobalBounds().contains(point);
}

void Button::render(sf::RenderWindow &window) { window.draw(mText); }
