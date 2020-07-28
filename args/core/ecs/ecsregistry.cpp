#include <core/ecs/ecsregistry.hpp>
#include <core/ecs/entity.hpp>
#include <core/ecs/component_container.hpp>
#include <core/ecs/component_handle.hpp>

namespace args::core::ecs
{
	// 2 because the world entity is 1 and 0 is invalid_id
	id_type EcsRegistry::m_lastEntityId = 2;

	EcsRegistry::EcsRegistry() : m_families(), m_entityData(), m_entities(), m_queryRegistry(*this)
	{
		// Create world entity.
		m_entityData.emplace(1);
		m_entities.emplace(1, 1, this);
	}

	inline component_container_base* EcsRegistry::getFamily(id_type componentTypeId)
	{
		if (!m_families.contains(componentTypeId))
			throw args_unknown_component_error;

		return m_families[componentTypeId];
	}

	inline component_handle_base EcsRegistry::getComponent(id_type entityId, id_type componentTypeId)
	{
		if (getEntity(entityId).has_component(componentTypeId))
			return component_handle_base(entityId, *this);
		return component_handle_base(invalid_id, *this);
	}

	inline component_handle_base EcsRegistry::createComponent(id_type entityId, id_type componentTypeId)
	{
		if (!validateEntity(entityId))
			throw args_entity_not_found_error;

		getFamily(componentTypeId)->create_component(entityId);

		m_entityData[entityId].components.insert(componentTypeId, componentTypeId);
		m_queryRegistry.evaluateEntityChange(entityId, componentTypeId, true);

		return component_handle_base(entityId, *this);
	}

	inline void EcsRegistry::destroyComponent(id_type entityId, id_type componentTypeId)
	{
		if (!validateEntity(entityId))
			throw args_entity_not_found_error;

		getFamily(componentTypeId)->destroy_component(entityId);

		m_entityData[entityId].components.erase(componentTypeId);
		m_queryRegistry.evaluateEntityChange(entityId, componentTypeId, true);
	}

	A_NODISCARD inline bool EcsRegistry::validateEntity(id_type entityId)
	{
		return m_entities.contains(entityId);
	}

	inline entity EcsRegistry::createEntity()
	{
		id_type id = m_lastEntityId++;

		if (validateEntity(id))
			throw args_entity_exists_error;

		m_entityData[id] = {};
		m_entities.emplace(id, id, this);

		return m_entities[id];
	}

	inline void EcsRegistry::destroyEntity(id_type entityId)
	{
		if (!validateEntity(entityId))
			throw args_entity_not_found_error;

		for (id_type componentTypeId : m_entityData[entityId].components)
			m_families[componentTypeId]->destroy_component(entityId);

		m_entityData.erase(entityId);
		m_entities.erase(entityId);

		m_queryRegistry.markEntityDestruction(entityId);
	}

	A_NODISCARD inline entity EcsRegistry::getEntity(id_type entityId)
	{
		if (!validateEntity(entityId))
			return entity(invalid_id, this);

		return m_entities[entityId];
	}

	A_NODISCARD inline entity_data& EcsRegistry::getEntityData(id_type entityId)
	{
		if (!validateEntity(entityId))
			throw args_entity_not_found_error;

		return m_entityData[entityId];
	}

	A_NODISCARD inline sparse_map<id_type, entity>::dense_value_container& EcsRegistry::getEntities()
	{
		return m_entities.dense();
	}
}