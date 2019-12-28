#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <vector>
#include <algorithm>

#include "GulgEngine/GulgEngine.hpp"
#include "GulgEngine/GulgDeclarations.hpp"

namespace Gg {

namespace Algorithm {

class AbstractAlgorithm {

	public:

		AbstractAlgorithm(GulgEngine &gulgEngine);
		virtual ~AbstractAlgorithm();

		void addEntity(const Entity newEntity);
		void deleteEntity(const Entity entity);

		Signature getSignature() const;

		virtual void apply() = 0;

	protected:

		GulgEngine &m_gulgEngine;
		std::vector<Entity> m_entitiesToApply;

		Signature m_signature;
};

}}

#endif
