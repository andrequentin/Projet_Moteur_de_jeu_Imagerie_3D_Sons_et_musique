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
    CollisionsResolution::~CollisionsResolution() {
    }

    glm::vec3 getClosestPoint(glm::vec3 t, glm::vec3 bbmin,glm::vec3 bbmax){
      glm::vec3 result{0.f};
      for(unsigned int i {0};i<3;i++){
        if(t[i]>bbmax[i]){
          result[i]=bbmax[i];
        }else if(t[i]<bbmin[i]){
          result[i]=bbmin[i];
        }else{
          result[i]=t[i];
        }
      }
      return result;
    }
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
               std::shared_ptr<Gg::Component::Forces> newGForces{std::make_shared<Gg::Component::Forces>(glm::vec3{0.f},0.1f,1.f,2.f)};
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

           FMOD_RESULT fmodResult;
           FMOD::Studio::EventInstance *explosioneventInstance{nullptr};
           fmodResult = collisions->explosioneventDescription->createInstance(&explosioneventInstance);

            if (fmodResult != FMOD_OK) {

               std::cout << "Error " << fmodResult << " with FMOD studio API event creation: " << FMOD_ErrorString(fmodResult) << std::endl;
           }
           FMOD_3D_ATTRIBUTES att3D{
             FMOD_VECTOR{ eT[3][0],eT[3][1],eT[3][2]},
             FMOD_VECTOR{0.f,0.f,0.f },
             FMOD_VECTOR{ 0.f,-1.f,0.f},
             FMOD_VECTOR{0.f,0.f,-1.f}};
           explosioneventInstance->set3DAttributes(&att3D);
           explosioneventInstance->start();
           explosioneventInstance->release();

         }else{
           // std::cout<<voxelToCheck.size()<<std::endl;
           glm::vec3 c1{ePosition + eCollider->c1  };
           glm::vec3 c2{ePosition + eCollider->c2  };
           // c1 -= (eForces->forces + eForces->velocity);
           // c2 -= (eForces->forces + eForces->velocity);
           glm::vec3 bbmin{
             std::min(c1[0] - eCollider->r,c2[0] - eCollider->r),
             std::min(c1[1] - eCollider->r,c2[1] - eCollider->r),
             std::min(c1[2] - eCollider->r,c2[2] - eCollider->r)
           };
           glm::vec3 bbmax{
             std::max(c1[0] + eCollider->r,c2[0] + eCollider->r),
             std::max(c1[1] + eCollider->r,c2[1] + eCollider->r),
             std::max(c1[2] + eCollider->r,c2[2] + eCollider->r)
           };

           glm::vec3 collisional_response{0.f,0.f,0.f};
           glm::vec3 vcolor{0.f,0.f,0.f};
           for(unsigned int j{0};j<voxelToCheck.size();j++){
              glm::vec3 posV = vM->getVoxelPosition(voxelToCheck[j]);
              glm::vec3 brV = -1.f *  (posV+0.5f);
              glm::vec3 brE {getClosestPoint(brV,bbmin,bbmax)};
              // glm::vec3 brE{ePosition + eCollider->r};

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


              if( glm::dot(df,(eForces->velocity+eForces->forces))<=0.f
                  && v[0]>=0.f && v[1]>=0.f && v[2]>=0.f
                  && v[0] < vM->getWorldDimensions()[0] && v[1] < vM->getWorldDimensions()[1]&& v[2] < vM->getWorldDimensions()[2]
                  && vM->getColor(v[0],v[1],v[2])[3]<0.2f
                ){
                for(unsigned int k{0};k<3;k++){
                  if(df[k]!=0.f ){
                      collisional_response[k] = (eForces->velocity[k]+eForces->forces[k]);
                      k=3;
                    }
                  }
                }
                if(df[2]==-1.f){
                  vcolor = vM->getColor(voxelToCheck[j]);
                }
              }
              if(voxelToCheck.size()>0 && glm::length(collisional_response)==0.f){
                collisional_response[2] = (eForces->velocity[2]+eForces->forces[2]);
              }
              glm::vec soundTest {eForces->velocity+eForces->forces}; soundTest[2]=0.f;
              if(collisional_response[2] != 0
                && glm::length(soundTest) > 0.1f
                && m_gulgEngine.entityHasComponent(currentEntity.first,"StepSound")
              ){
                std::shared_ptr<Gg::Component::StepSound> sS {
                  std::static_pointer_cast<Gg::Component::StepSound>(m_gulgEngine.getComponent(currentEntity.first, "StepSound"))
                };
                FMOD_RESULT fmodResult;

                if(vcolor[0]<0.4f){
                  fmodResult = sS->stepeventInstance->setParameterByName("Matiere", 0);
                }else{
                  fmodResult = sS->stepeventInstance->setParameterByName("Matiere", 1);
                }
                if (fmodResult != FMOD_OK) {
                   std::cout << "Error " << fmodResult << " with FMOD studio API parameter: " << FMOD_ErrorString(fmodResult) << std::endl;
               }
                FMOD_3D_ATTRIBUTES att3D{
                  FMOD_VECTOR{ eT[3][0],eT[3][1],eT[3][2]},
                  FMOD_VECTOR{0.f,0.f,0.f },
                  FMOD_VECTOR{ 0.f,-1.f,0.f},
                  FMOD_VECTOR{0.f,0.f,-1.f}};
                sS->stepeventInstance->set3DAttributes(&att3D);
                FMOD_STUDIO_PLAYBACK_STATE s;
                sS->stepeventInstance->getPlaybackState(&s);
                if(s != FMOD_STUDIO_PLAYBACK_PLAYING )sS->stepeventInstance->start();

              }
              eForces->addForce(-collisional_response );
              eForces->velocity/=1.1f;

          }
      }
      //For each entity :
    	/*for(std::pair<Gg::Entity,Gg::Entity> collidingEntity: collisions->entity_entity_collisions) {
      }*/
      if(explode)   {
        localRemeshing(vxsToRs,*vM , *std::static_pointer_cast<Gg::Component::Mesh>(m_gulgEngine.getComponent(world, "MainMesh")));
        std::static_pointer_cast<Gg::Component::Mesh>(m_gulgEngine.getComponent(world, "MainMesh"))->reshape();
       }


    }

  }
}
