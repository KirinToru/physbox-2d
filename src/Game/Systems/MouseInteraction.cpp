#include <Game/Systems/MouseInteraction.hpp>
#include <iostream>

struct QueryContext {
  b2Pos point;
  b2ShapeId hitShape;
  bool hit;
};

// Callback for b2World_OverlapAABB
static bool queryCallback(b2ShapeId shapeId, void* context) {
  QueryContext* qc = static_cast<QueryContext*>(context);
  
  if (b2Shape_TestPoint(shapeId, qc->point)) {
    qc->hitShape = shapeId;
    qc->hit = true;
    return false; // Stop searching
  }
  return true; // Continue searching
}

MouseInteraction::MouseInteraction() : mIsDragging(false) {
  mCursorShape.setRadius(5.f);
  mCursorShape.setOrigin({5.f, 5.f});
  mCursorShape.setFillColor(sf::Color(255, 255, 255, 150));
  mDraggedBody = b2_nullBodyId;
}

void MouseInteraction::init(b2WorldId world, float p2m) {
  mWorld = world;
  mP2M = p2m;
}

bool MouseInteraction::handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view) {
  if (const auto* mPress = event.getIf<sf::Event::MouseButtonPressed>()) {
    if (mPress->button == sf::Mouse::Button::Left) {
      sf::Vector2f worldPos = window.mapPixelToCoords(mPress->position, view);
      b2Pos p = {worldPos.x / mP2M, worldPos.y / mP2M};

      // Create a small AABB around the cursor
      b2AABB aabb;
      aabb.lowerBound = {p.x - 0.1f, p.y - 0.1f};
      aabb.upperBound = {p.x + 0.1f, p.y + 0.1f};

      QueryContext qc;
      qc.point = p;
      qc.hit = false;
      qc.hitShape = b2_nullShapeId;

      b2QueryFilter filter = b2DefaultQueryFilter();
      b2World_OverlapAABB(mWorld, p, aabb, filter, queryCallback, &qc);

      if (qc.hit) {
        mDraggedBody = b2Shape_GetBody(qc.hitShape);
        
        // Don't drag static bodies
        if (b2Body_GetType(mDraggedBody) == b2_dynamicBody || b2Body_GetType(mDraggedBody) == b2_kinematicBody) {
          mIsDragging = true;
          b2Body_SetAwake(mDraggedBody, true);
          return true; // We grabbed something
        }
      }
    }
  } else if (const auto* mRelease = event.getIf<sf::Event::MouseButtonReleased>()) {
    if (mRelease->button == sf::Mouse::Button::Left) {
      mIsDragging = false;
      mDraggedBody = b2_nullBodyId;
    }
  }
  return false;
}

void MouseInteraction::update(float dt, const sf::RenderWindow& window, const sf::View& view) {
  if (mIsDragging && b2Body_IsValid(mDraggedBody)) {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, view);
    
    mCursorShape.setPosition(worldPos);

    b2Vec2 targetP = {worldPos.x / mP2M, worldPos.y / mP2M};
    b2Pos bodyP = b2Body_GetPosition(mDraggedBody);
    b2Vec2 vel = b2Body_GetLinearVelocity(mDraggedBody);
    float mass = b2Body_GetMass(mDraggedBody);
    
    // Spring-Damper (PD Controller) to pull body to mouse
    float frequencyHz = 5.0f;
    float dampingRatio = 0.7f;
    
    float omega = 2.0f * 3.14159265f * frequencyHz;
    float k = mass * omega * omega; // Stiffness
    float d = 2.0f * mass * dampingRatio * omega; // Damping
    
    b2Vec2 force;
    force.x = k * (targetP.x - bodyP.x) - d * vel.x;
    force.y = k * (targetP.y - bodyP.y) - d * vel.y;
    
    b2Body_ApplyForceToCenter(mDraggedBody, force, true);
    b2Body_SetAwake(mDraggedBody, true);
  }
}

void MouseInteraction::render(sf::RenderWindow& window) {
  if (mIsDragging) {
    window.draw(mCursorShape);
  }
}
