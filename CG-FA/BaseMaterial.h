#pragma once

#include <unordered_map>
#include <optional>

#include <GL/glew.h>
#include <glm/glm.hpp>

struct Texture
{
	GLuint ID;
	GLenum unit;

	Texture() = default;
	Texture(const char* path, bool isColor = true);
	Texture(GLuint id) : ID(id), unit(unit_count++) {}

private:
	static inline GLenum unit_count = 1;
};

struct BaseMaterial
{
	static constexpr const char* MAT_COLOR = "material.color";
	static constexpr const char* MAT_METALLIC = "material.metallic";
	static constexpr const char* MAT_ROUGHNESS = "material.roughness";

	glm::vec3 color = glm::vec3(0.0f);
	float metallic = 0.0f;
	float roughness = 1.0f;

	// Shader intended to use with this material
	GLuint shader = 0;

	// Needs to be called inside material constuctor to initialize and set shader
	void InitShaderProgram(const char* vertexFile, const char* fragmentFile);

	// Materials can share the same shader if the shader name is the same
	virtual const char* GetShaderName() const = 0;

	// Bind all properties of this material to the currently bound shader
	void Bind() const;

#pragma region Set uniforms

	template<class UniType>
	void SetUniform(const UniType& v, const char* name)  const { static_assert(false, "Uniform type not supported"); }

	template<>
	void SetUniform<float>(const float& v, const char* name) const
	{
		glUniform1f(GetUniformLocation(name), v);
	}

	template<>
	void SetUniform<bool>(const bool& b, const char* name) const
	{
		glUniform1i(GetUniformLocation(name), b);
	}

	template<>
	void SetUniform<glm::vec3>(const glm::vec3& v, const char* name) const
	{
		glUniform3fv(GetUniformLocation(name), 1, &v[0]);
	}

	template<>
	void SetUniform<GLenum>(const GLenum& v, const char* name) const
	{
		glUniform1i(GetUniformLocation(name), v);
	}

	template<>
	void SetUniform<glm::mat4>(const glm::mat4& v, const char* name) const
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &v[0][0]);
	}

	template<>
	void SetUniform<Texture>(const Texture& texture, const char* name) const
	{
		glUniform1i(GetUniformLocation(name), texture.unit);
	}

#pragma endregion

protected:
	// Bind all extensions of material
	virtual void bind() const = 0;

private:
	// Currently bound material
	static inline const BaseMaterial* CURR_MAT = 0;

	// Cache to store all uniform locations of this material
	mutable std::unordered_map<const char*, GLint> UNIFORM_CACHE;

	// Cache to track all different shaders
	static inline std::unordered_map<const char*, GLuint> SHADER_CACHE;

	// Get cached uniform location inside the shader of this material
	int GetUniformLocation(const char* name) const;
};

struct TexturedMaterial : public BaseMaterial
{
	static constexpr const char* TEX_DIFFUSE = "tex_diffuse";
	static constexpr const char* HAS_DIFFUSE = "has_diffusemap";
	static constexpr const char* TEX_GLOSS = "tex_gloss";
	static constexpr const char* HAS_GLOSS = "has_glossmap";
	static constexpr const char* TEX_NORMAL = "tex_normal";
	static constexpr const char* HAS_NORMAL = "has_normalmap";

	std::optional<Texture> diffuse;
	std::optional<Texture> gloss;
	std::optional<Texture> normal;

	TexturedMaterial() = default;

	virtual ~TexturedMaterial()
	{
		if (diffuse)
			glDeleteTextures(1, &diffuse.value().ID);
		if (gloss)
			glDeleteTextures(1, &gloss.value().ID);
		if (normal)
			glDeleteTextures(1, &normal.value().ID);
	}

	// Inherited via BaseMaterial
	virtual void bind() const override;
	virtual inline const char* GetShaderName() const override { return "texmat"; }
};
