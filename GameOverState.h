#pragma once

#include "State.h"

namespace GEX {

	class GameOverState : public State
	{
	public:
		GameOverState(StateStack& stack, Context context);


		void						draw() override;
		bool						update(sf::Time dt) override;
		bool						handleEvent(const sf::Event& event) override;

	private:
		sf::Text					gameOverText_;
		sf::Time					elapsedTime_;
	};

}