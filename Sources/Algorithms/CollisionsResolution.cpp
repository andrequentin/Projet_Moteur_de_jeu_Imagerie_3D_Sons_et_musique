#include "Algorithms/CollisionsResolution.hpp"
#include <glm/gtx/string_cast.hpp>

#include <iostream>

namespace Gg {
  namespace Algorithm {

    CollisionsResolution::CollisionsResolution(Gg::GulgEngine &gulgEngine,Gg::Entity &w, Collisions* c):
      AbstractAlgorithm{gulgEngine},world{w},collisions(c) {

      m_signature = gulgEngine.getComponentSignature("SceneObject");
      m_signature += gulgEngine.getComponentSignature("Collider");
      m_signature += gulgEngine.getComponentSignature("Forces");

    }
    CollisionsResolution::~CollisionsResolution() {}


    void CollisionsResolution::apply() {
      bool explode{false};
      glm::mat4 wT{std::static_pointer_cast<Gg::Component::SceneObject>(m_gulgEngine.getComponent(world, "SceneObject"))->m_globalTransformations};
      std::shared_ptr<VoxelMap> vM{
        std::static_pointer_cast<VoxelMap>(m_gulgEngine.getComponent(world, "VoxelMap"))
      };
       std::vector<std::vector<unsigned int>> vxsToRs;
      //For each entity :
    	for(unsigned int i{0};i<collisions->entity_world_collisions.size();i++) {
        std::pair<Gg::Entity,std::vector<int>> currentEntity = collisions->entity_world_collisions[i];
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

         ePosition -= 0.5f;
         if(voxelToCheck.size()>0 && m_gulgEngine.entityHasComponent(currentEntity.first,"Explosive")
         && std::static_pointer_cast<Gg::Component::Explosive>(m_gulgEngine.getComponent(currentEntity.first, "Explosive"))->eTrigger == ON_COLLISION ){
            vxsToRs.push_back(vM->explode(-1.f*ePosition[0],-1.f*ePosition[1],-1.f*ePosition[2],std::static_pointer_cast<Gg::Component::Explosive>(m_gulgEngine.getComponent(currentEntity.first, "Explosive"))->explosivePower));
            std::vector<unsigned int> vv = vxsToRs[vxsToRs.size()-1];
           collisions->toDelete.push_back(currentEntity.first);
           explode=true;
           float x { -1.f*ePosition[0]}, y {-1.f*ePosition[1]}, z {-1.f*ePosition[2]};
           float eP = std::static_pointer_cast<Gg::Component::Explosive>(m_gulgEngine.getComponent(currentEntity.first, "Explosive"))->explosivePower;
           for(unsigned int j{0};j<vv.size();j++){
             glm::vec3 vP {vM->getVoxelPosition(vv[j])};
             if(vM->getColor(vv[j])[3] != 0.f && (eP*eP) >=  (x-vP[0])*(x-vP[0])+(y-vP[1])*(y-vP[1])+(z-vP[2])*(z-vP[2]) ){
               Gg::Entity newG{m_gulgEngine.getNewEntity()};
               std::shared_ptr<Gg::Component::SceneObject> newGScene{std::make_shared<Gg::Component::SceneObject>()};
               std::shared_ptr<Gg::Component::Transformation> newGTransformation{std::make_shared<Gg::Component::Transformation>()};
               std::shared_ptr<Gg::Component::Collider> newGCollider{std::make_shared<Gg::Component::Collider>
                 (glm::vec3{0.f,0.f,0.f},glm::vec3{0.f,0.f,0.f},0.5f)};
               std::shared_ptr<Gg::Component::Forces> newGForces{std::make_shared<Gg::Component::Forces>()};
               std::shared_ptr<Gg::Component::Mesh> newGMesh{std::make_shared<Gg::Component::Mesh>(m_gulgEngine.getProgram("MainProgram"))};
               Cube(newGMesh,0.5f,vM->getColor(vv[j]));
               newGTransformation->translate(- vM->getVoxelPosition(vv[j]));
               glm::vec3 f{(-1.f * ePosition) - vM->getVoxelPosition(vv[j])  };
               f=glm::normalize(f);f[2]-=1.f;
               newGForces->addForce(f * eP);
               m_gulgEngine.addComponentToEntity(newG, "SceneObject", std::static_pointer_cast<Gg::Component::AbstractComponent>(newGScene));
               m_gulgEngine.addComponentToEntity(newG, "Transformations", std::static_pointer_cast<Gg::Component::AbstractComponent>(newGTransformation));
               m_gulgEngine.addComponentToEntity(newG, "Collider", std::static_pointer_cast<Gg::Component::AbstractComponent>(newGCollider));
               m_gulgEngine.addComponentToEntity(newG, "Forces", std::static_pointer_cast<Gg::Component::AbstractComponent>(newGForces));
               m_gulgEngine.addComponentToEntity(newG, "MainMesh", std::static_pointer_cast<Gg::Component::AbstractComponent>(newGMesh));
               collisions->toAdd.push_back(newG);

             }
           }

         }else{

           glm::vec3 brE {ePosition + eCollider->r};
           // brE-=eForces->forces;
           glm::vec3 collisional_response{0.f,0.f,0.f};

           for(unsigned int i{0};i<voxelToCheck.size();i++){
              glm::vec3 posV = vM->getVoxelPosition(voxelToCheck[i]);
              glm::vec3 brV = -1.f *  (posV-0.5f);
              glm::vec3 df {brE-brV};
              if(std::abs(df[0])> std::abs(df[1]) && std::abs(df[0])> std::abs(df[2]) ){
                df[2]=0.f;
                df[1]=0.f;
              }else if(std::abs(df[1])> std::abs(df[0]) && std::abs(df[1])> std::abs(df[2])){
                df[2]=0.f;
                df[0]=0.f;
              } else{
                df[0]=0.f;
                df[1]=0.f;
              }
              df = glm::normalize(df);
              glm::vec3 v{posV -df};
              // std::cout<<to_string(df)
              // <<to_string(eForces->velocity+eForces->forces)
              // <<std::endl;

              if( glm::dot(df,(eForces->velocity+eForces->forces))<0.f
                  && v[0]>=0.f && v[1]>=0.f && v[2]>=0.f
                  && v[0] < vM->getWorldDimensions()[0] && v[1] < vM->getWorldDimensions()[1]&& v[2] < vM->getWorldDimensions()[2]
                  && vM->getColor(v[0],v[1],v[2])[3]<0.2f){
                for(unsigned int i{0};i<3;i++){
                  if(std::abs(df[i])>0.f  && std::abs(collisional_response[i]) == 0.f  ){
                      collisional_response[i] +=(eForces->velocity[i]+eForces->forces[i]);
                      i=3;
                    }
                  }
                }
              }

            eForces->velocity/=1.1f;

            eForces->addForce(-collisional_response );
          }
      }
      //For each entity :
    	for(std::pair<Gg::Entity,Gg::Entity> collidingEntity: collisions->entity_entity_collisions) {
      }
      if(explode)   {
        localRemeshing(vxsToRs,*vM , *std::static_pointer_cast<Gg::Component::Mesh>(m_gulgEngine.getComponent(world, "MainMesh")));
        std::static_pointer_cast<Gg::Component::Mesh>(m_gulgEngine.getComponent(world, "MainMesh"))->reshape();
       }


    }

  }
}
