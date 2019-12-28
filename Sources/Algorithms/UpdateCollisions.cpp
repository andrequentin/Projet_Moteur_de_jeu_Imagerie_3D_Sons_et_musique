#include "Algorithms/UpdateCollisions.hpp"


namespace Gg {

  namespace Algorithm {

    UpdateCollisions::UpdateCollisions(Gg::GulgEngine &gulgEngine,Gg::Entity &w):
    	AbstractAlgorithm{gulgEngine},world{w} {

    	m_signature = gulgEngine.getComponentSignature("SceneObject");
      m_signature += gulgEngine.getComponentSignature("Collider");
      m_signature += gulgEngine.getComponentSignature("Forces");

    }

    UpdateCollisions::~UpdateCollisions() {}

    void UpdateCollisions::apply() {

      entity_world_collisions.clear();
      entity_entity_collisions.clear();
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
         std::shared_ptr<Gg::Component::Forces> eForces {
           std::static_pointer_cast<Gg::Component::Forces>(m_gulgEngine.getComponent(currentEntity, "Forces"))
         };
         //TO DO
         //"bounding box" du caps Collider
         // bbmin(min((c1.x-r),(c2.x-r)),min((c1.y-r),(c2.y-r)),min((c1.z-r),(c2.z-r)))
         // bbmax(max((c1.x+r),(c2.x+r)),max((c1.y+r),(c2.y+r)),max((c1.z+r),(c2.z+r)))
         glm::vec3 c1{ePosition + eCollider->c1  };
         glm::vec3 c2{ePosition + eCollider->c2  };
         c1 *= -1;
         c2 *= -1;
         c1[2] -= (eForces->forces[2] + eForces->velocity[2]);
         c2[2] -= (eForces->forces[2] + eForces->velocity[2]);

         glm::vec3 bbmin{
           std::min(c1[0] - eCollider->r ,c2[0] - eCollider->r),
           std::min(c1[1] - eCollider->r,c2[1] - eCollider->r),
           std::min(c1[2] - eCollider->r,c2[2] - eCollider->r)
         };
         glm::vec3 bbmax{
           std::max(c1[0] + eCollider->r,c2[0] + eCollider->r),
           std::max(c1[1] + eCollider->r,c2[1] + eCollider->r),
           std::max(c1[2] + eCollider->r,c2[2] + eCollider->r)
         };
         bbmin+=1.f;
         bbmax+=1.f;
         //TO DO
         //Tester pour les autres entités

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
         if(voxelToCheck.size()>0){
           entity_world_collisions.push_back(std::pair<Gg::Entity,std::vector<int>>(currentEntity,voxelToCheck));
         }
         // std::cout<<"colliding  "<<voxelToCheck.size()<< " voxels of the world"<<std::endl;
      }
    }
  }
}
