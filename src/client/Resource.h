#pragma once

#include <utility>
#include <memory>
#include <GL/glew.h>
#include <string>

class Config;
class Texture;
class Shader;
class Font;

using ConfigPtr = std::shared_ptr<Config>;
using TexturePtr = std::shared_ptr<Texture>;
using ShaderPtr = std::shared_ptr<Shader>;
using FontPtr = std::shared_ptr<Font>;

class Resource
{
public:
	enum ResourceType
	{
		TextureRes, ConfigRes, ShaderRes, FontRes
	};
private:
	static void registerResource(const std::string& key, const std::pair<ResourceType, std::shared_ptr<void>>& value);

public:
	static void init();
	static TexturePtr getTexture(const char* name);
	static ShaderPtr getShader(const char* name);
};

class Shader
{
	//!着色器信息
	struct ShaderInfo
	{
		GLenum       type;
		std::string  fileName;
	};

	GLuint mProgram = 0;

	Shader() = default;

public:
	~Shader()
	{
		glDeleteProgram(mProgram);
	}

	Shader(const Shader&) = delete;
	Shader(const Shader&&) = delete;
	Shader& operator=(const Shader&&) = delete;
	Shader& operator=(const Shader&) = delete;

	//!加载着色器
	static ShaderPtr loadShader(const char* fileName);

	//!获取Uniform位置
	GLuint getUniformLocation(const char* name) const
	{
		return glGetUniformLocation(mProgram, name);
	}

	//!绑定着色器
	void useShader() const
	{
		glUseProgram(mProgram);
	}
};

struct SDL_Surface;

class Texture
{
	GLuint mTextureId = 0;
	GLenum mTextureType = 0;

	int mHeight = 0;
	int mWidth = 0;

	Texture() = default;

public:
	GLenum & type = mTextureType;

	int& height = mHeight;
	int& width = mWidth;

	Texture(const Texture&) = delete;
	Texture(const Texture&&) = delete;
	Texture& operator=(const Texture&&) = delete;
	Texture& operator=(const Texture&) = delete;

	~Texture()
	{
		glDeleteTextures(1, &mTextureId);
	}

	static TexturePtr loadTexture(const char* fileName);
	static TexturePtr createFromSurface(SDL_Surface* surface);
	static TexturePtr create(GLuint height, GLuint width, GLenum type);

	void bind() const
	{
		glBindTexture(GL_TEXTURE_2D, mTextureId);
	}
};
