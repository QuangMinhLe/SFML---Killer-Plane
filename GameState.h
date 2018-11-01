#pragma once
#include "State.h"
#include "World.h"
#include "PlayerControl.h"


class GameState : public GEX::State
{
public:
	GameState(GEX::StateStack& stack, Context context);
	
	void						draw() override;
	bool						update(sf::Time dt) override;
	bool						handleEvent(const sf::Event& event) override;

private:
	GEX::World					world_;
	GEX::PlayerControl&			player_;
};

