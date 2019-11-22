#ifndef CHARACTER_UPDATE_HPP
#define CHARACTER_UPDATE_HPP

#include "Systems/AbstractSystem.hpp"

#include "Algorithms/Graphics/AnimationUpdate.hpp"
#include "Algorithms/SynchronizePositions.hpp"

class CharacterUpdate: public Gg::Systems::AbstractSystem {

	public:

		CharacterUpdate(Gg::GulgEngine &gulgEngine);
		
		virtual ~CharacterUpdate(); 

	protected:

};


#endif