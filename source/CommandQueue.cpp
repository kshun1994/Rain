#include "rnpch.h"
#include "CommandQueue.h"
#include "SceneNode.h"


void CommandQueue::push(const Command& command)
{
	queue_.push(command);
}

Command CommandQueue::pop()
{
	Command command = queue_.front();
	queue_.pop();
	return command;
}

bool CommandQueue::isEmpty() const
{
	return queue_.empty();
}
int CommandQueue::getQueueSize() const
{
	return queue_.size();
}