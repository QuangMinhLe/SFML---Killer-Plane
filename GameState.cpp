#include "GameState.h"



GameState::GameState(GEX::StateStack & stack, Context context) : 
	State(stack, context),
	world_(*context.window),
	player_(*context.player)
{
}

void GameState::draw()
{
	world_.draw();
}

bool GameState::update(sf::Time dt)
{
	auto& commands = world_.getCommandQueue();		// == GEX::CommandQueue& commands = world.getCommandQueue();
	world_.update(dt, commands);
	if (!world_.hasAlivePlayer())
	{
		player_.setMissionStatus(GEX::MissionStatus::MissionFailure);
		requestStackPush(GEX::StateID::GameOver);
	}
	else if (world_.hasPlayerReachedEnd())
	{
		player_.setMissionStatus(GEX::MissionStatus::MissionAccomplished);
		requestStackPush(GEX::StateID::GameOver);
	}
	player_.handleRealtimeInput(commands);

	

	return true;
}

bool GameState::handleEvent(const sf::Event & event)
{
	GEX::CommandQueue& commands = world_.getCommandQueue();
	player_.handleEvent(event, commands);
	
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(GEX::StateID::Pause);
	return false;
}


