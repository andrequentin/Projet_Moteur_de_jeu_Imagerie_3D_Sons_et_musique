#ifndef COMPONENT_KEEPER_HPP
#define COMPONENT_KEEPER_HPP

#include <string>
#include <map>
#include <memory>

#include "GulgEngine/GulgDeclarations.hpp"
#include "Components/Component.hpp"

namespace Gg {

class ComponentKeeper {

	public:

		ComponentKeeper();

		void addEntity(const Entity entity);
		void addComponent(const Entity entity, const std::string name, std::shared_ptr<Component::AbstractComponent> newComponent);

		void deleteEntity(const Entity entity);
		void deleteComponent(const Entity entity, const std::string name);

		bool entityExist(const Entity entity) const;
		bool entityHasComponent(const Entity entity, const std::string name) const;

		void cloneEntity(const Entity entityToClone, const Entity clone);
		

		std::shared_ptr<Component::AbstractComponent> getComponent(const Entity entity, const std::string name) const;

	private:

		std::map<Entity, std::map<std::string, std::shared_ptr<Component::AbstractComponent>>> m_components;
                              
};

}

#endif