#pragma once

#include <functional>
#include "Resource.h"
#include "IScene.h"

/*!Init scene
 * All init should be finished in this scene by passing the init function
 */
class SceneInit :public IScene
{
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
	void refresh(double timePassed) override {};

public:
	SceneInit(std::function<void()> func);
	~SceneInit();
};