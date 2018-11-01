#pragma once
#include "Entity.h"
#include "TextureManager.h"

namespace GEX {
	class Projectile : public Entity
	{
	public:
		enum class Type
		{
			AlliedBullet,
			EnemyBullet,
			Missile
		};
	public:
						Projectile(Type type, const TextureManager& textures);
		
		unsigned int	getCategory() const override;
		//sf::FloatRect	getBoundingRect() const override;

		float			getMaxSpeed() const;
		int				getDamage() const;
		bool			isGuided() const;
		void			guidedTowards(sf::Vector2f position);
		sf::FloatRect	getBoundingBox() const override;
		
	private:
		void			updateCurrent(sf::Time dt, CommandQueue& commands) override;
		void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		Type			type_;
		sf::Sprite		sprite_;
		sf::Vector2f	targetDirection_;
	};

}