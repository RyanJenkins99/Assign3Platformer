#include "cmp_physics.h"
#include "system_physics.h"

using namespace std;
using namespace sf;

using namespace Physics;

void PhysicsComponent::update(double dt) {
  _parent->setPosition(invert_height(bv2_to_sv2(_body->GetPosition())));
  _parent->setRotation((180 / b2_pi) * _body->GetAngle());
}
// This function updates the position and rotation of the parent entity based on the position and angle
// of the Box2D body associated with this component. The position is converted between screen coordinates and Box2D coordinates
// using the invert_height and sv2_to_bv2 functions.

PhysicsComponent::PhysicsComponent(Entity* p, bool dyn,
                                   const Vector2f& size)
    : Component(p), _dynamic(dyn) {

  b2BodyDef BodyDef;
  // Is Dynamic(moving), or static(Stationary)
  BodyDef.type = _dynamic ? b2_dynamicBody : b2_staticBody;
  BodyDef.position = sv2_to_bv2(invert_height(p->getPosition()));

  // Create the body
  _body = Physics::GetWorld()->CreateBody(&BodyDef);
  _body->SetActive(true);
  {
    // Create the fixture shape
    b2PolygonShape Shape;
    // SetAsBox box takes HALF-Widths!
    Shape.SetAsBox(sv2_to_bv2(size).x * 0.5f, sv2_to_bv2(size).y * 0.5f);
    b2FixtureDef FixtureDef;
    // Fixture properties
    // FixtureDef.density = _dynamic ? 10.f : 0.f;
    FixtureDef.friction = _dynamic ? 0.1f : 0.8f;
    FixtureDef.restitution = .2;
    FixtureDef.shape = &Shape;
    // Add to body
    _fixture = _body->CreateFixture(&FixtureDef);
    //_fixture->SetRestitution(.9)
    FixtureDef.restitution = .2;
  }

    // This is the constructor for the PhysicsComponent class.
    // It takes as arguments a pointer to the parent entity, a boolean indicating whether the body is dynamic (moving)
    // or static (stationary), and the size of the body. It creates a Box2D body
    // and fixture (a shape attached to the body) based on these arguments, and associates them with the PhysicsComponent object.


}

void PhysicsComponent::setFriction(float r) { _fixture->SetFriction(r); }
//This function sets the friction of the fixture attached to the body.

void PhysicsComponent::setMass(float m) { _fixture->SetDensity(m); }
//This function sets the density (mass per unit area) of the fixture attached to the body.

void PhysicsComponent::teleport(const sf::Vector2f& v) {
  _body->SetTransform(sv2_to_bv2(invert_height(v)), 0.0f);
}
//This function teleports the body to a new position v in screen coordinates,
// converting it to Box2D coordinates using the sv2_to_bv2 function.

const sf::Vector2f PhysicsComponent::getVelocity() const {
  return bv2_to_sv2(_body->GetLinearVelocity(), true);
}
//This function returns the linear velocity of the body in screen coordinates,
// converting it from Box2D coordinates using the bv2_to_sv2 function.

void PhysicsComponent::setVelocity(const sf::Vector2f& v) {
  _body->SetLinearVelocity(sv2_to_bv2(v, true));
}
//This function sets the linear velocity of the body to a new value v in screen coordinates,
// converting it to Box2D coordinates using the sv2_to_bv2 function.

b2Fixture* const PhysicsComponent::getFixture() const { return _fixture; }
//This function returns a pointer to the fixture attached to the body.

PhysicsComponent::~PhysicsComponent() {
  auto a = Physics::GetWorld();
  _body->SetActive(false);
  Physics::GetWorld()->DestroyBody(_body);
  // delete _body;
  _body = nullptr;
}

//The PhysicsComponent class also has a destructor, which destroys the Box2D body and
// fixture when the PhysicsComponent object is deleted.

void PhysicsComponent::render() {}

//function appears to be a placeholder for rendering
// the physics body associated with this component.

