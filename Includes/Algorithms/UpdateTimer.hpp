#ifndef UPDATE_TIMER_ALGORITHM_HPP
#define UPDATE_TIMER_ALGORITHM_HPP

#include "Algorithms/Algorithm.hpp"
#include "Systems/Time.hpp"
#include "Components/SceneObject.hpp"
#include "Components/Collider.hpp"
#include "Components/Forces.hpp"
#include "Components/VoxelMap.hpp"
#include "Components/Explosive.hpp"

#include "Components/Timer.hpp"

#include "NewMap.hpp"

namespace Gg {

namespace Algorithm {

class UpdateTimer: public AbstractAlgorithm {

	public:

		UpdateTimer(GulgEngine &gulgEngine, Gg::Entity &w, Time* c );
		virtual ~UpdateTimer();

		void apply();


	private:
    Gg::Entity &world;
		 Time* timeSystem;
};

}}

#endif
