#include <Game/Game.hpp>
#include <Game/States/MenuState.hpp>
#include <Game/States/GameState.hpp>
#include <Game/States/PauseState.hpp>
#include <iostream>
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game() : mWindow(sf::VideoMode({1280, 720}), "PhysBox 2D") {
  mWindow.setFramerateLimit(60);
  mWindow.setVerticalSyncEnabled(true);
  
  sf::Font consoleFont;
  if (!consoleFont.openFromFile("assets/fonts/trebuc.ttf")) {
    std::cerr << "Failed to load font for DevConsole!" << std::endl;
  }
  mConsole.init(consoleFont);
  
  mStates.push_back(std::make_unique<MenuState>(this));
}

void Game::pushState(std::unique_ptr<State> state) {
  mPendingChanges.push_back({Action::Push, std::move(state)});
}

void Game::popState() { mPendingChanges.push_back({Action::Pop, nullptr}); }

void Game::changeState(std::unique_ptr<State> state) {
  mPendingChanges.push_back({Action::Change, std::move(state)});
}

void Game::clearStatesAndPush(std::unique_ptr<State> state) {
  mPendingChanges.push_back({Action::ClearAndPush, std::move(state)});
}

void Game::applyPendingChanges() {
  for (auto &change : mPendingChanges) {
    switch (change.action) {
    case Action::Push:
      mStates.push_back(std::move(change.state));
      break;
    case Action::Pop:
      if (!mStates.empty())
        mStates.pop_back();
      break;
    case Action::Change:
      if (!mStates.empty())
        mStates.pop_back();
      mStates.push_back(std::move(change.state));
      break;
    case Action::ClearAndPush:
      mStates.clear();
      mStates.push_back(std::move(change.state));
      break;
    }
  }
  mPendingChanges.clear();
}

void Game::run() {
  sf::Clock clock;
  sf::Time timeSinceLastUpdate = sf::Time::Zero;

  while (mWindow.isOpen()) {
    sf::Time dt = clock.restart();
    timeSinceLastUpdate += dt;

    while (timeSinceLastUpdate > TimePerFrame) {
      timeSinceLastUpdate -= TimePerFrame;
      processEvents();
      update(TimePerFrame);
      applyPendingChanges();
    }
    render();
  }
}

void Game::processEvents() {
  while (const std::optional event = mWindow.pollEvent()) {
    if (event->is<sf::Event::Closed>())
      mWindow.close();

    if (const auto *keyPress = event->getIf<sf::Event::KeyPressed>()) {
      if (keyPress->code == sf::Keyboard::Key::F4) {
        if (keyPress->alt)
          mWindow.close(); // Alt+F4 closes the game
        else
          cycleWindowMode(); // F4 alone cycles window mode
      } else if (keyPress->code == sf::Keyboard::Key::Grave) {
        mConsole.toggle();
        if (mConsole.isOpen()) {
            if (!mStates.empty() && dynamic_cast<GameState*>(mStates.back().get())) {
                pushState(std::make_unique<PauseState>(this));
                mConsoleOpenedFromGame = true;
            } else {
                mConsoleOpenedFromGame = false;
            }
        } else {
            if (mConsoleOpenedFromGame && !mStates.empty() && dynamic_cast<PauseState*>(mStates.back().get())) {
                popState();
            }
            mConsoleOpenedFromGame = false;
        }
      }
    }
    
    if (mConsole.isOpen()) {
        mConsole.handleEvent(*event, mWindow);
    } else if (!mStates.empty()) {
      sf::Event ev = *event;
      mStates.back()->handleInput(ev);
    }
  }
}

void Game::update(sf::Time dt) {
  if (!mStates.empty())
    mStates.back()->update(dt);
}

void Game::render() {
  mWindow.clear(sf::Color::Black);
  for (const auto &state : mStates)
    state->render(mWindow);
  if (mConsole.isOpen()) mConsole.render(mWindow);
  mWindow.display();
}

void Game::cycleWindowMode() {
  mWindowMode = (mWindowMode + 1) % 3;

  switch (mWindowMode) {
  case 0:
    mWindow.create(sf::VideoMode({1280, 720}), "PhysBox 2D",
                   sf::Style::Default);
    break;
  case 1:
    mWindow.create(sf::VideoMode::getDesktopMode(), "PhysBox 2D",
                   sf::Style::None);
    break;
  case 2:
    mWindow.create(sf::VideoMode::getDesktopMode(), "PhysBox 2D",
                   sf::State::Fullscreen);
    break;
  }
  mWindow.setFramerateLimit(60);
  mWindow.setVerticalSyncEnabled(true);
}
