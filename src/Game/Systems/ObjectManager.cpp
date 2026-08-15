#include <Game/Systems/ObjectManager.hpp>

ObjectManager::ObjectManager() {}

void ObjectManager::spawnBox(b2WorldId world, sf::Vector2f position, float P2M) {
  PropPhysicsSettings boxSettings;
  boxSettings.density = 2.0f;
  boxSettings.friction = 0.5f;
  boxSettings.restitution = 0.0f; // Heavy thud
  boxSettings.linearDamping = 2.0f; // High air resistance
  boxSettings.angularDamping = 0.5f; // Keep spin
  boxSettings.useCCD = true;
  boxSettings.damageMultiplier = 1.5f;

  auto prop = std::make_unique<InteractiveProp2D>();
  prop->init(world, position, {50.f, 50.f}, P2M, boxSettings, false);

  auto boxShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(50.f, 50.f));
  boxShape->setOrigin({25.f, 25.f});
  boxShape->setFillColor(sf::Color(139, 69, 19));
  boxShape->setOutlineColor(sf::Color::Black);
  boxShape->setOutlineThickness(2.f);

  mSpawnedProps.push_back(std::move(prop));
  mSpawnedShapes.push_back(std::move(boxShape));
}

void ObjectManager::spawnBall(b2WorldId world, sf::Vector2f position, float P2M) {
  PropPhysicsSettings ballSettings;
  ballSettings.density = 1.5f;
  ballSettings.friction = 0.3f;
  ballSettings.restitution = 0.0f;
  ballSettings.linearDamping = 1.0f;
  ballSettings.angularDamping = 0.2f;
  ballSettings.useCCD = true;
  ballSettings.damageMultiplier = 1.0f;

  auto prop = std::make_unique<InteractiveProp2D>();
  prop->init(world, position, {50.f, 50.f}, P2M, ballSettings, true);

  auto ballShape = std::make_unique<sf::CircleShape>(25.f);
  ballShape->setOrigin({25.f, 25.f});
  ballShape->setFillColor(sf::Color(255, 69, 0));
  ballShape->setOutlineColor(sf::Color::Black);
  ballShape->setOutlineThickness(2.f);

  mSpawnedProps.push_back(std::move(prop));
  mSpawnedShapes.push_back(std::move(ballShape));
}

void ObjectManager::clear(b2WorldId world) {
  for (auto& prop : mSpawnedProps) {
    if (b2Body_IsValid(prop->mBody)) {
      b2DestroyBody(prop->mBody);
    }
  }
  mSpawnedProps.clear();
  mSpawnedShapes.clear();
}

void ObjectManager::render(sf::RenderWindow& window, float P2M) {
  for (size_t i = 0; i < mSpawnedProps.size(); ++i) {
    b2BodyId bodyId = mSpawnedProps[i]->mBody;
    if (b2Body_IsValid(bodyId)) {
      b2Vec2 pos = b2Body_GetPosition(bodyId);
      b2Rot rot = b2Body_GetRotation(bodyId);
      
      mSpawnedShapes[i]->setPosition({pos.x * P2M, pos.y * P2M});
      mSpawnedShapes[i]->setRotation(sf::radians(b2Rot_GetAngle(rot)));
      window.draw(*mSpawnedShapes[i]);
    }
  }
}

std::vector<b2BodyId> ObjectManager::getBodies() const {
  std::vector<b2BodyId> bodies;
  bodies.reserve(mSpawnedProps.size());
  for (const auto& prop : mSpawnedProps) {
    bodies.push_back(prop->mBody);
  }
  return bodies;
}
