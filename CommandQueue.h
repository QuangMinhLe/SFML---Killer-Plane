#pragma once
#include "Command.h"
#include <queue>

namespace GEX {

	class CommandQueue
	{
	public:
		void					push(const Command& command);
		Command					pop();
		bool					isEmpty() const;
	private:
		std::queue<Command>		queue_;
	};
	
}