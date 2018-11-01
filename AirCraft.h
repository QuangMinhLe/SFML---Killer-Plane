#pragma once

#include "Entity.h"
#include "TextureManager.h"
#include "TextNode.h"
#include "Projectile.h"
#include "Command.h"
#include "Category.h"
#include <SFML\Graphics\Sprite.hpp>
#include "Animation.h"

namespace GEX {
	
	class TextNode;

	enum class AircraftType {Eagle, Raptor, Avenger};

	class AirCraft : public Entity
	{
	public:
								AirCraft(AircraftType type, const TextureManager& textures);

		virtual void			 drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
		unsigned int			 getCategory() const override;
	
		void					updateTexts(); //update health for aircrafts
		
		void					fire();
		void					launchMissile();

		void					increaseFireRate();
		void					increaseFireSpread();
		void					collectMissile(unsigned int count);

		sf::FloatRect			getBoundingBox()const override;
		bool					isAllied() const;

		bool					isMarkedForRemoval() const override;

		void					remove() override;
		void					updateLateralRoll(); //check and update the sprite if the airplane is moving on x axis 


	protected:
		void					updateCurrent(sf::Time dt, CommandQueue& commands) override;

	private:
		void					updateMovementPattern(sf::Time dt);
		float					getMaxSpeed() const;

		void					checkPickupDrop(CommandQueue& commands);
		void					checkProjectileLaunch(sf::Time dt, CommandQueue& commands);

		void					createBullets(SceneNode& node, const TextureManager& textures);
		void					createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureManager& textures);
		void					createPickup(SceneNode& node, const TextureManager& textures) const;

	private:
		AircraftType			type_;
		sf::Sprite				sprite_;
		Animation				explosion_;
		bool					showExplosion_;

		TextNode*				healthDisplay_;
		TextNode*				missileDiplay_;
		
		float					travelDistance_;
		std::size_t				directionIndex_;

		bool					isFiring_;
		bool					isLaunchingMissile_;
		bool					isMarkedForRemoval_;

		int						fireRateLevel_;
		int						fireSpreadLevel_;
		int						missileAmmo_;

		bool					spawnPickup_;

		sf::Time				fireCountdown_;

		Command					fireCommand_;
		Command					launchMissileCommand_;
		Command					dropPickupCommand_;
	};

	
}
