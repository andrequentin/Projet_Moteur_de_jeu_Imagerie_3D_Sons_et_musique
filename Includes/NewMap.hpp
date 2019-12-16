#ifndef NEW_MAP_HPP
#define NEW_MAP_HPP

#include <glm/gtx/normal.hpp>

#include "GulgEngine/GulgEngine.hpp"

#include "Components/Mesh.hpp"
#include "Components/Transformation.hpp"
#include "Components/SceneObject.hpp"
#include "Components/VoxelMap.hpp"

void generateWorld(VoxelMap &currentMap, const unsigned int interpolationFrequency);

std::vector<std::pair<unsigned int, glm::vec3>> voxelsAndOrientations(const unsigned int voxelMapSize);

std::vector<std::pair<unsigned int, glm::vec3>> selectVisibleFaces(const VoxelMap &map, const std::vector<std::pair<unsigned int, glm::vec3>> &facesToSelect);

std::vector<glm::vec3> getFaceFromOrientation(const glm::vec3 &position, const glm::vec3 &orientation);

void worldMapToMesh(VoxelMap &map, Gg::Component::Mesh &mesh);

void Square(std::shared_ptr<Gg::Component::Mesh> mesh);

void newMap(Gg::GulgEngine & engine, Gg::Entity &worldID, GLuint program);

#endif
