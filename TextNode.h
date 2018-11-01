#pragma once
#include "SceneNode.h"
#include <SFML/Graphics/Text.hpp>

namespace GEX {
	class TextNode : public SceneNode
	{
	public:
		explicit			TextNode(const std::string& text);

		void				setText(const std::string& text, const sf::Color color = sf::Color::White);

	private:
		void				drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		sf::Text			text_;
	};

}