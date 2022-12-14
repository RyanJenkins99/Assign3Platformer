#include "cmp_hurt_player.h"
#include <LevelSystem.h>
#include <engine.h>

using namespace std;
using namespace sf;

void HurtComponent::update(double dt) {//explain what's going on here
  if (auto pl = _player.lock()) {
    if (length(pl->getPosition() - _parent->getPosition()) < 25.0) {
      pl->setForDelete();
      _parent->setForDelete();
    }
  }
}

HurtComponent::HurtComponent(Entity* p)
    : Component(p), _player(_parent->scene->ents.find("player")[0]) {}
