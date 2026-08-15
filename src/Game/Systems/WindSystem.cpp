#include <Game/Systems/WindSystem.hpp>
#include <cmath>
#include <iostream>

WindSystem::WindSystem() : mWindForce(0.f, 0.f), mWindVisualTimer(0.f), mP2M(50.f) {}

void WindSystem::setWindForce(sf::Vector2f force) {
  mWindForce = force;
}

sf::Vector2f WindSystem::getWindForce() const {
  return mWindForce;
}

// Raycast callback to check if wind hits a static body before the target
static float RayCastCallbackWind(b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void* context) {
    bool* hitStatic = static_cast<bool*>(context);
    
    b2BodyId bodyId = b2Shape_GetBody(shapeId);
    if (b2Body_GetType(bodyId) == b2_staticBody) {
        *hitStatic = true;
        return 0.0f; // Terminate raycast
    }
    
    return -1.0f; // Continue raycast, ignore dynamic bodies
}

bool WindSystem::isExposedToWind(b2WorldId world, b2Vec2 targetPos, b2Vec2 windDir) {
    if (b2Length(windDir) < 0.1f) return false;
    
    b2Vec2 normWind = b2Normalize(windDir);
    
    // Cast from far upwind toward the target
    float rayLength = 50.0f; // meters
    b2Pos rayOrigin = {targetPos.x - normWind.x * rayLength, targetPos.y - normWind.y * rayLength};
    b2Vec2 rayTranslation = {normWind.x * rayLength, normWind.y * rayLength};
    
    bool hitStatic = false;
    b2QueryFilter filter = b2DefaultQueryFilter();
    
    b2World_CastRay(world, rayOrigin, rayTranslation, filter, RayCastCallbackWind, &hitStatic);
    
    return !hitStatic;
}

void WindSystem::update(float dtSec, b2WorldId world, const std::vector<b2BodyId>& bodies) {
  mWindVisualTimer += dtSec;
  mWorld = world;
  
  if (mWindForce.x != 0.f || mWindForce.y != 0.f) {
    for (b2BodyId bodyId : bodies) {
      if (b2Body_IsValid(bodyId)) {
        b2Vec2 pos = b2Body_GetPosition(bodyId);
        if (isExposedToWind(world, pos, {mWindForce.x, mWindForce.y})) {
          float mass = b2Body_GetMass(bodyId);
          // Scale wind force by mass so heavy objects (like the player) still move
          b2Body_ApplyForceToCenter(bodyId, {mWindForce.x * mass * 0.5f, mWindForce.y * mass * 0.5f}, true);
        }
      }
    }
  }
}

void WindSystem::render(sf::RenderWindow& window) {
  if (mWindForce.x == 0.f && mWindForce.y == 0.f) return;

  sf::View view = window.getView();
  sf::FloatRect viewBounds(view.getCenter() - view.getSize() / 2.f, view.getSize());

  sf::Vector2f windDir = mWindForce;
  float length = std::sqrt(windDir.x * windDir.x + windDir.y * windDir.y);
  if (length == 0.f) return;
  
  sf::Vector2f normWind = windDir / length;
  
  float spacing = 200.f; // Spacing between arrows
  float offset = std::fmod(mWindVisualTimer * 100.f, spacing); // Move arrows along direction
  
  sf::VertexArray arrows(sf::PrimitiveType::Lines);
  
  // Create a grid of points over the view
  for (float x = viewBounds.position.x - spacing; x < viewBounds.position.x + viewBounds.size.x + spacing; x += spacing) {
    for (float y = viewBounds.position.y - spacing; y < viewBounds.position.y + viewBounds.size.y + spacing; y += spacing) {
      // Shift point along wind direction based on time
      sf::Vector2f pos = sf::Vector2f(x, y) + normWind * offset;
      
      // We don't have access to world here easily for accurate raycast occlusion of visuals, 
      // but we could just draw them everywhere for now (or pass world).
      // For simplicity in refactoring, we'll draw them globally as before.
      
      // Check if this arrow position is exposed to wind
      b2Vec2 arrowPosB2 = {pos.x / mP2M, pos.y / mP2M};
      if (!isExposedToWind(mWorld, arrowPosB2, {mWindForce.x, mWindForce.y})) {
        continue; // Skip arrows behind walls
      }
      
      sf::Color windColor(200, 200, 200, 100);
      sf::Vector2f endPos = pos + normWind * 40.f;
      
      // Arrow line
      arrows.append(sf::Vertex(pos, windColor));
      arrows.append(sf::Vertex(endPos, windColor));
      
      // Arrow head
      sf::Vector2f perp(-normWind.y, normWind.x);
      sf::Vector2f head1 = endPos - normWind * 10.f + perp * 5.f;
      sf::Vector2f head2 = endPos - normWind * 10.f - perp * 5.f;
      
      arrows.append(sf::Vertex(endPos, windColor));
      arrows.append(sf::Vertex(head1, windColor));
      
      arrows.append(sf::Vertex(endPos, windColor));
      arrows.append(sf::Vertex(head2, windColor));
    }
  }
  
  window.draw(arrows);
}
