#include "GulgEngine/GulgEngine.hpp"

#include "Components/Mesh.hpp"
#include "Components/Transformation.hpp"
#include "Components/SceneObject.hpp"

void newMap(Gg::GulgEngine &  engine, Gg::Entity & worldID, GLuint program){

  std::shared_ptr<Gg::Component::SceneObject> worldScene{std::make_shared<Gg::Component::SceneObject>()};
  std::shared_ptr<Gg::Component::Transformation> worldTransformation{std::make_shared<Gg::Component::Transformation>()};
  std::shared_ptr<Gg::Component::Mesh> worldMesh{std::make_shared<Gg::Component::Mesh>(program)};

  engine.addComponentToEntity(worldID, "SceneObject", std::static_pointer_cast<Gg::Component::AbstractComponent>(worldScene));
  engine.addComponentToEntity(worldID, "Transformations", std::static_pointer_cast<Gg::Component::AbstractComponent>(worldTransformation));
  engine.addComponentToEntity(worldID, "MainMesh", std::static_pointer_cast<Gg::Component::AbstractComponent>(worldMesh));

  worldMesh->m_vertexPosition.resize(4);
  worldMesh->m_vertexPosition[0] = glm::vec3{0.f, 0.f, 0.f};
  worldMesh->m_vertexPosition[1] = glm::vec3{0.f, 1.f, 0.f};
  worldMesh->m_vertexPosition[2] = glm::vec3{1.f, 0.f, 0.f};
  worldMesh->m_vertexPosition[3] = glm::vec3{1.f, 1.f, 0.f};

  for(unsigned int i{0}; i < 4; i++) {

      worldMesh->m_vertexColor.emplace_back(glm::vec3{0.55f, 0.45f, 0.1f});
  }

  for(unsigned int i{0}; i < 4; i++) {

      worldMesh->m_vertexNormal.emplace_back(glm::vec3{1.f, 0.f, 0.f});
  }

  worldMesh->m_vertexIndice.emplace_back(0);
  worldMesh->m_vertexIndice.emplace_back(1);
  worldMesh->m_vertexIndice.emplace_back(2);

  worldMesh->m_vertexIndice.emplace_back(1);
  worldMesh->m_vertexIndice.emplace_back(3);
  worldMesh->m_vertexIndice.emplace_back(2);

  worldMesh->reshape();

}
