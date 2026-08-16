#include <Game/Systems/WindSystem.hpp>
#include <cmath>
#include <iostream>
#include <random>
#include <cstdint>

WindSystem::WindSystem() : mWindForce(0.f, 0.f), mSpawnTimer(0.f), mP2M(50.f) {}

void WindSystem::setWindForce(sf::Vector2f force) {
  mWindForce = force;
}

sf::Vector2f WindSystem::getWindForce() const {
  return mWindForce;
}

static float RayCastCallbackWind(b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void* context) {
    bool* hitStatic = static_cast<bool*>(context);
    
    b2BodyId bodyId = b2Shape_GetBody(shapeId);
    if (b2Body_GetType(bodyId) == b2_staticBody) {
        *hitStatic = true;
        return 0.0f;
    }
    
    return -1.0f;
}

bool WindSystem::isExposedToWind(b2WorldId world, b2Vec2 targetPos, b2Vec2 windDir) {
    if (b2Length(windDir) < 0.1f) return false;
    
    b2Vec2 normWind = b2Normalize(windDir);
    
    float rayLength = 50.0f;
    b2Pos rayOrigin = {targetPos.x - normWind.x * rayLength, targetPos.y - normWind.y * rayLength};
    b2Vec2 rayTranslation = {normWind.x * rayLength, normWind.y * rayLength};
    
    bool hitStatic = false;
    b2QueryFilter filter = b2DefaultQueryFilter();
    
    b2World_CastRay(world, rayOrigin, rayTranslation, filter, RayCastCallbackWind, &hitStatic);
    
    return !hitStatic;
}

void WindSystem::update(float dtSec, b2WorldId world, const std::vector<b2BodyId>& bodies, const sf::View& view) {
  mWorld = world;
  
  // Apply forces to bodies
  if (mWindForce.x != 0.f || mWindForce.y != 0.f) {
    for (b2BodyId bodyId : bodies) {
      if (b2Body_IsValid(bodyId)) {
        b2Vec2 pos = b2Body_GetPosition(bodyId);
        if (isExposedToWind(world, pos, {mWindForce.x, mWindForce.y})) {
          float mass = b2Body_GetMass(bodyId);
          b2Body_ApplyForceToCenter(bodyId, {mWindForce.x * mass * 0.5f, mWindForce.y * mass * 0.5f}, true);
        }
      }
    }
  }

  // Particle System Update
  float windLen = std::sqrt(mWindForce.x * mWindForce.x + mWindForce.y * mWindForce.y);
  if (windLen > 0.1f) {
    mSpawnTimer += dtSec;
    // Spawn more particles if wind is stronger
    float spawnRate = 0.02f / (windLen / 10.f); 
    if (mSpawnTimer > spawnRate) {
      mSpawnTimer = 0.f;
      
      sf::FloatRect viewBounds(view.getCenter() - view.getSize() / 2.f, view.getSize());
      sf::Vector2f normWind = mWindForce / windLen;
      
      // Calculate a spawn line perpendicular to wind, positioned upwind
      sf::Vector2f perp(-normWind.y, normWind.x);
      
      // Move spawn center outside the view upwind
      float maxDim = std::max(viewBounds.size.x, viewBounds.size.y);
      sf::Vector2f spawnCenter = view.getCenter() - normWind * (maxDim * 0.6f);
      
      // Random position along the perpendicular line
      static std::mt19937 rng(std::random_device{}());
      std::uniform_real_distribution<float> distOffset(-maxDim, maxDim);
      std::uniform_real_distribution<float> distSpeed(windLen * 3.f, windLen * 6.f);
      std::uniform_real_distribution<float> distLife(1.f, 3.f);
      std::uniform_real_distribution<float> distRot(-180.f, 180.f);
      
      float offset = distOffset(rng);
      sf::Vector2f spawnPos = spawnCenter + perp * offset;
      
      WindParticle p;
      p.position = spawnPos;
      // Add a bit of randomness to velocity
      std::uniform_real_distribution<float> distVelAdd(-20.f, 20.f);
      p.velocity = normWind * distSpeed(rng) + sf::Vector2f(distVelAdd(rng), distVelAdd(rng));
      p.rotation = distRot(rng);
      p.rotationSpeed = distRot(rng) * 2.f;
      p.life = distLife(rng);
      p.maxLife = p.life;
      
      // Pollen colors (light green/yellow)
      std::uniform_int_distribution<int> colorDist(0, 50);
      p.color = sf::Color(150 + colorDist(rng), 200 + colorDist(rng), 50 + colorDist(rng), 255);
      
      mParticles.push_back(p);
    }
  }

  // Update existing particles
  for (auto it = mParticles.begin(); it != mParticles.end(); ) {
    it->life -= dtSec;
    if (it->life <= 0.f) {
      it = mParticles.erase(it);
      continue;
    }
    
    // Add some turbulence/gravity to leaves/pollen
    it->velocity.y += 20.f * dtSec; // slight gravity
    
    it->position += it->velocity * dtSec;
    it->rotation += it->rotationSpeed * dtSec;
    
    // Check occlusion occasionally or use a simplified approach
    // We do it every frame for particles for accuracy, but it can be expensive.
    // For small number of particles, raycast is okay.
    b2Vec2 pB2 = {it->position.x / mP2M, it->position.y / mP2M};
    if (!isExposedToWind(mWorld, pB2, {mWindForce.x, mWindForce.y})) {
       // Quickly fade out if hitting a wall
       it->life -= dtSec * 5.f; 
    }
    
    ++it;
  }
}

void WindSystem::render(sf::RenderWindow& window) {
  if (mParticles.empty()) return;

  sf::VertexArray quads(sf::PrimitiveType::Triangles);
  
  for (const auto& p : mParticles) {
    float alpha = 255.f * (p.life / p.maxLife);
    if (alpha < 0) alpha = 0;
    if (alpha > 255) alpha = 255;
    
    sf::Color c = p.color;
    c.a = static_cast<std::uint8_t>(alpha);
    
    // Small quad for pollen/leaf
    float size = 4.f;
    sf::Transform transform;
    transform.translate(p.position).rotate(sf::degrees(p.rotation));
    
    sf::Vector2f p1 = transform.transformPoint(sf::Vector2f(-size, -size));
    sf::Vector2f p2 = transform.transformPoint(sf::Vector2f(size, -size));
    sf::Vector2f p3 = transform.transformPoint(sf::Vector2f(size, size));
    sf::Vector2f p4 = transform.transformPoint(sf::Vector2f(-size, size));
    
    // Triangle 1
    quads.append(sf::Vertex(p1, c));
    quads.append(sf::Vertex(p2, c));
    quads.append(sf::Vertex(p3, c));
    
    // Triangle 2
    quads.append(sf::Vertex(p1, c));
    quads.append(sf::Vertex(p3, c));
    quads.append(sf::Vertex(p4, c));
  }
  
  window.draw(quads);
}

