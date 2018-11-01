#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"

namespace sf {
	class Sprite;
	class Text;
}

	void centerOrigin(sf::Sprite& sprite);

	void centerOrigin(sf::Text& text);

	void centerOrigin(GEX::Animation& animation);

	//degree/ radiance conversioon
	float		toDegree(float radian);
	float		toRadian(float degree);

	//random number generation
	int			randomInt(int exclusiveMax);
	float		length(sf::Vector2f vector);
	sf::Vector2f unitVector(sf::Vector2f vector);