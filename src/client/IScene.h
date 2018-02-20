#pragma once

#include <memory>

class IScene
{
	friend class Application;
protected:
	//!render
	virtual void draw() = 0;
	//!refresh
	virtual void refresh(double timePassed) = 0;

public:
	virtual ~IScene() = default;
};

using IScenePtr = std::shared_ptr<IScene>;