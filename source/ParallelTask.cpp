#include "rnpch.h"
#include "ParallelTask.h"


ParallelTask::ParallelTask()
: thread_(&ParallelTask::runTask, this)
, finished_(false)
{
}

void ParallelTask::execute()
{
	finished_ = false;
	elapsedTime_.restart();
	thread_.launch();
}

bool ParallelTask::isFinished()
{
	sf::Lock lock(mutex_);
	return finished_;
}

float ParallelTask::getCompletion()
{
	sf::Lock lock(mutex_);

	// 100% at 10 seconds of elapsed time
	return elapsedTime_.getElapsedTime().asSeconds() / 10.f;
}

void ParallelTask::runTask()
{
	// Dummy task - stall 10 seconds
	bool ended = false;
	while (!ended)
	{
		sf::Lock lock(mutex_); // Protect the clock 
		if (elapsedTime_.getElapsedTime().asSeconds() >= 10.f)
			ended = true;
	}

	{ // finished_ may be accessed from multiple threads, protect it
		sf::Lock lock(mutex_);
		finished_ = true;
	}	
}