#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SceneGame.h"

void SceneGame::draw()
{
	glClearColor(0.0, 1.0, 1.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	mTextureShader->useShader();
	mBackgroundTexture->bind();

	glBindVertexArray(mVao);
	glDrawArrays(GL_QUADS, 0, 4);
}

void SceneGame::refresh(double timePassed)
{

}

SceneGame::SceneGame() :mPerspectiveMat(glm::perspective(glm::radians(60.0f), 1.0f, 0.3f, 1000.0f))
{
	mTextureShader = Shader::loadShader("res/shader/Simple3DTexture.shader");
	mBackgroundTexture = Texture::loadTexture("res/images/InitSceneBackground.png");

	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mVbo);

	glBindVertexArray(mVao);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);

	float bufferData[4][5] =
	{
		{-1.0f,-1.0f,-6.0f,		0.0f,1.0f },
		{-1.0f, 1.0f,-6.0f,		0.0f,0.0f },
		{ 1.0f, 1.0f,-6.0f,		1.0f,0.0f },
		{ 1.0f,-1.0f,-6.0f,		1.0f,1.0f }
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(bufferData), bufferData, GL_STATIC_DRAW);

	//set perspective martix
	glUniformMatrix4fv(mTextureShader->getUniformLocation("perspective"), 1, GL_FALSE, glm::value_ptr(mPerspectiveMat));

	//set data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, nullptr);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(sizeof(float) * 3));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);
}

SceneGame::~SceneGame()
{
	//delete buffer object and vertex array object
	glDeleteVertexArrays(1, &mVao);
	glDeleteBuffers(1, &mVbo);
}
