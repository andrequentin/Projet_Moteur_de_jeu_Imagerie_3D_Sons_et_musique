#include "GulgEngine/GulgEngine.hpp"

#include "Components/Mesh.hpp"
#include "Components/Transformation.hpp"
#include "Components/SceneObject.hpp"
#include "Components/VoxelMap.hpp"

void generateWorld(VoxelMap &currentMap, unsigned int interpolationFrequency){

  // BRUIT PERLIN ALÉATOIRE
  std::vector<std::vector<unsigned int>> bruit;
  std::array<unsigned int, 3> worldDimension{currentMap.getWorldDimensions()};
  bruit.resize(worldDimension[0]);

  auto const seed = std::time(nullptr);
  std::default_random_engine engin { seed };
  for(unsigned  x{0};x < worldDimension[0] ;x++){
    for(unsigned int y{0};y < worldDimension[1]; y++){
      bruit[x].push_back(engin()%(worldDimension[2]/interpolationFrequency)+1);
      // std::cout<<bruit[x][y]<<"\t";
    }
    // std::cout<<std::endl;
  }

  if(worldDimension[0] % interpolationFrequency != 0 || worldDimension[1] % interpolationFrequency != 0){
    throw std::runtime_error("Error : worldDimension[0] and worldDimension[1] must be multiples of interpolationFrequency");
  }

  //INTERPOLATION LINÉAIRE 2D
  for(unsigned int f{0};f < interpolationFrequency;f++){
    for(unsigned int g{0};g < interpolationFrequency;g++){
      unsigned int minX{(worldDimension[0]*f)/interpolationFrequency};
      unsigned int  maxX{(worldDimension[0]*(f+1))/interpolationFrequency-1};
      if(f>0)minX--;
      unsigned int  minY{(worldDimension[1]*g)/interpolationFrequency};
      unsigned int  maxY{(worldDimension[1]*(g+1))/interpolationFrequency-1};
      if(g>0)minY--;
      double  a (bruit[minX][minY]), // point d'interpolation 0-0
              b (bruit[minX][maxY]), // point d'interpolation 0-1
              c (bruit[maxX][minY]), // point d'interpolation 1-0
              d (bruit[maxX][maxY]); // point d'interpolation 1-1

      for(unsigned int i = minX; i <= maxX; i++){
        for(unsigned int j = minY; j <= maxY; j++){
          double  u=static_cast<double>(i-minX)/static_cast<double>(maxX-minX), // distance entre a et la coordonées x du point courant
                  v=static_cast<double>(j-minY)/static_cast<double>(maxY-minY); // distance entre b et la coordonées y du point courant
          double  ap (a*(1.0-u) + c*(u)), // ap = interpolation a-c distance u
                  bp (b*(1.0-u) + d*(u)); // bp = interpolation b-d distance u
          bruit[i][j] = static_cast<unsigned int>(ap*(1.0-v) + bp*(v)); //interpolation ap-bp distance v
        }
      }
    }
  }
  // std::cout<<"--------------"<<std::endl;

  for(unsigned int x{0};x < worldDimension[0] ;x++){
    for(unsigned int y{0};y < worldDimension[1]; y++){
      // std::cout<<bruit[x][y]<<"\t";
      for(unsigned int z{0};z<bruit[x][y];z++){
        currentMap.setColor(currentMap.getVoxelID(x,y,z), glm::vec4{(static_cast<float>(z)/worldDimension[2]),
                                                                    (static_cast<float>(z)/worldDimension[2]), 
                                                                    (static_cast<float>(z)/worldDimension[2]), 1.f});
      }
    }
    // std::cout<<std::endl;
  }
}

std::vector<std::pair<unsigned int, glm::vec3>> voxelsAndOrientations(const unsigned int voxelMapSize) {

  std::vector<std::pair<unsigned int, glm::vec3>> result;
  result.reserve(voxelMapSize*6);

  for(unsigned int i{0}; i < voxelMapSize; i++) {

    result.emplace_back(std::make_pair(i, glm::vec3{1.f, 0.f, 0.f}));
    result.emplace_back(std::make_pair(i, glm::vec3{-1.f, 0.f, 0.f}));
    result.emplace_back(std::make_pair(i, glm::vec3{0.f, 1.f, 0.f}));
    result.emplace_back(std::make_pair(i, glm::vec3{0.f, -1.f, 0.f}));
    result.emplace_back(std::make_pair(i, glm::vec3{0.f, 0.f, 1.f}));
    result.emplace_back(std::make_pair(i, glm::vec3{0.f, 0.f, -1.f}));
  }

  return result;
}

