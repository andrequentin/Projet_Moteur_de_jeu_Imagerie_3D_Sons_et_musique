#include "Systems/Lightning.hpp"

Lightning::Lightning(Gg::GulgEngine &gulgEngine, const GLuint program): 
	System{gulgEngine},
	m_nbMaxLight{32},
	m_currentNbLight{0},
	m_program{program},
	m_nbLightID{glGetUniformLocation(m_program, "LightNumber")} {

	m_algorithms.emplace_back(std::make_unique<Gg::Algorithm::UpdateLight>("Light", program, gulgEngine));
}

Lightning::~Lightning() {}


void Lightning::addEntity(const Gg::Entity newEntity) {

	for(std::unique_ptr<Gg::Algorithm::AbstractAlgorithm> &currentAlgo: m_algorithms) {

		if((m_gulgEngine.getEntitySignature(newEntity) & currentAlgo->getSignature()) == currentAlgo->getSignature()) {

			currentAlgo->addEntity(newEntity);
		}
	}

	m_currentNbLight++;
}

void Lightning::deleteEntity(const Gg::Entity newEntity) {

	for(std::unique_ptr<Gg::Algorithm::AbstractAlgorithm> &currentAlgo: m_algorithms) { currentAlgo->deleteEntity(newEntity); }
	if(m_currentNbLight >= 1) { m_currentNbLight--; }
} 

void Lightning::applyAlgorithms() {

	glUseProgram(m_program);
	glUniform1ui(m_nbLightID, m_currentNbLight);

	for(std::unique_ptr<Gg::Algorithm::AbstractAlgorithm> &currentAlgo: m_algorithms) { currentAlgo->apply(); }
}

unsigned int Lightning::getNbMaxLight() const { return m_nbMaxLight; }
unsigned int Lightning::getCurrentNbLight() const { return m_currentNbLight; }