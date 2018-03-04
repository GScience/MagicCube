#pragma once

#include <functional>
#include <future>

using TaskReturnObj = std::shared_ptr<void>;

class Task
{
	//what should the task do
	mutable std::future<TaskReturnObj> mFuture;

	//result
	mutable TaskReturnObj mTaskResult = nullptr;

public:
	explicit Task(const std::function<TaskReturnObj()>& func) :mFuture(std::async(func))
	{
		mFuture.wait_for(std::chrono::milliseconds(0));
	}

	//!wait for result
	void wait() const
	{
		mFuture.wait();
	}

	//!get result
	TaskReturnObj get() const
	{
		if (mFuture.valid() && check())
			mTaskResult = mFuture.get();

		return mTaskResult;
	}

	/*!check task
	 * @return return true if the result is valid
	 */
	bool check(const unsigned int waitFor = 0) const
	{
		return mFuture.wait_for(std::chrono::milliseconds(waitFor)) == std::future_status::ready;
	}
};
