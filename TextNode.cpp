#include "TextNode.h"
#include "Ultility.h"
#include "FontManager.h"
#include <SFML/Graphics/RenderTarget.hpp>

namespace GEX {
	TextNode::TextNode(const std::string & text)
	{
		text_.setFont(FontManager::getInstance().get(FontID::Main));
		text_.setCharacterSize(20);
		setText(text);
	}

	void TextNode::setText(const std::string & text, const sf::Color color)
	{
		text_.setString(text);
		text_.setFillColor(color);
		centerOrigin(text_);
	}

	void TextNode::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		target.draw(text_, states);
	}
}