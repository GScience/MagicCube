#pragma once

#include "Task.h"
#include <list>

class TaskList
{
	using TaskWithCallback = std::pair<Task, std::function<void(const Task&)>>;

	std::list<TaskWithCallback> mTaskList;

public:
	void addTask(Task& task, const std::function<void(const Task&)>& onFinish)
	{
		mTaskList.emplace_back(std::make_pair(std::forward<Task>(task), onFinish));
	}

	void refresh();
};