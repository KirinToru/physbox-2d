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
  prop->init(world, position, {50.f, 50.f}, P2M, boxSettings, InteractiveProp2D::ShapeType::Box);

  auto boxShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(50.f, 50.f));
  boxShape->setOrigin({25.f, 25.f});
  boxShape->setFillColor(sf::Color(139, 69, 19)); // Brown
  boxShape->setOutlineColor(sf::Color::Black);
  boxShape->setOutlineThickness(2.f);

  mSpawnedProps.push_back(std::move(prop));
  mSpawnedShapes.push_back(std::move(boxShape));
}

void ObjectManager::spawnBall(b2WorldId world, sf::Vector2f position, float P2M) {
  PropPhysicsSettings ballSettings;
  ballSettings.density = 1.5f;
  ballSettings.friction = 0.3f;
  ballSettings.restitution = 0.8f; // Bouncy
  ballSettings.linearDamping = 1.0f;
  ballSettings.angularDamping = 0.2f;
  ballSettings.useCCD = true;
  ballSettings.damageMultiplier = 1.0f;

  auto prop = std::make_unique<InteractiveProp2D>();
  prop->init(world, position, {50.f, 50.f}, P2M, ballSettings, InteractiveProp2D::ShapeType::Circle);

  auto ballShape = std::make_unique<sf::CircleShape>(25.f);
  ballShape->setOrigin({25.f, 25.f});
  ballShape->setFillColor(sf::Color::Red); // Red
  ballShape->setOutlineColor(sf::Color::Black);
  ballShape->setOutlineThickness(2.f);

  mSpawnedProps.push_back(std::move(prop));
  mSpawnedShapes.push_back(std::move(ballShape));
}

void ObjectManager::spawnTriangle(b2WorldId world, sf::Vector2f position, float P2M) {
  PropPhysicsSettings triSettings;
  triSettings.density = 5.0f; // Heavy steel
  triSettings.friction = 0.4f;
  triSettings.restitution = 0.1f;
  triSettings.linearDamping = 1.5f;
  triSettings.angularDamping = 0.8f;
  triSettings.useCCD = true;
  triSettings.damageMultiplier = 2.0f; // Pointy and heavy

  auto prop = std::make_unique<InteractiveProp2D>();
  prop->init(world, position, {50.f, 50.f}, P2M, triSettings, InteractiveProp2D::ShapeType::Triangle);

  auto triShape = std::make_unique<sf::ConvexShape>();
  triShape->setPointCount(3);
  triShape->setPoint(0, {0.f, 0.f});
  triShape->setPoint(1, {50.f, 50.f});
  triShape->setPoint(2, {0.f, 50.f});
  triShape->setOrigin({25.f, 25.f});
  triShape->setFillColor(sf::Color(128, 128, 128)); // Grey
  triShape->setOutlineColor(sf::Color::Black);
  triShape->setOutlineThickness(2.f);

  mSpawnedProps.push_back(std::move(prop));
  mSpawnedShapes.push_back(std::move(triShape));
}

void ObjectManager::spawnStar(b2WorldId world, sf::Vector2f position, float P2M) {
  PropPhysicsSettings starSettings;
  starSettings.density = 0.5f; // Light
  starSettings.friction = 0.2f;
  starSettings.restitution = 0.5f;
  starSettings.linearDamping = 0.5f;
  starSettings.angularDamping = 0.1f;
  starSettings.useCCD = true;
  starSettings.damageMultiplier = 0.5f;

  auto prop = std::make_unique<InteractiveProp2D>();
  prop->init(world, position, {60.f, 60.f}, P2M, starSettings, InteractiveProp2D::ShapeType::Star);

  auto starShape = std::make_unique<sf::ConvexShape>();
  starShape->setPointCount(10);
  // Create a 5-point star
  float outerRadius = 30.f;
  float innerRadius = 12.f;
  for (int i = 0; i < 10; ++i) {
      float angle = i * 3.14159265f / 5.f - 3.14159265f / 2.f;
      float radius = (i % 2 == 0) ? outerRadius : innerRadius;
      starShape->setPoint(i, {30.f + radius * std::cos(angle), 30.f + radius * std::sin(angle)});
  }
  starShape->setOrigin({30.f, 30.f});
  starShape->setFillColor(sf::Color::Yellow); // Yellow
  starShape->setOutlineColor(sf::Color(255, 200, 0));
  starShape->setOutlineThickness(2.f);

  mSpawnedProps.push_back(std::move(prop));
  mSpawnedShapes.push_back(std::move(starShape));
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
