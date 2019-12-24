#ifndef LOAD_ANIMATION_HPP
#define LOAD_ANIMATION_HPP

#include <string>

#include "tinyxml2.h"

#include "GulgEngine/GulgEngine.hpp"
#include "GulgEngine/GulgDeclarations.hpp"

#include "Components/AnimatedMesh.hpp"

bool loadVertex(std::shared_ptr<Gg::Component::AnimatedMesh> mesh, tinyxml2::XMLElement *vertexDataNode);

bool loadTriangleNormalTextureCoords(std::shared_ptr<Gg::Component::AnimatedMesh> mesh, 
									 tinyxml2::XMLElement *normalDataNode,
									 tinyxml2::XMLElement *triangleDataNode);

bool loadMesh(std::shared_ptr<Gg::Component::AnimatedMesh> mesh, tinyxml2::XMLDocument &document);

bool loadAnimation(Gg::GulgEngine &engine, const Gg::Entity entity, const std::string path);


#endif