void PhysicsComponent::impulse(const sf::Vector2f& i) {
  auto a = b2Vec2(i.x, i.y * -1.0f);
  _body->ApplyLinearImpulseToCenter(a, true);
}
// function applies a linear impulse (a sudden change in velocity) to the center of the body
// associated with this component. The impulse is specified as a 2D vector i in screen coordinates,
// and is negated in the y-axis before being applied to the body. This is likely because
// Box2D uses a coordinate system with the y-axis pointing downwards, while many
// graphics libraries use a coordinate system with the y-axis pointing upwards.

void PhysicsComponent::dampen(const sf::Vector2f& i) {
  auto vel = _body->GetLinearVelocity();
  vel.x *= i.x;
  vel.y *= i.y;
  _body->SetLinearVelocity(vel);
}
// This function reduces the linear velocity of the body associated with this component by a factor of
// i. The i vector is interpreted as a damping factor for each axis, with values less than
// 1 reducing the velocity and values greater than 1 increasing it.


bool PhysicsComponent::isTouching(const PhysicsComponent& pc) const {
  b2Contact* bc;
  return isTouching(pc, bc);
}

//function returns true if the body associated with this component is in contact with the body
// associated with the pc PhysicsComponent. It does this by iterating over the contact points
// in the Box2D world and checking if either fixture is associated with this PhysicsComponent or the pc PhysicsComponent.

bool PhysicsComponent::isTouching(const PhysicsComponent& pc,
                                  b2Contact const* bc) const {
  const auto _otherFixture = pc.getFixture();
  const auto& w = *Physics::GetWorld();
  const auto contactList = w.GetContactList();
  const auto clc = w.GetContactCount();
  for (int32 i = 0; i < clc; i++) {
    const auto& contact = (contactList[i]);
    if (contact.IsTouching() && ((contact.GetFixtureA() == _fixture &&
                                  contact.GetFixtureA() == _otherFixture) ||
                                 (contact.GetFixtureA() == _otherFixture &&
                                  contact.GetFixtureA() == _fixture))) {
      bc = &contact;
      return true;
    }
  }
  return false;
}

//function is similar to the previous function, but it also returns a pointer to the contact point in the bc argument.

std::vector<b2Contact const*> PhysicsComponent::getTouching() const {
  std::vector<b2Contact const*> ret;

  b2ContactEdge* edge = _body->GetContactList();
  while (edge != NULL) {
    const b2Contact* contact = edge->contact;
    if (contact->IsTouching()) {
      ret.push_back(contact);
    }
    edge = edge->next;
  }

  return ret;
}

// This function returns a list of pointers to the contact points where the body associated with this component
// is in contact with other bodies. It does this by iterating over the contact edges (pairs of bodies in contact)
// attached to the body and adding the contact points to a list if they are touching.

void PhysicsComponent::setRestitution(float r) {
  _fixture->SetRestitution(r);
}
//This function sets the restitution (bounciness) of the fixture attached to the body associated with this component.
// A value of 0 means the fixture does not bounce at all, while a value of 1 means it bounces back with the same velocity it had before the collision.

//-----------------------------------------------------------------------

// Enemy class definition
//class Enemy {
//public:
//    Enemy(const sf::Vector2f& pos);
//    void update(double dt);
//
//private:
//    PhysicsComponent* physics;
//};
//
//// Enemy class implementation
//Enemy::Enemy(const sf::Vector2f& pos) {
//    // Initialize the physics component
//    physics = new PhysicsComponent(this, true, Vector2f(1.0f, 1.0f));
//    physics->teleport(pos);
//}
//
//void Enemy::update(double dt) {
//    // Check for collisions with the player
//    if (playerPhysics->isTouching(*physics)) {
//        // Handle the player's death in the game logic
//        handlePlayerDeath();
//    }
//}
//
//// ...
//
//int main() {
//    // ...
//
//    // Create an enemy object
//    Enemy enemy(Vector2f(0.0f, 0.0f));
//
//    // ...
//
//    // Game update loop
//    while (window.isOpen()) {
//        // ...
//
//        // Update the enemy
//        enemy.update(dt);
//
//        // ...
//    }
//}