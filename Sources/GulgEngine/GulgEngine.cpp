#include "GulgEngine/GulgEngine.hpp"

namespace Gg {

GulgEngine::GulgEngine(): m_signaturesAreLoaded{false} {}

bool GulgEngine::loadSignatures(const std::string path) {

	m_signaturesAreLoaded = m_signatureLoader.loadFile(path);
	if(!m_signaturesAreLoaded) { 

		std::cout << "GulgEngine warning: can't load signature file\"" << path << "\"." << std::endl;
	}

	else { m_entitySignatureKeeper.resetSignatures(m_signatureLoader.getNumberOfSignatures()); }

	return m_signaturesAreLoaded;
}

Entity GulgEngine::getNewEntity() {

	checkSignatureLoad();

	Entity newEntity{m_entityCreator.createEntity()};
	m_entitySignatureKeeper.addEntity(newEntity);
	m_componentKeeper.addEntity(newEntity);

	return newEntity;
}

void GulgEngine::deleteEntity(const Entity entity) {

	checkSignatureLoad();

	m_entityCreator.freeEntity(entity);
	m_entitySignatureKeeper.deleteEntity(entity);
	m_componentKeeper.deleteEntity(entity);
}

void GulgEngine::addComponentToEntity(const Entity entity, const std::string name, std::shared_ptr<Component::AbstractComponent> component) {

	checkSignatureLoad();

	m_componentKeeper.addComponent(entity, name, component);
	m_entitySignatureKeeper.addToSignature(entity, m_signatureLoader.getSignature(name));
}

void GulgEngine::deleteComponentToEntity(const Entity entity, const std::string name) {

	checkSignatureLoad();

	m_componentKeeper.deleteComponent(entity, name);
	m_entitySignatureKeeper.deleteToSignature(entity, m_signatureLoader.getSignature(name));
}

bool GulgEngine::entityHasComponent(const Entity entity, const std::string name) {

	return m_componentKeeper.entityHasComponent(entity, name);
}

Signature GulgEngine::getEntitySignature(const Entity entity) const {

	checkSignatureLoad();
	return m_entitySignatureKeeper.getSignature(entity);
}

std::shared_ptr<Component::AbstractComponent> GulgEngine::getComponent(const Entity entity, const std::string name) const {

	checkSignatureLoad();
	return m_componentKeeper.getComponent(entity, name);
}

Signature GulgEngine::getComponentSignature(const std::string name) const {

	checkSignatureLoad();
	return m_signatureLoader.getSignature(name);
}

void GulgEngine::checkSignatureLoad() const {

	if(!m_signaturesAreLoaded) {

		throw std::runtime_error("GulgEngine error: signature file isn't loaded.");
	}
}

bool GulgEngine::loadProgram(const std::string vertexPath, const std::string fragmentPath, const std::string name) {

	return m_programKeeper.loadProgram(vertexPath, fragmentPath, name);
}

GLuint GulgEngine::getProgram(const std::string name) const { return m_programKeeper.getProgram(name); }



bool GulgEngine::loadTexture(const std::string path, const std::string name) {

	return m_textureKeeper.loadTexture(path, name);
}

GLuint GulgEngine::getTexture(const std::string name) const { return m_textureKeeper.getTexture(name); }



Entity GulgEngine::cloneEntity(const Entity entityToClone) {

	Entity newEntity{getNewEntity()};
	m_entitySignatureKeeper.addToSignature(newEntity, m_entitySignatureKeeper.getSignature(entityToClone));
	m_componentKeeper.cloneEntity(entityToClone, newEntity);

	return newEntity;
}

}