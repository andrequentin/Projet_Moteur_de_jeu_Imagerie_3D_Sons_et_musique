#ifndef GULG_ENGINE_HPP
#define GULG_ENGINE_HPP

#include <GL/glew.h>
#include <GL/gl.h>

#include "GulgEngine/GulgDeclarations.hpp"

#include "GulgEngine/EntityCreator.hpp"
#include "GulgEngine/EntitySignatureKeeper.hpp"
#include "GulgEngine/SignatureLoader.hpp"
#include "GulgEngine/ComponentKeeper.hpp"
#include "GulgEngine/ProgramKeeper.hpp"
#include "GulgEngine/TextureKeeper.hpp"

#include "Components/Component.hpp"

namespace Gg {

class GulgEngine {

	public:

		GulgEngine();

		bool loadSignatures(const std::string path);

		Entity getNewEntity();
		void deleteEntity(const Entity entity);

		void addComponentToEntity(const Entity entity, const std::string name, std::shared_ptr<Component::AbstractComponent> component);
		void deleteComponentToEntity(const Entity entity, const std::string name);
		bool entityHasComponent(const Entity entity, const std::string name);

		Signature getEntitySignature(const Entity entity) const;

		std::shared_ptr<Component::AbstractComponent> getComponent(const Entity entity, const std::string name) const;
		Signature getComponentSignature(const std::string name) const;

		bool loadProgram(const std::string vertexPath, const std::string fragmentPath, const std::string name);
		GLuint getProgram(const std::string name) const;

		bool loadTexture(const std::string path, const std::string name);
		GLuint getTexture(const std::string name) const;

		Entity cloneEntity(const Entity entityToClone);

	private:

		void checkSignatureLoad() const;

		EntityCreator m_entityCreator;
		EntitySignatureKeeper m_entitySignatureKeeper;
		ComponentKeeper m_componentKeeper;
		SignatureLoader m_signatureLoader;
		ProgramKeeper m_programKeeper;
		TextureKeeper m_textureKeeper;

		bool m_signaturesAreLoaded;
                              
};

}

#endif