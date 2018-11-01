#include "GameOverState.h"
#include "FontManager.h"
#include "Ultility.h"
#include "GameState.h"

namespace GEX {

	GameOverState::GameOverState(StateStack & stack, Context context) :
		State(stack, context),
		gameOverText_(),
		elapsedTime_(sf::Time::Zero)
	{
		sf::Font& font = FontManager::getInstance().get(GEX::FontID::Main);
		sf::Vector2f windowSize(context.window->getSize());

		gameOverText_.setFont(font);
		if (context.player->getMissionStatus() == MissionStatus::MissionFailure) {
			gameOverText_.setString("Mission Failure");
		}
		else {
			gameOverText_.setString("Mission Accomplished");
		}

		gameOverText_.setCharacterSize(70);
		centerOrigin(gameOverText_);
		gameOverText_.setPosition(0.5f * windowSize.x, 0.54f * windowSize.y);
		
		
	}
	void GameOverState::draw()
	{
		auto& window = *getContext().window;			// auto& == sf::RenderWindow&
		window.setView(window.getDefaultView());

		sf::RectangleShape backgroundShape;
		backgroundShape.setFillColor(sf::Color(0, 0, 0, 50));
		backgroundShape.setSize(window.getView().getSize());

		window.draw(backgroundShape);
		window.draw(gameOverText_);
	}

	bool GameOverState::update(sf::Time dt)
	{
		elapsedTime_ += dt;
		if (elapsedTime_ > sf::seconds(3))
		{
			requestStackClear();
			requestStackPush(StateID::Menu);

		}
		return false;
	}

	bool GameOverState::handleEvent(const sf::Event & event)
	{
		return false;
	}
}