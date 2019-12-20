#include "Algorithms/UpdateCollisions.hpp"

namespace Gg {

  namespace Algorithm {

    UpdateCollisions::UpdateCollisions(Gg::GulgEngine &gulgEngine,Gg::Entity &w):
    	AbstractAlgorithm{gulgEngine},world{w} {

    	m_signature = gulgEngine.getComponentSignature("SceneObject");
      m_signature += gulgEngine.getComponentSignature("Transformations");
      m_signature += gulgEngine.getComponentSignature("Collider");
      m_signature += gulgEngine.getComponentSignature("WorldCollider");
    }

    UpdateCollisions::~UpdateCollisions() {}

    void UpdateCollisions::apply() {

      //Get world Collider

      //For each entity :
    //	for(Gg::Entity currentEntity: m_entitiesToApply) {
          //check with world collider
          //check with other entity
      // }
    }
  }
}
