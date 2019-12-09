#ifndef LIGHTNING_SYSTEM_HPP
#define LIGHTNING_SYSTEM_HPP

#include "Systems/System.hpp"

#include "Algorithms/UpdateLight.hpp"

class Lightning: public Gg::Systems::System {

	public:

		Lightning(Gg::GulgEngine &gulgEngine, const GLuint program);
		
		virtual ~Lightning();

		virtual void addEntity(const Gg::Entity newEntity);
		virtual void deleteEntity(const Gg::Entity newEntity);

		virtual void applyAlgorithms();

		unsigned int getNbMaxLight() const;
		unsigned int getCurrentNbLight() const;

	private:

		const unsigned int m_nbMaxLight;
		unsigned int m_currentNbLight;

		const GLuint m_program;
		const GLint m_nbLightID;
};


#endif