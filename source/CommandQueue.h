#pragma once

#include "Command.h"

class CommandQueue
{
public:
	void											push(const Command& command);
	Command											pop();
	bool											isEmpty() const;
	int												getQueueSize() const;

private:
	std::queue<Command>								mQueue;
};