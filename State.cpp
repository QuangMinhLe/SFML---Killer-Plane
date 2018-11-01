#include "State.h"
#include "StateStack.h"

namespace GEX {

	State::Context::Context(
		sf::RenderWindow & window,
		TextureManager & textures,
		PlayerControl & player)

		: window(&window),
		textures(&textures),
		player(&player)
	{
	}

	State::State(StateStack & stack, Context context): stack_(&stack), context_(context)
	{
	}

	State::~State()
	{
	}

	void State::requestStackPush(StateID stateID)
	{
		stack_->pushState(stateID);
	}

	void State::requestStackPop()
	{
		stack_->popState();
	}

	void State::requestStackClear()
	{
		stack_->clearStates();
	}

	State::Context State::getContext() const
	{
		return context_;
	}

}