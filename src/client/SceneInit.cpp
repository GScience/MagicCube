#include "SceneInit.h"

SceneInit::SceneInit(std::function<void()> func)
{
	mTextureShader = Shader::loadShader("res/shader/Simple2DTexture.shader");
	mBackgroundTexture = Texture::loadTexture("res/images/InitSceneBackground.png");

	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mVbo);

	glBindVertexArray(mVao);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);

	float bufferData[4][4] =
	{
		{-1.0f,-1.0f,	0.0f,1.0f},
		{-1.0f, 1.0f,	0.0f,0.0f},
		{ 1.0f, 1.0f,	1.0f,0.0f},
		{ 1.0f,-1.0f,	1.0f,1.0f}
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(bufferData), bufferData, GL_STATIC_DRAW);

	mTextureShader->useShader();

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, reinterpret_cast<void*>(sizeof(float) * 2));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//draw the scene
	SceneInit::draw();

	//run function
	func();
}

SceneInit::~SceneInit()
{
	//delete buffer object and vertex array object
	glDeleteVertexArrays(1, &mVao);
	glDeleteBuffers(1, &mVbo);
}

void SceneInit::draw()
{
	glClearColor(0.0, 1.0, 1.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	mTextureShader->useShader();
	mBackgroundTexture->bind();

	glBindVertexArray(mVao);
	glDrawArrays(GL_QUADS, 0, 4);
}