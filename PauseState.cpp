#include "PauseState.h"
#include "Ultility.h"
#include "FontManager.h"

PauseState::PauseState(GEX::StateStack & stack, Context context) :
	State(stack, context),
	backgroundSprite_(),
	pausedText_(),
	instructionText_()
{

	pausedText_.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	pausedText_.setString("Game Paused");
	pausedText_.setCharacterSize(80);
	centerOrigin(pausedText_);

	instructionText_.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	instructionText_.setString("Press backspace to return to the main menu");
	centerOrigin(instructionText_);
	
	sf::Vector2f viewSize = context.window->getView().getSize();
	pausedText_.setPosition(0.5f * viewSize.x, 0.4f * viewSize.y);
	instructionText_.setPosition(0.5f * viewSize.x, 0.6f * viewSize.y);
}

void PauseState::draw()
{
	auto& window = *getContext().window;			// auto& == sf::RenderWindow&
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(pausedText_);
	window.draw(instructionText_);
}

bool PauseState::update(sf::Time dt)
{
	return false;
}

bool PauseState::handleEvent(const sf::Event & event)
{
	if (event.type != sf::Event::KeyPressed)
		return false;

	if (event.key.code == sf::Keyboard::Escape)
	{
		requestStackPop();
	}

	if (event.key.code == sf::Keyboard::BackSpace)
	{
		requestStackClear();
		requestStackPush(GEX::StateID::Menu);
	}

	return false;
}
