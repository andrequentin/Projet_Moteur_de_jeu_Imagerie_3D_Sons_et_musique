#ifndef ENTITY_SIGNATURE_KEEPER_HPP
#define ENTITY_SIGNATURE_KEEPER_HPP

#include <string>
#include <map>

#include "GulgEngine/Signature.hpp"
#include "GulgEngine/GulgDeclarations.hpp"

namespace Gg {

class EntitySignatureKeeper {

	public:

		EntitySignatureKeeper(const size_t signatureSize = 0);

		void resetSignatures(const size_t signatureSize);

		void addEntity(const Entity entity);
		void deleteEntity(const Entity entity);

		bool entityExist(const Entity entity) const;

		void addToSignature(const Entity entity, const Signature signature);
		void deleteToSignature(const Entity entity, const Signature signature);

		void cloneEntity(const Entity entityToClone, const Entity clone);
		
		Signature getSignature(const Entity entity) const;

	private:

		std::map<Entity, Signature> m_signatures;
		size_t m_signatureSize;
                              
};

}

#endif