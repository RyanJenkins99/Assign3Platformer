#include "cmp_player_physics.h"
#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include "../components/PlayerAudioComponent.h"

using namespace std;
using namespace sf;
using namespace Physics;

bool PlayerPhysicsComponent::isGrounded() const {
    //retrieves a list of all contacts that the player's body is currently in contact with (touch).
  auto touch = getTouching();
  //gets the current position of the player's body.
  const auto& pos = _body->GetPosition();
  //It calculates some values for the height and width of the player's body,
  // based on the size passed to the constructor of the component.
  const float halfPlrHeigt = _size.y * .5f;
  const float halfPlrWidth = _size.x * .52f;
  //creates a b2WorldManifold object called manifold which will be used to store
  // information about the contact points between the player and other objects.
  b2WorldManifold manifold;
  //shows where the player makes contact with the world
  for (const auto& contact : touch) {
    contact->GetWorldManifold(&manifold);
    const int numPoints = contact->GetManifold()->pointCount;
    bool onTop = numPoints > 0;
    // If all contacts are below the player.
    for (int j = 0; j < numPoints; j++) {
      onTop &= (manifold.points[j].y < pos.y - halfPlrHeigt);
    }
    //After the loop finishes, it checks whether onTop is still true.
    // If it is, it returns true to indicate that the player is grounded.
    if (onTop) {
      return true;
    }
  }

  return false;
}

void PlayerPhysicsComponent::update(double dt) {

  const auto pos = _parent->getPosition();

  //Teleport to start if we fall off map.
  if (pos.y > ls::getHeight() * ls::getTileSize()) {
    teleport(ls::getTilePosition(ls::findTiles(ls::START)[0]));
  }

  if (Keyboard::isKeyPressed(Keyboard::Left) ||
      Keyboard::isKeyPressed(Keyboard::Right)) {
    // Moving Either Left or Right
    if (Keyboard::isKeyPressed(Keyboard::Right)) {
      if (getVelocity().x < _maxVelocity.x)
        impulse({(float)(dt * _groundspeed), 0});
    } else {
      if (getVelocity().x > -_maxVelocity.x)
        impulse({-(float)(dt * _groundspeed), 0});
    }
  } else {
    // Dampen X axis movement
    dampen({0.9f, 1.0f});
  }

  // Handle Jump
  if (Keyboard::isKeyPressed(Keyboard::Up)) {
    _grounded = isGrounded();

    if (_grounded) {
      setVelocity(Vector2f(getVelocity().x, 1.f));
      teleport(Vector2f(pos.x, pos.y - 2.0f));
      impulse(Vector2f(0, -6.f));
        PlayerAudioComponent::play("res/music/jump1.wav");

    }
  }

  //Are we in air?
  if (!_grounded) {
    // Check to see if we have landed yet
    _grounded = isGrounded();
    // disable friction while jumping
    setFriction(0.f);
  } else {
    setFriction(0.1f);
  }

  // Clamp velocity.
  auto v = getVelocity();
  v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
  v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);
  setVelocity(v);

  PhysicsComponent::update(dt);
}
//It takes a pointer to an Entity and a Vector2f as arguments,
// and passes the Entity pointer and a true value as the second argument to the base PhysicsComponent constructor.
// It also passes the Vector2f object as the third argument.
PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p,
                                               const Vector2f& size)
    : PhysicsComponent(p, true, size) {
  _size = sv2_to_bv2(size, true);
  //It initializes the _maxVelocity member variable to (200.f, 400.f).
  _maxVelocity = Vector2f(200.f, 400.f);
  //It initializes the _groundspeed member variable to 30.
  _groundspeed = 30.f;
  //It sets the _grounded member variable to false.
  _grounded = false;
  //It disables sleeping for the physics body associated with the component.
  _body->SetSleepingAllowed(false);
  //It sets the physics body to have fixed rotation.
  _body->SetFixedRotation(true);
  //Bullet items have higher-res collision detection
  _body->SetBullet(true);
}
