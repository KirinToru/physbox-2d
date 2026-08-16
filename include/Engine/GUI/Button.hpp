#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <optional>
#include <list>

class Button {
public:
  Button(const sf::Font &font, const std::string &text, sf::Vector2f position);
  void select(bool selected);
  void setText(const std::string& text);
  void setPosition(sf::Vector2f position);
  bool contains(sf::Vector2f point) const;
  void render(sf::RenderWindow &window);
  
  static void playClickSound();

private:
  sf::Text mText;
  bool mIsSelected;
  
  static sf::SoundBuffer sHoverBuffer;
  static sf::SoundBuffer sClickBuffer;
  static std::optional<sf::Sound> sSoundPool[16];
  static int sNextSoundIndex;
  static bool sSoundsLoaded;
  
  static void playSoundFromBuffer(const sf::SoundBuffer& buffer);
};