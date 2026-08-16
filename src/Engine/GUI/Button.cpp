#include <Engine/GUI/Button.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include <cstdint>

sf::SoundBuffer Button::sHoverBuffer;
sf::SoundBuffer Button::sClickBuffer;
std::optional<sf::Sound> Button::sSoundPool[16];
int Button::sNextSoundIndex = 0;
bool Button::sSoundsLoaded = false;

static void fadeOutBuffer(sf::SoundBuffer& buffer, float fadeTimeSeconds) {
    std::uint64_t sampleCount = buffer.getSampleCount();
    if (sampleCount == 0) return;
    unsigned int channels = buffer.getChannelCount();
    unsigned int sampleRate = buffer.getSampleRate();
    
    std::uint64_t fadeSamples = static_cast<std::uint64_t>(fadeTimeSeconds * sampleRate * channels);
    if (fadeSamples > sampleCount) fadeSamples = sampleCount;
    if (fadeSamples == 0) return;
    
    std::vector<std::int16_t> newSamples(buffer.getSamples(), buffer.getSamples() + sampleCount);
    
    std::uint64_t startIndex = sampleCount - fadeSamples;
    for (std::uint64_t i = 0; i < fadeSamples; ++i) {
        float factor = 1.0f - static_cast<float>(i) / fadeSamples;
        newSamples[startIndex + i] = static_cast<std::int16_t>(newSamples[startIndex + i] * factor);
    }
    
    buffer.loadFromSamples(newSamples.data(), sampleCount, channels, sampleRate, buffer.getChannelMap());
}

void Button::playSoundFromBuffer(const sf::SoundBuffer& buffer) {
    if (!sSoundsLoaded) return;
    
    if (!sSoundPool[sNextSoundIndex].has_value()) {
        sSoundPool[sNextSoundIndex].emplace(buffer);
    } else {
        sSoundPool[sNextSoundIndex]->setBuffer(buffer);
    }
    
    sSoundPool[sNextSoundIndex]->setVolume(20.f); // Reduce volume (50% or less since original might be loud)
    sSoundPool[sNextSoundIndex]->play();
    sNextSoundIndex = (sNextSoundIndex + 1) % 16;
}

Button::Button(const sf::Font &font, const std::string &text,
               sf::Vector2f position)
    : mText(font), mIsSelected(false) {
    
  if (!sSoundsLoaded) {
    if (!sHoverBuffer.loadFromFile("assets/sounds/buttonrollover.wav")) {
      std::cerr << "Failed to load buttonrollover.wav\n";
    } else {
      fadeOutBuffer(sHoverBuffer, 0.05f); // 50ms fade out
    }
    
    if (!sClickBuffer.loadFromFile("assets/sounds/buttonclickrelease.wav")) {
      std::cerr << "Failed to load buttonclickrelease.wav\n";
    } else {
      fadeOutBuffer(sClickBuffer, 0.1f); // 100ms fade out for the click to eliminate the pop
    }
    sSoundsLoaded = true;
  }

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
  if (selected && !mIsSelected) {
    playSoundFromBuffer(sHoverBuffer);
  }
  
  mIsSelected = selected;
  if (mIsSelected) {
    mText.setFillColor(sf::Color(255, 150, 0));
    mText.setScale({1.2f, 1.2f});
  } else {
    mText.setFillColor(sf::Color::White);
    mText.setScale({1.0f, 1.0f});
  }
}

void Button::playClickSound() {
  playSoundFromBuffer(sClickBuffer);
}

void Button::setText(const std::string& text) {
  mText.setString(text);
}

void Button::setPosition(sf::Vector2f position) { mText.setPosition(position); }

bool Button::contains(sf::Vector2f point) const {
  return mText.getGlobalBounds().contains(point);
}

void Button::render(sf::RenderWindow &window) { window.draw(mText); }
