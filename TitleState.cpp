#include "TitleState.h"
#include "TextureManager.h"
#include "Ultility.h"
#include "FontManager.h"

TitleState::TitleState(GEX::StateStack & stack, Context context) :
	State(stack, context),
	backgroundSprite_(),
	text_(),
	showText_(true),
	textEffectTime_(sf::Time::Zero)
{
	backgroundSprite_.setTexture(context.textures->get(GEX::TextureID::TitleScreen));
	text_.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	text_.setString("Press Any key to Start");
	
	centerOrigin(text_);
	text_.setPosition(context.window->getView().getSize() / 2.f);

}

void TitleState::draw()
{
	auto& window = *getContext().window;			// auto& == sf::RenderWindow&
	window.draw(backgroundSprite_);

	if (showText_) {
		window.draw(text_);
	}
}

bool TitleState::update(sf::Time dt)
{
	textEffectTime_ += dt;
	if (textEffectTime_ >= sf::seconds(0.5)) {
		showText_ = !showText_;
		textEffectTime_ = sf::Time::Zero;
	}

	return true;
}

bool TitleState::handleEvent(const sf::Event & event)
{
	if (event.type == sf::Event::KeyPressed) {
		requestStackPop();
		requestStackPush(GEX::StateID::Game);
	}
	return true;
}
