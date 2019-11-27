#ifndef DRAW_SCENE_SYSTEM_HPP
#define DRAW_SCENE_SYSTEM_HPP

#include "Systems/System.hpp"

#include "Algorithms/DrawMesh.hpp"

class DrawScene: public Gg::Systems::System {

	public:

		DrawScene(Gg::GulgEngine &gulgEngine);
		
		virtual ~DrawScene();

		void setCameraEntity(const Gg::Entity camera);
		void setProjection(const glm::mat4 projection);

	private:

		Gg::Entity m_cameraEntity;
		glm::mat4 m_projectionMatrix;
};


#endif