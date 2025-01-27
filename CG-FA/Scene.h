#pragma once

#include <vector>
#include <unordered_map>
#include <map>
#include <memory>

#include "BaseEntity.h"
#include "Player.h"
#include "SpaceBox.h"

// Light object
struct Light
{
	// Color of light
	glm::vec3 color;
	// Position of light
	glm::vec3 position;

	std::function<void(const float)> Move;

	Light(const glm::vec3& position, const glm::vec3& color) : position(position), color(color) {}
};

class Scene
{
public:
	// Player in scene
	Player player;

	// Skybox
	SpaceBox skybox;

	// All lights in scene
	std::vector<Light> lights;

	Scene() = default;

	// Add an entity to the scene, and return the added entity
	// This scene is expected to be the solo owner of this entity, as it creates a unique pointer
	template<class EntityType>
	EntityType& AddEntity(EntityType* entity)
	{
		// Add entity to the map of entities sorted by shader, polymorph to a BaseEntity, and echo added entity back
		return *static_cast<EntityType*>(entities[entity->GetMaterial()->shader].emplace_back(entity).get());
	}

	// Renderloop of scene to be called each frame
	void RenderLoop(const float delta);

private:
	// Map with shader id as key, to a list of all entities that use that shader
	using ShaderEntityMap = std::unordered_map<GLuint, std::vector<std::unique_ptr<BaseEntity>>>;

	// Map of all the shader locations of stored lights
	using ShaderLightsMap = std::map<std::pair<GLuint, std::string>, GLint>;

	// All entities present in scene sorted per shader
	ShaderEntityMap entities;

	mutable ShaderLightsMap light_cache;

	int GetLightLocation(const GLuint shader, const char* name) const;

	void setLights(const GLuint shader);
};