std::vector<std::pair<unsigned int, glm::vec3>> selectVisibleFaces(const VoxelMap &map, const std::vector<std::pair<unsigned int, glm::vec3>> &facesToSelect) {

  std::vector<std::pair<unsigned int, glm::vec3>> result;
  result.reserve(facesToSelect.size());

  glm::vec3 testingFace;
  std::array<unsigned int, 3> worldSize{map.getWorldDimensions()};

  for(const std::pair<unsigned int, glm::vec3> &currendFace: facesToSelect) {

    testingFace = map.getVoxelPosition(currendFace.first) + currendFace.second;

    if(map.getColor(currendFace.first) != glm::vec4{0.f, 0.f, 0.f, 0.f}) { // Delete void voxel

      //Out of bounds, have to draw it
      if(testingFace.x < 0.f || testingFace.x >= worldSize[0]
      || testingFace.y < 0.f || testingFace.y >= worldSize[1]
      || testingFace.z < 0.f || testingFace.z >= worldSize[2]) {

        result.emplace_back(currendFace);
      }

      //No neighbourg
      else if(map.getColor(testingFace.x, testingFace.y, testingFace.z) == glm::vec4{0.f, 0.f, 0.f, 0.f}) {

        result.emplace_back(currendFace);
      }
    }
  }

  return result;
}

std::vector<glm::vec3> getFaceFromOrientation(const glm::vec3 &position, const glm::vec3 &orientation) {

  std::vector<glm::vec3> quad;
  quad.resize(4);

  if(orientation[0] == 0.f && orientation[1] == 0.f) {

    quad[0] = (position + orientation/2.f + glm::vec3{-0.5f, 0.5f, 0.f}); 
    quad[1] = (position + orientation/2.f + glm::vec3{-0.5f, -0.5f, 0.f}); 
    quad[2] = (position + orientation/2.f + glm::vec3{0.5f, -0.5f, 0.f}); 
    quad[3] = (position + orientation/2.f + glm::vec3{0.5f, 0.5f, 0.f}); 
  }

  else if(orientation[0] == 0.f && orientation[2] == 0.f) {

    quad[0] = (position + orientation/2.f + glm::vec3{-0.5f, 0.f, 0.5f}); 
    quad[1] = (position + orientation/2.f + glm::vec3{-0.5f, 0.f, -0.5f}); 
    quad[2] = (position + orientation/2.f + glm::vec3{0.5f, 0.f, -0.5f}); 
    quad[3] = (position + orientation/2.f + glm::vec3{0.5f, 0.f, 0.5f}); 
  }

  else if(orientation[1] == 0.f && orientation[2] == 0.f) {

    quad[0] = (position + orientation/2.f + glm::vec3{0.f, -0.5f, 0.5f}); 
    quad[1] = (position + orientation/2.f + glm::vec3{0.f, -0.5f, -0.5f}); 
    quad[2] = (position + orientation/2.f + glm::vec3{0.f, 0.5f, -0.5f}); 
    quad[3] = (position + orientation/2.f + glm::vec3{0.f, 0.5f, 0.5f}); 
  }

  return quad;
}

void worldMapToMesh(VoxelMap &map, Gg::Component::Mesh &mesh) {

  mesh.m_vertexPosition.clear();
  mesh.m_vertexColor.clear();
  mesh.m_vertexNormal.clear();
  std::array<unsigned int, 3> worldDimensions{map.getWorldDimensions()};

  std::vector<std::pair<unsigned int, glm::vec3>> allFaces{voxelsAndOrientations(worldDimensions[0]*worldDimensions[1]*worldDimensions[2])};
  allFaces = selectVisibleFaces(map, allFaces);

  std::vector<glm::vec3> currentQuad;

  for(std::pair<unsigned int, glm::vec3> currentFace: allFaces) {

    currentQuad = getFaceFromOrientation(map.getVoxelPosition(currentFace.first), currentFace.second);
    mesh.m_vertexPosition.emplace_back(currentQuad[0]);
    mesh.m_vertexPosition.emplace_back(currentQuad[1]);
    mesh.m_vertexPosition.emplace_back(currentQuad[2]);

    mesh.m_vertexPosition.emplace_back(currentQuad[0]);
    mesh.m_vertexPosition.emplace_back(currentQuad[3]);
    mesh.m_vertexPosition.emplace_back(currentQuad[2]);

    mesh.m_vertexColor.emplace_back(map.getColor(currentFace.first));
  }

}




void newMap(Gg::GulgEngine & engine, Gg::Entity &worldID, GLuint program){

  std::shared_ptr<Gg::Component::SceneObject> worldScene{std::make_shared<Gg::Component::SceneObject>()};
  std::shared_ptr<Gg::Component::Transformation> worldTransformation{std::make_shared<Gg::Component::Transformation>()};
  std::shared_ptr<Gg::Component::Mesh> worldMesh{std::make_shared<Gg::Component::Mesh>(program)};
  std::shared_ptr<VoxelMap> worldMap{std::make_shared<VoxelMap>(80, 80, 80)};

  engine.addComponentToEntity(worldID, "SceneObject", std::static_pointer_cast<Gg::Component::AbstractComponent>(worldScene));
  engine.addComponentToEntity(worldID, "Transformations", std::static_pointer_cast<Gg::Component::AbstractComponent>(worldTransformation));
  engine.addComponentToEntity(worldID, "MainMesh", std::static_pointer_cast<Gg::Component::AbstractComponent>(worldMesh));
  engine.addComponentToEntity(worldID, "VoxelMap", std::static_pointer_cast<Gg::Component::AbstractComponent>(worldMap));

  generateWorld(*worldMap, 4);
  worldMapToMesh(*worldMap, *worldMesh);

  worldMesh->reshape();
}