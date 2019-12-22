#include "Algorithms/UpdateCollisions.hpp"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

namespace Gg {

  namespace Algorithm {

    UpdateCollisions::UpdateCollisions(Gg::GulgEngine &gulgEngine,Gg::Entity &w):
    	AbstractAlgorithm{gulgEngine},world{w} {

    	m_signature = gulgEngine.getComponentSignature("SceneObject");
      m_signature += gulgEngine.getComponentSignature("Collider");
      std::cout<< "ouii"<<std::endl;

    }

    UpdateCollisions::~UpdateCollisions() {}

    void UpdateCollisions::apply() {
      //Get world Collider
      glm::mat4 wT{std::static_pointer_cast<Gg::Component::SceneObject>(m_gulgEngine.getComponent(world, "SceneObject"))->m_globalTransformations};
      std::shared_ptr<VoxelMap> vM{
        std::static_pointer_cast<VoxelMap>(m_gulgEngine.getComponent(world, "VoxelMap"))
      };
      //For each entity :
    	for(Gg::Entity currentEntity: m_entitiesToApply) {
        glm::mat4 eT{std::static_pointer_cast<Gg::Component::SceneObject>(m_gulgEngine.getComponent(currentEntity, "SceneObject"))->m_globalTransformations};

        glm::vec3 ePosition{
          eT[3][0],eT[3][1],eT[3][2]
        };
        ePosition *= -1.f;

        if(ePosition[0]>=0.f && ePosition[1]>=0.f && ePosition[2]>=0.f && ePosition[0]<= vM->getWorldDimensions()[0]&& ePosition[1]<= vM->getWorldDimensions()[1]&& ePosition[2]<= vM->getWorldDimensions()[2])
        {
          if(vM->getColor( std::floor(ePosition[0]),std::floor(ePosition[1]),std::floor(ePosition[2]))[3] > 0.0f){

          std::cout<<"Collision between world & entity at position : "<< to_string(ePosition)<<std::endl;
        }
        }  //check with world collider
          //check with other entity
      }
    }
  }
}
