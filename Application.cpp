#include "Application.h"
#include "State.h"
#include "TitleState.h"
#include "MenuState.h"
#include "PauseState.h"
#include "GameOverState.h"
#include "GameState.h"
#include "FontManager.h"
#include <iostream>


const sf::Time Application::TimePerFrame = sf::seconds(1.0f / 60.0f);

Application::Application() :
	window_(sf::VideoMode(1280, 960),"Killer PLane", sf::Style::Close),
	player_(),
	textures_(),
	stateStack_(GEX::State::Context(window_, textures_, player_)),
	statisticsText_(),
	statisticsUpdateTime_(),
	statisticsNumFrames_(0)
{
	window_.setKeyRepeatEnabled(false);
	GEX::FontManager::getInstance().load(GEX::FontID::Main, "Media/Sansation.ttf");
	//if (!font_.loadFromFile("Media/Sansation.ttf")) {
	//	//error handling
	//}

	textures_.load(GEX::TextureID::TitleScreen, "Media/Textures/TitleScreenBig.png");

	statisticsText_.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	statisticsText_.setPosition(5.0f, 5.0f);
	statisticsText_.setCharacterSize(15.f);
	statisticsText_.setString("Frames / Second = \nTime / Update = ");

	registerStates();
	stateStack_.pushState(GEX::StateID::Title);
}

void Application::run()
{

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (window_.isOpen()) {

		timeSinceLastUpdate += clock.restart();

		while (timeSinceLastUpdate > TimePerFrame) {
			processInput();
			update(TimePerFrame);

			if (stateStack_.isEmpty())
				window_.close();

			timeSinceLastUpdate -= TimePerFrame;
		}
		updateStatistics(timeSinceLastUpdate);

		render();
	}
}

void Application::processInput()
{
	sf::Event event;
	while (window_.pollEvent(event))
	{
		stateStack_.handleEvent(event);

		if (event.type == sf::Event::Closed)
			window_.close();
	}
}

void Application::update(sf::Time deltaTime)
{
	stateStack_.update(deltaTime);
}

void Application::render()
{
	window_.clear();
	stateStack_.draw();

	window_.setView(window_.getDefaultView());
	window_.draw(statisticsText_);
	window_.display();
}

void Application::updateStatistics(sf::Time deltaTime)
{
	statisticsUpdateTime_ += deltaTime;
	statisticsNumFrames_ += 1;

	if (statisticsUpdateTime_ > sf::seconds(1)) {
		statisticsText_.setString(
			"Frames / Second = " + std::to_string(statisticsNumFrames_) + "\n" +
			"Time / Update = " + std::to_string(statisticsUpdateTime_.asMicroseconds() / statisticsNumFrames_) + "ms"
		);

		statisticsUpdateTime_ -= sf::seconds(1);
		statisticsNumFrames_ = 0;
	}

}

void Application::registerStates()
{
	stateStack_.registerState<TitleState>(GEX::StateID::Title);
	stateStack_.registerState<MenuState>(GEX::StateID::Menu);
	stateStack_.registerState<GameState>(GEX::StateID::Game);
	stateStack_.registerState<PauseState>(GEX::StateID::Pause);
	stateStack_.registerState<GEX::GameOverState>(GEX::StateID::GameOver);
}
