#include "TaskList.h"

void TaskList::refresh()
{
	for (auto task = mTaskList.begin(); task != mTaskList.end(); )
		if (task->first.check())
		{
			task->second(task->first);
			task = mTaskList.erase(task);
		}
		else
			++task;
}