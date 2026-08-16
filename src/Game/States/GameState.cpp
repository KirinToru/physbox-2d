#include <Game/States/GameState.hpp>
#include <Game/States/MenuState.hpp>
#include <Game/States/PauseState.hpp>
#include <Game/States/GameState.hpp>
#include <Game/States/MenuState.hpp>
#include <Game/States/PauseState.hpp>
#include <Game/Game.hpp>
#include <iostream>
#include <sstream>

GameState::GameState(Game *game)
    : State(game), mCamera({0.f, 0.f}, {1280.f, 720.f}),
      mMouseInteraction(),
      mSpawnMenu(), mBackgroundSprite(mBackgroundTexture) {

  if (!mBackgroundTexture.loadFromFile("assets/backgrounds/bg.png")) {
    std::cerr << "Failed to load bg.png" << std::endl;
  }
  mBackgroundTexture.setRepeated(true);
  mBackgroundSprite.setTexture(mBackgroundTexture);
  mBackgroundSprite.setTextureRect(sf::IntRect({0, 0}, {8000, 6000}));
  (void)mFont.openFromFile("assets/fonts/trebuc.ttf");
  mBackgroundSprite.setPosition({-4000.f, -3000.f});

  initPhysics();

  mSpawnMenu.init(mFont);

  mGame->getConsole().setCommandCallback([this](const std::string& commandLine) {
    std::istringstream iss(commandLine);
    std::string cmd;
    iss >> cmd;
    
    if (cmd == "gravity") {
      float gx, gy;
      if (iss >> gx >> gy) {
        b2World_SetGravity(mWorld, {gx, gy});
        mGame->getConsole().addLog("Gravity set to " + std::to_string(gx) + ", " + std::to_string(gy));
      } else {
        mGame->getConsole().addLog("Usage: gravity <x> <y>");
      }
    } else if (cmd == "wind") {
      float wx, wy;
      if (iss >> wx >> wy) {
        mWindSystem.setWindForce({wx, wy});
        mGame->getConsole().addLog("Wind set to " + std::to_string(wx) + ", " + std::to_string(wy));
      } else {
        mGame->getConsole().addLog("Usage: wind <x> <y>");
      }
    } else if (cmd == "autojump") {
      int val;
      if (iss >> val) {
        mPlayer.setAutoJump(val != 0);
        mGame->getConsole().addLog("Autojump set to " + std::to_string(val != 0));
      } else {
        mGame->getConsole().addLog("Usage: autojump <0|1>");
      }
    } else if (cmd == "info") {
      int val;
      if (iss >> val) {
        if ((val != 0) != mHUD.isInfoVisible()) mHUD.toggleInfo();
        mGame->getConsole().addLog("Info set to " + std::to_string(val != 0));
      } else {
        mGame->getConsole().addLog("Usage: info <0|1>");
      }
    } else if (cmd == "clear") {
      mGame->getConsole().clearLog();
    } else if (cmd == "help") {
      mGame->getConsole().addLog("Available commands:");
      mGame->getConsole().addLog("  help           - Show this help message");
      mGame->getConsole().addLog("  clear          - Clear the console history");
      mGame->getConsole().addLog("  gravity <x> <y> - Set the world gravity forces");
      mGame->getConsole().addLog("  wind <x> <y>   - Set global wind force");
      mGame->getConsole().addLog("  autojump <0|1> - Toggle auto-bunnyhop");
      mGame->getConsole().addLog("  info <0|1>     - Toggle FPS and speed display");
    } else {
      mGame->getConsole().addLog("Unknown command: " + cmd);
    }
  });
}

void GameState::initPhysics() {
  b2WorldDef worldDef = b2DefaultWorldDef();
  worldDef.gravity = {0.0f, 25.0f};
  mWorld = b2CreateWorld(&worldDef);

  mMouseInteraction.init(mWorld, 50.0f);

  float P2M = 50.0f;

  mPlayer.init(mWorld, {200.f, 400.f}, P2M);

  b2ShapeDef staticShapeDef = b2DefaultShapeDef();
  staticShapeDef.material.friction = 0.5f;

  // Helper lambda to create a static box with visuals
  auto createStaticBox = [&](float cx, float cy, float w, float h, sf::Color color) {
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = {cx / P2M, cy / P2M};
    b2BodyId bodyId = b2CreateBody(mWorld, &bodyDef);
    b2Polygon box = b2MakeBox(w / 2.f / P2M, h / 2.f / P2M);
    b2CreatePolygonShape(bodyId, &staticShapeDef, &box);

    sf::RectangleShape shape({w, h});
    shape.setOrigin({w / 2.f, h / 2.f});
    shape.setPosition({cx, cy});
    shape.setFillColor(color);
    mStaticShapes.push_back(shape);
  };

  sf::Color wallColor(100, 100, 100);
  sf::Color shelterColor(80, 80, 80);

  // Ground - wide platform
  createStaticBox(960.f, 600.f, 1920.f, 20.f, wallColor);

  // Left boundary wall (short)
  createStaticBox(0.f, 500.f, 20.f, 220.f, wallColor);

  // Right boundary wall (short) 
  createStaticBox(1920.f, 500.f, 20.f, 220.f, wallColor);

  // Small shelter in the middle for wind testing:
  // Vertical wall
  createStaticBox(800.f, 530.f, 20.f, 120.f, shelterColor);
  // Small roof on top
  createStaticBox(850.f, 470.f, 120.f, 15.f, shelterColor);

  mObjectManager.spawnBox(mWorld, {600.f, 400.f}, P2M);
  mObjectManager.spawnBall(mWorld, {700.f, 400.f}, P2M);
  mObjectManager.spawnBox(mWorld, {900.f, 400.f}, P2M); // Box behind shelter
}

