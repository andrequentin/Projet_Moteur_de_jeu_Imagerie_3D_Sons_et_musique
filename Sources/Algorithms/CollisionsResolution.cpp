#include "Algorithms/CollisionsResolution.hpp"



namespace Gg {
  namespace Algorithm {

    CollisionsResolution::CollisionsResolution(Gg::GulgEngine &gulgEngine,Gg::Entity &w,Gg::Algorithm::UpdateCollisions* c):
      AbstractAlgorithm{gulgEngine},world{w},collisions(c) {

      m_signature = gulgEngine.getComponentSignature("SceneObject");
      m_signature += gulgEngine.getComponentSignature("Collider");
      m_signature += gulgEngine.getComponentSignature("Forces");

    }
    CollisionsResolution::~CollisionsResolution() {}

    void CollisionsResolution::apply() {
      glm::mat4 wT{std::static_pointer_cast<Gg::Component::SceneObject>(m_gulgEngine.getComponent(world, "SceneObject"))->m_globalTransformations};
      std::shared_ptr<VoxelMap> vM{
        std::static_pointer_cast<VoxelMap>(m_gulgEngine.getComponent(world, "VoxelMap"))
      };
      //For each entity :
    	for(std::pair<Gg::Entity,std::vector<int>> currentEntity: collisions->entity_world_collisions) {
        glm::mat4 eT{std::static_pointer_cast<Gg::Component::SceneObject>(m_gulgEngine.getComponent(currentEntity.first, "SceneObject"))->m_globalTransformations};
        glm::vec3 ePosition{
          eT[3][0],eT[3][1],eT[3][2]
        };
         std::shared_ptr<Gg::Component::Collider> eCollider {
           std::static_pointer_cast<Gg::Component::Collider>(m_gulgEngine.getComponent(currentEntity.first, "Collider"))
         };
         std::shared_ptr<Gg::Component::Forces> eForces {
           std::static_pointer_cast<Gg::Component::Forces>(m_gulgEngine.getComponent(currentEntity.first, "Forces"))
         };
         std::vector<int> voxelToCheck {currentEntity.second};


         eForces->velocity/=1.1f;
         glm::vec3 brE {ePosition + 0.5f};
         glm::vec3 collisional_response{0.f,0.f,0.f};
         for(unsigned int i{0};i<voxelToCheck.size();i++){
            glm::vec3 brV = -1.f *  (vM->getVoxelPosition(voxelToCheck[i])-0.5f);
            glm::vec3 df {brE-brV};

            if(glm::dot(df,(eForces->velocity+eForces->forces))<0.f){
              if(std::abs(df[0])>std::abs(df[1]) && std::abs(df[0]) > std::abs(df[2])){
                if(std::abs(collisional_response[0]) == 0.f){
                  collisional_response[0] +=(eForces->velocity[0]+eForces->forces[0]);
                }
              }else if(std::abs(df[1])>std::abs(df[0]) && std::abs(df[1]) > std::abs(df[2])){
                if(std::abs(collisional_response[1]) == 0.f){
                  collisional_response[1] +=(eForces->velocity[1]+eForces->forces[1]);

                }
              } else if(std::abs(df[2])>std::abs(df[1]) && std::abs(df[2]) > std::abs(df[0])){
                if(std::abs(collisional_response[2]) == 0.f){
                  collisional_response[2] +=(eForces->velocity[2]+eForces->forces[2]);

                }
              }
            }
          }

          // std::cout<<to_string(-collisional_response)<<std::endl;
            eForces->addForce(-collisional_response );
      }
      //For each entity :
    	for(std::pair<Gg::Entity,Gg::Entity> collidingEntity: collisions->entity_entity_collisions) {

      }
    }

  }
}
