#include "Systems/DrawScene.hpp"

DrawScene::DrawScene(Gg::GulgEngine &gulgEngine): 
	System{gulgEngine},
	m_cameraEntity{Gg::NoEntity},
	m_projectionMatrix{1.f} {

	m_algorithms.emplace_back(std::make_unique<Gg::Algorithm::DrawMesh>("MainMesh", m_cameraEntity, m_projectionMatrix, gulgEngine));
}

DrawScene::~DrawScene() {}

void DrawScene::setCameraEntity(const Gg::Entity camera) { m_cameraEntity = camera; }

void DrawScene::setProjection(const glm::mat4 projection) { m_projectionMatrix = projection; }