void GameState::handleInput(sf::Event &event) {
  if (const auto* keyPress = event.getIf<sf::Event::KeyPressed>()) {
    if (keyPress->code == sf::Keyboard::Key::Escape) {
      if (mSpawnMenu.isOpen()) {
        mSpawnMenu.setOpen(false);
      } else {
        mGame->pushState(std::make_unique<PauseState>(mGame));
      }
    }
    
    if (keyPress->code == sf::Keyboard::Key::Grave || keyPress->code == sf::Keyboard::Key::Grave) {
      mGame->getConsole().toggle();
    }
    
    if (keyPress->code == sf::Keyboard::Key::Q && !mGame->getConsole().isOpen()) {
      mSpawnMenu.setOpen(!mSpawnMenu.isOpen());
    }
  }
  
  if (mSpawnMenu.handleEvent(event, mGame->getWindow())) {
    return;
  }

  if (const auto* mousePress = event.getIf<sf::Event::MouseButtonPressed>()) {
    if (mousePress->button == sf::Mouse::Button::Right && !mSpawnMenu.isOpen() && !mGame->getConsole().isOpen()) {
      sf::Vector2i pixelPos = {mousePress->position.x, mousePress->position.y};
      sf::Vector2f worldPos = mGame->getWindow().mapPixelToCoords(pixelPos, mCamera);
      
      if (mSpawnMenu.getSelectedItem() == SpawnMenu::ItemType::Box) {
        mObjectManager.spawnBox(mWorld, worldPos, 50.0f);
      } else if (mSpawnMenu.getSelectedItem() == SpawnMenu::ItemType::Ball) {
        mObjectManager.spawnBall(mWorld, worldPos, 50.0f);
      } else if (mSpawnMenu.getSelectedItem() == SpawnMenu::ItemType::Triangle) {
        mObjectManager.spawnTriangle(mWorld, worldPos, 50.0f);
      } else if (mSpawnMenu.getSelectedItem() == SpawnMenu::ItemType::Star) {
        mObjectManager.spawnStar(mWorld, worldPos, 50.0f);
      }
    }
  }

  if (!mSpawnMenu.isOpen() && !mGame->getConsole().isOpen()) {
    mMouseInteraction.handleEvent(event, mGame->getWindow(), mCamera);
  }
}

void GameState::update(sf::Time dt) {
  float dtSec = dt.asSeconds();
  
  sf::Vector2f vel = mPlayer.getVelocity();
  mHUD.setPlayerSpeed(std::abs(vel.x)); // Pass horizontal speed to HUD
  mHUD.setEntityCount(mObjectManager.getBodies().size() + 1); // +1 for player
  mHUD.update(dt);

  if (!mGame->getConsole().isOpen()) {
    // Fixed physics timestep for stability
    const float fixedDt = 1.0f / 60.0f;
    mPhysicsAccumulator += dtSec;
    while (mPhysicsAccumulator >= fixedDt) {
      std::vector<b2BodyId> windBodies = mObjectManager.getBodies();
      windBodies.push_back(mPlayer.getBody());
      mWindSystem.update(fixedDt, mWorld, windBodies, mCamera);
      
      mPlayer.update(fixedDt);
      b2World_Step(mWorld, fixedDt, 12);
      mPhysicsAccumulator -= fixedDt;
    }
    mMouseInteraction.update(dtSec, mGame->getWindow(), mCamera);
  }

  sf::Vector2f targetPos = mPlayer.getPosition();
  sf::Vector2f currentPos = mCamera.getCenter();
  mCamera.setCenter(currentPos + (targetPos - currentPos) * 5.0f * dtSec);
}

void GameState::render(sf::RenderWindow &window) {
  window.setView(mCamera);
  
  window.draw(mBackgroundSprite);
  
  for (const auto& shape : mStaticShapes) {
    window.draw(shape);
  }

  mObjectManager.render(window, 50.0f);
  mPlayer.render(window, mHUD.isHitboxVisible());
  mWindSystem.render(window);
  mMouseInteraction.render(window);

  mHUD.render(window);
  mSpawnMenu.render(window);
}
