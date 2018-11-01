#pragma once
#include <SFML/Graphics/Texture.hpp> 
#include <SFML/Graphics/Sprite.hpp> 
#include <SFML/System/Time.hpp> 
#include <SFML/Graphics/RenderTarget.hpp>

namespace GEX {
	class Animation : public sf::Drawable, public sf::Transformable
	{
	public:
		Animation();
		Animation(const sf::Texture& texture);
		//Getters and setters 
		void				setTexture(const sf::Texture& texture);
		const sf::Texture*	getTexture() const;
		void				setFrameSize(sf::Vector2i framesize);
		sf::Vector2i		getFrameSize() const;
		void				setNumFrames(std::size_t numFrames);
		std::size_t			getNumFrames() const;
		void				setDuration(sf::Time time);
		sf::Time			getDuration() const;
		void				setRepeating(bool repeat);
		bool				isRepeating() const;

		//Methods 
		void				restart();
		bool				isFinished() const;
		sf::FloatRect		getLocalBounds() const;
		sf::FloatRect		getGlobalBounds() const;
		void				update(sf::Time dt);

	private:
		void				draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		sf::Sprite			sprite_;
		sf::Vector2i		frameSize_;
		std::size_t			numberOfFrames_;
		std::size_t			currentFrame_;
		sf::Time			duration_;
		sf::Time			elapsedTime_;
		bool				repeat_;
	};

}