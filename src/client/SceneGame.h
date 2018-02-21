#pragma once

#include "IScene.h"
#include "Resource.h"
#include <glm/glm.hpp>
#include <GL/glew.h>

class SceneGame : public IScene
{
	//!perspective matrix
	const glm::mat4 mPerspectiveMat;

	//!shader
	ShaderPtr mTextureShader;
	//!texture
	TexturePtr mBackgroundTexture;

	//!vbo
	GLuint mVbo = 0;
	//!vao
	GLuint mVao = 0;

protected:
	//!render
	void draw() override;
	//!refresh
	void refresh(double timePassed) override;

public:
	SceneGame();
	~SceneGame();
};
