#pragma once

#include "Task.h"

class TaskQueue
{
public:
	void addTask(Task task, std::function<void()> onFinish);
};