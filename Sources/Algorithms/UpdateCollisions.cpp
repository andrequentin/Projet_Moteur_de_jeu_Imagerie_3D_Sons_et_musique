#include "Algorithms/UpdateCollisions.hpp"
#include <glm/gtx/string_cast.hpp>

namespace Gg {

  namespace Algorithm {

    UpdateCollisions::UpdateCollisions(Gg::GulgEngine &gulgEngine,Gg::Entity &w, Collisions* c):
    	AbstractAlgorithm{gulgEngine},world{w},collisions{c} {

    	m_signature = gulgEngine.getComponentSignature("SceneObject");
      m_signature += gulgEngine.getComponentSignature("Transformations");
      m_signature += gulgEngine.getComponentSignature("Collider");
      m_signature += gulgEngine.getComponentSignature("Forces");

    }

    UpdateCollisions::~UpdateCollisions() {}

    void UpdateCollisions::apply() {

      collisions->entity_world_collisions.clear();
      collisions->entity_entity_collisions.clear();
      //Get world Collider
      glm::mat4 wT{std::static_pointer_cast<Gg::Component::SceneObject>(m_gulgEngine.getComponent(world, "SceneObject"))->m_globalTransformations};
      std::shared_ptr<VoxelMap> vM{
        std::static_pointer_cast<VoxelMap>(m_gulgEngine.getComponent(world, "VoxelMap"))
      };
      //For each entity :
    	for(unsigned int i =0; i < m_entitiesToApply.size();i++) {
        Gg::Entity currentEntity {m_entitiesToApply[i]};
        glm::mat4 eT{std::static_pointer_cast<Gg::Component::SceneObject>(m_gulgEngine.getComponent(currentEntity, "SceneObject"))->m_globalTransformations};
        glm::vec3 ePosition{
          eT[3][0],eT[3][1],eT[3][2]
        };
        std::shared_ptr<Gg::Component::Collider> eCollider {
          std::static_pointer_cast<Gg::Component::Collider>(m_gulgEngine.getComponent(currentEntity, "Collider"))
        };
        ePosition -= 0.5f;

        std::shared_ptr<Gg::Component::Forces> eForces {
          std::static_pointer_cast<Gg::Component::Forces>(m_gulgEngine.getComponent(currentEntity, "Forces"))
        };
        //TO DO

        glm::vec3 bbmin{ePosition + eCollider->bbmax  };
        glm::vec3 bbmax{ ePosition + eCollider->bbmin };
        bbmin *= -1;
        bbmax *= -1;
        bbmin -= (eForces->forces + eForces->velocity);
        bbmax -= (eForces->forces + eForces->velocity);


        //tester avec le world
        //récupérer voxel voisins (bbmin -> bbmax)
        std::array<unsigned int, 3> wD = vM->getWorldDimensions();
        std::vector<int> voxelToCheck;
        for(float i{std::floor(std::max(bbmin[0],0.f))};i < std::ceil(std::min(bbmax[0],static_cast<float>(wD.at(0)-1)));i+=1.f){
          for(float j{std::floor(std::max(bbmin[1],0.f))};j < std::ceil(std::min(bbmax[1],static_cast<float>(wD.at(1)-1)));j+=1.f){
            for(float k{std::floor(std::max(bbmin[2],0.f))};k < std::ceil(std::min(bbmax[2],static_cast<float>(wD.at(2)-1)));k+=1.f){
              if(std::find(voxelToCheck.begin(),voxelToCheck.end(), vM->getVoxelID(i,j,k)) == voxelToCheck.end()
              && vM->getColor(i,j,k)[3] > 0.f
            ){
                voxelToCheck.push_back(vM->getVoxelID(i,j,k));
              }
            }
          }
        }
        // std::cout<<"collidin with" <<voxelToCheck.size() <<" voxels"<<std::endl;
        if(voxelToCheck.size()>0){
          collisions->entity_world_collisions.push_back(std::pair<Gg::Entity,std::vector<int>>(currentEntity,voxelToCheck));
        }
         // std::cout<<"colliding  "<<voxelToCheck.size()<< " voxels of the world"<<std::endl;

        if(i!=m_entitiesToApply.size()-1){
          for(unsigned int j = i+1;j<m_entitiesToApply.size();j++){
            Gg::Entity currentEntity2{m_entitiesToApply[j]};
            glm::mat4 eT2{std::static_pointer_cast<Gg::Component::SceneObject>(m_gulgEngine.getComponent(currentEntity2, "SceneObject"))->m_globalTransformations};
            glm::vec3 ePosition2{
              eT2[3][0],eT2[3][1],eT2[3][2]
            };
            std::shared_ptr<Gg::Component::Collider> eCollider2 {
              std::static_pointer_cast<Gg::Component::Collider>(m_gulgEngine.getComponent(currentEntity2, "Collider"))
            };
            ePosition2 -= 0.5f;
            glm::vec3 bbmin2{ePosition2 + eCollider2->bbmax };
            glm::vec3 bbmax2{ePosition2 + eCollider2->bbmin   };
            bbmin2 *= -1;
            bbmax2 *= -1;



            if( (bbmin.x <= bbmax2.x && bbmax.x >= bbmin2.x) &&
                (bbmin.y <= bbmax2.y && bbmax.y >= bbmin2.y) &&
                (bbmin.z <= bbmax2.z && bbmax.z >= bbmin2.z) ){
                  collisions->entity_entity_collisions.push_back(std::pair<Gg::Entity,Gg::Entity>(currentEntity,currentEntity2));
            }

          }
        }
      }
    }
  }
}
