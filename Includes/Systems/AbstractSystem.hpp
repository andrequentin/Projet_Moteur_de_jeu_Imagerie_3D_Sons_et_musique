#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <vector>
#include <memory>

#include "GulgEngine/GulgEngine.hpp"
#include "GulgEngine/GulgDeclarations.hpp"

#include "Algorithms/Algorithm.hpp"

namespace Gg {

namespace Systems {

class AbstractSystem {

	public:

		AbstractSystem(GulgEngine &gulgEngine);
		virtual ~AbstractSystem(); 

		void addEntity(const Entity newEntity);
		void deleteEntity(const Entity newEntity);

		virtual void applyAlgorithms();

	protected:

		std::vector<std::unique_ptr<Algorithm::AbstractAlgorithm>> m_algorithms;
		GulgEngine &m_gulgEngine;      
};

}}

#endif