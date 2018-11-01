#include "Projectile.h"
#include "DataTable.h"
#include "Ultility.h"
#include "Category.h"
#include "EmitterNode.h"

namespace GEX {

	namespace {
		const std::map<Projectile::Type, ProjectileData> TABLE = initializeProjectileData();
	}

	Projectile::Projectile(Type type, const TextureManager & textures):
		Entity(1),
		type_(type),
		sprite_((textures.get(TABLE.at(type).texture)), TABLE.at(type).textureRect)
	{
		centerOrigin(sprite_);
		if (isGuided()) {
			std::unique_ptr<EmitterNode> smoke(new EmitterNode(Particle::Type::Smoke));
			smoke->setPosition(0.f, 2.f + Projectile::getBoundingBox().height / 2.f);
			attachChild(std::move(smoke));

			std::unique_ptr<EmitterNode> propellant(new EmitterNode(Particle::Type::Propellant));
			propellant->setPosition(0.f, 2.f + Projectile::getBoundingBox().height / 2.f);
			attachChild(std::move(propellant));
		}
	}

	unsigned int Projectile::getCategory() const
	{
		if (type_ == Type::EnemyBullet)
			return Category::EnemyProjectile;
		else
			return Category::AlliedProjectile;
	}

	float Projectile::getMaxSpeed() const
	{
		return TABLE.at(type_).speed;
	}

	int Projectile::getDamage() const
	{
		return TABLE.at(type_).damage;
	}

	bool Projectile::isGuided() const
	{
		return (type_== Type::Missile);
	}

	void Projectile::guidedTowards(sf::Vector2f position)
	{
		assert(isGuided());
		targetDirection_ = unitVector(position - getWorldPosition());
	}

	sf::FloatRect Projectile::getBoundingBox() const
	{
		return getWorldTransform().transformRect(sprite_.getGlobalBounds());
	}

	void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
	{
		if (isGuided())
		{
			const float APPROACH_RATE = 400.f;
			auto newVelocity = unitVector(APPROACH_RATE * dt.asSeconds() * targetDirection_ + getVelocity());
			newVelocity *= getMaxSpeed();
			setVelocity(newVelocity);

			auto angle = std::atan2(newVelocity.y, newVelocity.x);
			setRotation(toDegree(angle) + 90.f);

		}
		Entity::updateCurrent(dt, commands);
	}
	void Projectile::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		target.draw(sprite_, states);
	}
}