#include "AirCraft.h"
#include "Category.h"
#include "DataTable.h"
#include "Ultility.h"
#include <string>

#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

namespace GEX {

	namespace {
		const std::map<GEX::AircraftType, AircraftData> TABLE = initializeAircraftData();
	}

	AirCraft::AirCraft(AircraftType type, const TextureManager & textures) :
		Entity(TABLE.at(type).hitpoints),
		type_(type),
		sprite_(textures.get(TABLE.at(type).texture), TABLE.at(type).textureRect),
		healthDisplay_(nullptr),
		missileDiplay_(nullptr),
		travelDistance_(0.f),
		directionIndex_(0),
		isFiring_(false),
		fireRateLevel_(1),
		fireSpreadLevel_(1),
		fireCountdown_(sf::Time::Zero),
		fireCommand_(),
		missileAmmo_(30),
		isLaunchingMissile_(false),
		isMarkedForRemoval_(false),
		explosion_(textures.get(TextureID::Explosion)),
		showExplosion_(true),
		spawnPickup_(false)
	{
		//set up explosion
		explosion_.setFrameSize(sf::Vector2i(256, 256));
		explosion_.setNumFrames(16);
		explosion_.setDuration(sf::seconds(1));
		centerOrigin(explosion_);

		//setup command
		fireCommand_.category = Category::AirSceneLayer;
		fireCommand_.action = [this, &textures](SceneNode& node, sf::Time dt)
		{
			createBullets(node, textures);
		};


		launchMissileCommand_.category = Category::AirSceneLayer;
		launchMissileCommand_.action = [this, &textures](SceneNode& node, sf::Time dt)
		{
			createProjectile(node, Projectile::Type::Missile, 0.f, 0.5f, textures);
		};
		
		dropPickupCommand_.category = Category::AirSceneLayer;
		dropPickupCommand_.action = [this, &textures](SceneNode& node, sf::Time dt)
		{
			createPickup(node, textures);
		};
		/*
		sf::FloatRect bounds = sprite_.getLocalBounds();
		sprite_.setOrigin(bounds.width / 2.f, bounds.height / 2.f);*/


		centerOrigin(sprite_);
		std::unique_ptr<TextNode> health(new TextNode(""));
		healthDisplay_ = health.get();
		attachChild(std::move(health));

		std::unique_ptr<TextNode> missileammo(new TextNode(""));
		missileDiplay_ = missileammo.get();
		attachChild(std::move(missileammo));
	}

