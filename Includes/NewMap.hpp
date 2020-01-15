#ifndef NEW_MAP_HPP
#define NEW_MAP_HPP

#include <glm/gtx/normal.hpp>

#include <FMOD/fmod_studio.hpp>
#include <FMOD/fmod_errors.h>

#include "GulgEngine/GulgEngine.hpp"

#include "Components/Mesh.hpp"
#include "Components/Transformation.hpp"
#include "Components/SceneObject.hpp"
#include "Components/VoxelMap.hpp"

std::vector<glm::vec3> generateWorld(VoxelMap &currentMap, const unsigned int interpolationFrequency);

std::vector<std::pair<unsigned int, glm::vec3>> voxelsAndOrientations(const unsigned int voxelMapSize);

std::vector<std::pair<unsigned int, glm::vec3>> selectVisibleFaces(const VoxelMap &map, const std::vector<std::pair<unsigned int, glm::vec3>> &facesToSelect);

std::vector<glm::vec3> getFaceFromOrientation(const glm::vec3 &position, const glm::vec3 &orientation);

void localRemeshing(std::vector<std::vector<unsigned int>> v,VoxelMap &map, Gg::Component::Mesh &mesh);

void worldMapToMesh(VoxelMap &map, Gg::Component::Mesh &mesh);

void Cube(std::shared_ptr<Gg::Component::Mesh> mesh , float size,glm::vec3 color);

std::vector<FMOD::Studio::EventInstance*> generateBirds(std::vector<glm::vec3> birdPosition, FMOD::Studio::EventDescription *birdDescription);

std::vector<FMOD::Studio::EventInstance*> newMap(Gg::GulgEngine & engine, Gg::Entity &worldID, GLuint program, FMOD::Studio::EventDescription *birdDescription);


#endif
