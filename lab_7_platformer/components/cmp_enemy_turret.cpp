#include "cmp_enemy_turret.h"
#include "cmp_bullet.h"
#include "cmp_hurt_player.h"
#include "engine.h"
#include <SFML/Graphics/CircleShape.hpp>
using namespace std;
using namespace sf;

void EnemyTurretComponent::update(double dt) {
    //reduces a timer called _firetime by the amount of time that has passed since the last frame (dt).
  _firetime -= dt;
  //If _firetime is less than or equal to zero, it calls the fire method and resets _firetime to 1.
  if (_firetime <= 0.f) {
    fire();
    _firetime = 1.f;
  }
  static float angle = 0.f;
  //It declares a static variable called angle and increments it by 1 times dt (which is the elapsed time since the last frame).
  angle += 1.f * dt;
    //sets the rotation of the turret's parent entity to 180 degrees plus the sine of angle times 45 degrees.
    // This will cause the turret to oscillate between 135 degrees and 225 degrees over time.
  _parent->setRotation(180.f + sin(angle) * 45.f);
}

void EnemyTurretComponent::fire() const {
    //The fire method creates a new entity and stores it in a local variable called bullet.
  auto bullet = _parent->scene->makeEntity();
  //sets the position of the bullet to the position of the turret's parent entity.
  bullet->setPosition(_parent->getPosition());
  //It adds a HurtComponent, a BulletComponent, and a ShapeComponent to the bullet entity.
  bullet->addComponent<HurtComponent>();
  bullet->addComponent<BulletComponent>();
  //It sets the shape of the ShapeComponent to be a circle with radius 8,
  // and sets its color to red and its origin to the center of the circle.
  auto s = bullet->addComponent<ShapeComponent>();

  s->setShape<sf::CircleShape>(8.f);
  s->getShape().setFillColor(Color::Red);
  s->getShape().setOrigin(Vector2f(8.f, 8.f));
  //It adds a PhysicsComponent to the bullet entity, passing in true as the first
  // argument to indicate that the bullet should be a dynamic body (i.e. one that is affected by forces and collisions).
  // It also passes in a size of (8.f, 8.f) for the physics body.
  auto p = bullet->addComponent<PhysicsComponent>(true, Vector2f(8.f, 8.f));
  p->setRestitution(.4f);
  //sets the restitution (bounciness) of the bullet's physics body to 0.4 and its friction to 0.005.
  p->setFriction(.005f);
  //It calls the impulse method on the PhysicsComponent,
  // passing in a vector rotated by the negative of the turret's rotation.
  // This will cause the bullet to be launched in a direction that is perpendicular to the turret's barrel.
  p->impulse(sf::rotate(Vector2f(0, 15.f), -_parent->getRotation()));
}

EnemyTurretComponent::EnemyTurretComponent(Entity* p)
    : Component(p), _firetime(2.f) {}
