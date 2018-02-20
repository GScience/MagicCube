#include "Resource.h"
#include <unordered_map>
#include <string>
#include <dirent.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

std::unordered_map<std::string, std::pair<Resource::ResourceType, std::shared_ptr<void>>> resourceMap;

void Resource::registerResource(const std::string& key, const std::pair<ResourceType, std::shared_ptr<void>>& value)
{
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Register resource %s", key.c_str());
	resourceMap[key] = value;
}

void enumFile(std::vector<std::string>& files, const std::string& basePath = ".")
{
	const auto dir = opendir(basePath.c_str());
	struct dirent *direntPtr;

	while ((direntPtr = readdir(dir)) != nullptr)
	{
		if (strcmp(direntPtr->d_name, ".") == 0 || strcmp(direntPtr->d_name, "..") == 0)
			continue;
		if (direntPtr->d_type == DT_REG)
			if (basePath == ".")
				files.emplace_back(direntPtr->d_name);
			else
				files.emplace_back(basePath + "/" + direntPtr->d_name);
		else if (direntPtr->d_type == DT_DIR)
			if (basePath == ".")
				enumFile(files, direntPtr->d_name);
			else
				enumFile(files, basePath + "/" + direntPtr->d_name);
	}
}

void Resource::init()
{
	std::vector<std::string> files;
	
	enumFile(files);

	for (const auto& fileName : files)
	{
		auto resKey = fileName;

		for (auto& c : resKey)
			if (c == '\\' || c == '/')
				c = '.';

		const auto findResult = fileName.find_last_of('.');

		//是否有扩展名
		if (findResult == std::string::npos)
			continue;

		const auto extension = fileName.substr(findResult);

		if (extension == ".png")
			registerResource
			(
				resKey,
				std::make_pair
				(
					TextureRes,
					Texture::loadTexture(fileName.c_str())
				)
			);
		else if (extension == ".shader")
			registerResource
			(
				resKey,
				std::make_pair
				(
					ShaderRes,
					Shader::loadShader(fileName.c_str())
				)
			);
	}
}

TexturePtr Resource::getTexture(const char* name)
{
	const auto result = resourceMap.find(name);

	if (result == resourceMap.end())
		throw(std::invalid_argument(std::string("Resource doesn't exist:") + name));
	if (result->second.first != TextureRes)
		throw(std::invalid_argument(std::string("Resource type doesn't equal") + name));

	return std::static_pointer_cast<Texture>(result->second.second);
}

ShaderPtr Resource::getShader(const char* name)
{
	const auto result = resourceMap.find(name);

	if (result == resourceMap.end())
		throw std::invalid_argument(std::string("Resource doesn't exist:") + name);
	if (result->second.first != ShaderRes)
		throw std::invalid_argument(std::string("Resource type doesn't equal") + name);

	return std::static_pointer_cast<Shader>(result->second.second);
}

ShaderPtr Shader::loadShader(const char* fileName)
{
	//获取着色器信息
	std::vector<ShaderInfo> shaders;
	std::ifstream shaderFile(fileName);
	while (!shaderFile.eof())
	{
		std::string shaderFileName;
		std::string shaderType;

		shaderFile >> shaderFileName;
		shaderFile >> shaderType;

		if (shaderType == "GL_VERTEX_SHADER")
			shaders.push_back({ GL_VERTEX_SHADER, shaderFileName });
		else if (shaderType == "GL_FRAGMENT_SHADER")
			shaders.push_back({ GL_FRAGMENT_SHADER, shaderFileName });
		else
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Unknown shader type : %s", shaderType.c_str());
	}

	ShaderPtr shaderPtr(new Shader());

	//检测是否为空
	if (shaders.empty())
		return shaderPtr;

	shaders.push_back({ GL_NONE ,"" });

	//创建着色器程序
	const auto program = glCreateProgram();

	auto entry = shaders.begin();

	//循环编译着色器
	while (entry->type != GL_NONE)
	{
		//创建Shader
		const auto shader = glCreateShader(entry->type);

		//读取Shader代码
		std::ifstream shaderFileStream(entry->fileName);
		std::stringstream shaderStream;
		shaderStream << shaderFileStream.rdbuf();

		const auto source = shaderStream.str();

		//检测代码是否为空
		if (source.empty())
		{
			//删除Shader
			for (entry = shaders.begin(); entry->type != GL_NONE; ++entry)
				glDeleteShader(shader);

			break;
		}
		auto sourcePtr = source.c_str();

		//绑定源码
		glShaderSource(shader, 1, &sourcePtr, nullptr);

		//编译源码
		glCompileShader(shader);

		//获取编译状态
		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		//如果编译出错
		if (!compiled)
		{
			//获取错误并输出
			GLsizei len;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

			const auto log = new GLchar[len + 1];
			glGetShaderInfoLog(shader, len, &len, log);
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Shader compilation failed: %s", log);
			delete[] log;
			throw std::runtime_error("Shader compilation failed");
		}

		//绑定Shader到着色器程序
		glAttachShader(program, shader);

		++entry;
	}
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		const auto log = new GLchar[len + 1];
		glGetProgramInfoLog(program, len, &len, log);
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Shader linking failed: ");
		delete[] log;
		throw std::runtime_error("Shader linking failed");
	}
	glUseProgram(program);
	shaderPtr->mProgram = program;

	return shaderPtr;
}

TexturePtr Texture::createFromSurface(SDL_Surface* surface)
{
	TexturePtr texture(new Texture());

	glGenTextures(1, &texture->mTextureId);
	glBindTexture(GL_TEXTURE_2D, texture->mTextureId);

	//create ogl texture
	const auto bytes = surface->format->BytesPerPixel;
	const auto bits = surface->format->BitsPerPixel;

	if (bytes == 3) texture->mTextureType = GL_RGB;
	else if (bytes == 4) texture->mTextureType = GL_RGBA;
	else
		throw(std::invalid_argument("Wrong image format "));

	const auto bitPerColor = bits / bytes;

	GLuint dataType;

	if (bitPerColor == 8) dataType = GL_UNSIGNED_BYTE;
	else
		throw(std::invalid_argument("Wrong image bit "));

	glTexImage2D(GL_TEXTURE_2D, 0, texture->mTextureType, surface->w, surface->h, 0, texture->mTextureType, dataType,
		surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture->mHeight = surface->h;
	texture->mWidth = surface->w;

	return texture;
}

TexturePtr Texture::loadTexture(const char* fileName)
{
	//load image
	const auto surface = IMG_Load(fileName);
	try
	{
		auto texture = createFromSurface(surface);
		SDL_FreeSurface(surface);

		return texture;
	}
	catch (std::exception& e)
	{
		SDL_FreeSurface(surface);
		throw std::invalid_argument(std::string("failed to load image ") + fileName + " because " + e.what());
	}
}

TexturePtr Texture::create(const GLuint height, const GLuint width, const GLenum type)
{
	TexturePtr texture(new Texture());

	glGenTextures(1, &texture->mTextureId);
	glBindTexture(GL_TEXTURE_2D, texture->mTextureId);

	texture->mTextureType = type;

	const auto dataSize = height * width * (type == GL_RGBA ? 4 : 3);

	std::vector<unsigned char> data(dataSize, 0x00);

	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, &data[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture->mHeight = height;
	texture->mWidth = width;

	return texture;
}