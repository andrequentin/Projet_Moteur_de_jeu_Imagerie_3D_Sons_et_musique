#include "Systems/UpdateScene.hpp"

UpdateScene::UpdateScene(Gg::GulgEngine &gulgEngine): System{gulgEngine} {

	m_algorithms.emplace_back(std::make_unique<Gg::Algorithm::UpdateTransformations>(gulgEngine));
}

UpdateScene::~UpdateScene() {}