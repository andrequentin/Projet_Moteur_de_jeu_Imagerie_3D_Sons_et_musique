#ifndef WORLD_COLLIDER_HPP
#define WORLD_COLLIDER_HPP

#include <glm/vec3.hpp>

#include "Components/Component.hpp"
#include "Components/VoxelMap.hpp"

class WorldCollider: public Gg::Component::AbstractComponent{
public:
  WorldCollider(  VoxelMap &m);
  virtual std::shared_ptr<AbstractComponent> clone() const{
    return std::static_pointer_cast<Gg::Component::AbstractComponent>(std::make_shared<WorldCollider>(*this));
  }

  VoxelMap &map;
};
#endif
