#pragma once
#include "Entity.h"
#include "TextureManager.h"
#include "AirCraft.h"

namespace GEX {
	class Pickup : public Entity
	{
	public:
		enum class Type {
			HealthRefill,
			MissileRefill,
			FireSpread,
			FireRate,
			Count
		};

	public:
								Pickup(Type type, const TextureManager& textures);
								~Pickup() = default;

		unsigned int			getCategory() const override;
		sf::FloatRect			getBoundingBox() const override;
		void					apply(AirCraft& player);
	private:
		void					drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		Type					type_;
		sf::Sprite				sprite_;
	};

}