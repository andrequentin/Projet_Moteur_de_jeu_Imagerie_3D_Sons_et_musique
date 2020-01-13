#include "Algorithms/UpdateTimer.hpp"
#include <glm/gtx/string_cast.hpp>

#include <iostream>

namespace Gg {
  namespace Algorithm {

    UpdateTimer::UpdateTimer(Gg::GulgEngine &gulgEngine,Gg::Entity &w, Time* c):
      AbstractAlgorithm{gulgEngine},world{w},timeSystem(c) {

        m_signature = gulgEngine.getComponentSignature("Timer");

    }
    UpdateTimer::~UpdateTimer() {}


    void UpdateTimer::apply() {
      std::vector<std::vector<unsigned int>> vxsToRs;
      std::shared_ptr<VoxelMap> vM{
        std::static_pointer_cast<VoxelMap>(m_gulgEngine.getComponent(world, "VoxelMap"))
      };
      bool explode{false};
      // std::cout<< m_entitiesToApply.size()<<std::endl;
      for(unsigned int i =0; i < m_entitiesToApply.size();i++) {
        if(std::static_pointer_cast<Gg::Component::Timer>(m_gulgEngine.getComponent(m_entitiesToApply[i], "Timer"))->end <= std::chrono::system_clock::now()
        && m_gulgEngine.entityHasComponent(m_entitiesToApply[i],"Explosive")
        && m_gulgEngine.entityHasComponent(m_entitiesToApply[i],"SceneObject")){
          glm::mat4 eT{std::static_pointer_cast<Gg::Component::SceneObject>(m_gulgEngine.getComponent(m_entitiesToApply[i], "SceneObject"))->m_globalTransformations};
          glm::vec3 ePosition{
            eT[3][0],eT[3][1],eT[3][2]
          };
          float eP = std::static_pointer_cast<Gg::Component::Explosive>(m_gulgEngine.getComponent(m_entitiesToApply[i], "Explosive"))->explosivePower;
          vxsToRs.push_back(vM->explode(-1.f*ePosition[0],-1.f*ePosition[1],-1.f*ePosition[2],std::static_pointer_cast<Gg::Component::Explosive>(m_gulgEngine.getComponent(m_entitiesToApply[i], "Explosive"))->explosivePower));
          std::vector<unsigned int> vv = vxsToRs[vxsToRs.size()-1];
          timeSystem->toDelete.push_back(m_entitiesToApply[i]);
          explode=true;
          float x { -1.f*ePosition[0]}, y {-1.f*ePosition[1]}, z {-1.f*ePosition[2]};
          for(unsigned int j{0};j<vv.size();j++){
            glm::vec3 vP {vM->getVoxelPosition(vv[j])};
            if(vM->getColor(vv[j])[3] != 0.f && (eP*eP) >=  (x-vP[0])*(x-vP[0])+(y-vP[1])*(y-vP[1])+(z-vP[2])*(z-vP[2]) ){
              Gg::Entity newG{m_gulgEngine.getNewEntity()};
              std::shared_ptr<Gg::Component::SceneObject> newGScene{std::make_shared<Gg::Component::SceneObject>()};
              std::shared_ptr<Gg::Component::Transformation> newGTransformation{std::make_shared<Gg::Component::Transformation>()};
              std::shared_ptr<Gg::Component::Collider> newGCollider{std::make_shared<Gg::Component::Collider>()};
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
              timeSystem->toAdd.push_back(newG);

            }
          }
          FMOD_RESULT fmodResult;
          FMOD::Studio::EventInstance *explosioneventInstance{nullptr};
          fmodResult = timeSystem->explosioneventDescription->createInstance(&explosioneventInstance);

           if (fmodResult != FMOD_OK) {

              std::cout << "Error " << fmodResult << " with FMOD studio API event creation: " << FMOD_ErrorString(fmodResult) << std::endl;
          }
          FMOD_3D_ATTRIBUTES att3D{
            FMOD_VECTOR{ eT[3][0],eT[3][1],eT[3][2]},
            FMOD_VECTOR{0.f,0.f,0.f },
            FMOD_VECTOR{ 0.f,-1.f,0.f},
            FMOD_VECTOR{0.f,0.f,-1.f}};
          explosioneventInstance->set3DAttributes(&att3D);
          explosioneventInstance->setVolume(0.5f);
          explosioneventInstance->start();
          explosioneventInstance->release();

        }

      }
      if(explode)   {
        localRemeshing(vxsToRs,*vM , *std::static_pointer_cast<Gg::Component::Mesh>(m_gulgEngine.getComponent(world, "MainMesh")));
        std::static_pointer_cast<Gg::Component::Mesh>(m_gulgEngine.getComponent(world, "MainMesh"))->reshape();
       }


    }
  }
}