	void AirCraft::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		if (isDestroyed() && showExplosion_)
		{
			target.draw(explosion_, states);
		}
		else
		{
			target.draw(sprite_, states);
		}
		
	}
	unsigned int AirCraft::getCategory() const
	{
		switch (type_)
		{
		case AircraftType::Eagle:
			return Category::PlayerAircraft;
			break;

		case AircraftType::Raptor:
			return Category::EnemyAircraft;
			break;

		case AircraftType::Avenger:
			return Category::EnemyAircraft;
			break;

		default:
			return Category::None;
			break;

		}
	}
	void AirCraft::updateTexts()
	{
		sf::Color color = sf::Color::White;
		if (getHitpoints() <= 20)
		{
			color = sf::Color::Red;
		}
		healthDisplay_->setText(std::to_string(getHitpoints()) + "HP", color);
		healthDisplay_->setPosition(0.f, 50.f);
		healthDisplay_->setRotation(-getRotation());
		if (isAllied())
		{
			sf::Color color = sf::Color::Green;
			if (missileAmmo_ <= 3) {
				color = sf::Color::Red;
			}
			missileDiplay_->setText("Missile: " + std::to_string(missileAmmo_), color);


			missileDiplay_->setPosition(0.f, 70.f);


		}
	}
	void AirCraft::fire()
	{
		if(TABLE.at(type_).fireInterval != sf::Time::Zero)
			isFiring_ = true;
	}
	void AirCraft::launchMissile()
	{
		isLaunchingMissile_ = true;
	}
	void AirCraft::increaseFireRate()
	{
		if (fireRateLevel_ < 10)
			fireRateLevel_++;
	}
	void AirCraft::increaseFireSpread()
	{
		if (fireSpreadLevel_ < 3) {
			fireSpreadLevel_++;
		}
	}
	void AirCraft::collectMissile(unsigned int count)
	{
		missileAmmo_ += count;
	}
	sf::FloatRect AirCraft::getBoundingBox() const
	{
		return getWorldTransform().transformRect(sprite_.getGlobalBounds());
	}
	bool AirCraft::isAllied() const
	{
		return (type_ == AircraftType::Eagle);
	}
	bool AirCraft::isMarkedForRemoval() const
	{
		return (isDestroyed() && (explosion_.isFinished() || !showExplosion_));
	}
	void AirCraft::remove()
	{
		Entity::remove();

		showExplosion_ = false;
	}
	void AirCraft::updateLateralRoll()
	{
		if (TABLE.at(type_).hasRollAnimation)
		{
			auto textureRect = TABLE.at(type_).textureRect;
			if (getVelocity().x < 0.f) // Turn left 
			{
				textureRect.left = textureRect.width;
			}
			else if (getVelocity().x > 0.f) // Turn right 
			{
				textureRect.left = 2 * textureRect.width;
			}

			sprite_.setTextureRect(textureRect);
		}
	}
	void AirCraft::updateCurrent(sf::Time dt, CommandQueue& commands)
	{
		updateMovementPattern(dt);

		if (isDestroyed()) {
			checkPickupDrop(commands); 
			explosion_.update(dt);
			return;
		}
		updateMovementPattern(dt);
		Entity::updateCurrent(dt, commands);
		updateTexts();
		checkProjectileLaunch(dt, commands);
		updateLateralRoll();
	}
	void AirCraft::updateMovementPattern(sf::Time dt)
	{
		//movement pattern
		const std::vector<Direction>& directions = TABLE.at(type_).directions;

		if (!directions.empty()) {
			if (travelDistance_ > (directions[directionIndex_].distance)) {
				directionIndex_ = (directionIndex_+1) % directions.size();
				travelDistance_ = 0;
			}

			float radians = toRadian(directions[directionIndex_].angle + 90.f);
			float vx = getMaxSpeed() * std::cos(radians);
			float vy = getMaxSpeed() * std::sin(radians);

			setVelocity(vx, vy);
		}
	}
	float AirCraft::getMaxSpeed() const
	{
		return TABLE.at(type_).speed;
	}

	void AirCraft::createBullets(SceneNode & node, const TextureManager & textures)
	{
		Projectile::Type type = isAllied() ? Projectile::Type::AlliedBullet : Projectile::Type::EnemyBullet;

		switch (fireSpreadLevel_)
		{
		case 1:
			createProjectile(node, type, 0.f, 0.5f, textures);
			break;

		case 2:
			createProjectile(node, type, -0.33f, 0.5f, textures);
			createProjectile(node, type, +0.33f, 0.5f, textures);
			break;

		case 3:
			createProjectile(node, type, -0.5f, 0.5f, textures);
			createProjectile(node, type, 0.f, 0.5f, textures);
			createProjectile(node, type, +0.5f, 0.5f, textures);
			break;

		}
	}

	void AirCraft::createProjectile(SceneNode & node, Projectile::Type type, float xOffset, float yOffset, const TextureManager & textures)
	{
		std::unique_ptr<Projectile>projectile (new Projectile(type, textures));

		sf::Vector2f offset(xOffset * sprite_.getGlobalBounds().width, yOffset * sprite_.getGlobalBounds().height);
		sf::Vector2f velocity(0, projectile->getMaxSpeed());
		float sign = isAllied() ? -1.f : 1.f;

		projectile->setPosition(getWorldPosition() + offset * sign);
		projectile->setVelocity(velocity * sign);
		
		node.attachChild(std::move(projectile));
	}
	void AirCraft::createPickup(SceneNode & node, const TextureManager & textures) const
	{
		auto type = static_cast<Pickup::Type>(randomInt(static_cast<int>(Pickup::Type::Count)));
		
		std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
		pickup->setPosition(getWorldPosition());
		pickup->setVelocity(0.f, 0.f);
		node.attachChild(std::move(pickup));
	}
	void AirCraft::checkPickupDrop(CommandQueue & commands)
	{
		if (!isAllied() && randomInt(2) == 0 && !spawnPickup_)
			commands.push(dropPickupCommand_);
		
		spawnPickup_ = true;
	}
	void AirCraft::checkProjectileLaunch(sf::Time dt, CommandQueue & commands)
	{
		//Enemies always fire
		if (!isAllied()) { 
			fire(); 
		}

		if (isFiring_ && fireCountdown_ <= sf::Time::Zero) {
			commands.push(fireCommand_);
			isFiring_ = false;
			fireCountdown_ = TABLE.at(type_).fireInterval / (fireRateLevel_ + 1.f);
			int a = 1;
		}
		else if (fireCountdown_ > sf::Time::Zero) {
			fireCountdown_ -= dt;
		}

		//missile
		if (isLaunchingMissile_)
		{
			if (missileAmmo_ > 0) {

				commands.push(launchMissileCommand_);
				isLaunchingMissile_ = false;
				--missileAmmo_;

			}
		}
	}
}