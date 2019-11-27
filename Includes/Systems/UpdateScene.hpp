#ifndef UPDATE_SCENE_SYSTEM_HPP
#define UPDATE_SCENE_SYSTEM_HPP

#include "Systems/System.hpp"

#include "Algorithms/UpdateTransformations.hpp"

class UpdateScene: public Gg::Systems::System {

	public:

		UpdateScene(Gg::GulgEngine &gulgEngine);
		
		virtual ~UpdateScene();
};


#endif