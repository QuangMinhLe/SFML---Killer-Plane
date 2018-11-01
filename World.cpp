#include "World.h"
#include <functional>
#include <algorithm>
#include "Pickup.h"
#include "ParticleNode.h"

namespace GEX {

	World::World(sf::RenderWindow& window):
	window_(window),
	worldView_(window.getDefaultView()),
	textures_(),
	sceneGraph_(),
	sceneLayers_(),
	worldBounds_(0.f, 0.f, worldView_.getSize().x, 5000.f),
	spawnPosition_(worldView_.getSize().x/2.f, worldBounds_.height - worldView_.getSize().y/2.f),
	scrollSpeeds_(-50.f),
	playerAircraft_(nullptr), 
	counter_(1), 
	orientation_(1),
	commandQueue_()
	{
		loadTextures();

		//prepare view
		worldView_.setCenter(spawnPosition_);

		buildScene();
	}

	void World::update(sf::Time dt, CommandQueue& commands)
	{
		//scroll the world
		worldView_.move(0.f, scrollSpeeds_ * dt.asSeconds());

		playerAircraft_->setVelocity(0.f, 0.f);
		
		destroyEntitiesOutOfViews();
		guideMissile();

		while (!commandQueue_.isEmpty())
		{
			sceneGraph_.onCommand(commandQueue_.pop(), dt);
		}
		handleCollision();
		sceneGraph_.removeWrecks();
		adaptPlayerVelocity();

		sceneGraph_.update(dt, commands);

		adaptPlayerPosition();

		spawnEnemies();
		
		//zoom a little
		//worldView_.zoom(0.0000000000000000000000005);

		//rotate a little
		//worldView_.rotate(30);
		//move player
		/*sf::Vector2f pos = playerAircraft_->getPosition();

		float bounds = worldView_.getSize().x;

		if (pos.x < 120.f || pos.x > bounds - 120.f)
		{
			playerAircraft_->setVelocity(sf::Vector2f(playerAircraft_->getVelocity().x * -1 , playerAircraft_->getVelocity().y));
		}*/


	}
	void World::adaptPlayerVelocity()
	{
		sf::Vector2f velocity = playerAircraft_->getVelocity();
		if (velocity.x != 0.f && velocity.y != 0.f) {
			playerAircraft_->setVelocity(velocity / std::sqrt(2.f));
		}
		playerAircraft_->accelerate(0.f, scrollSpeeds_);
	}

	void World::addEnemies()
	{
		addEnemy(AircraftType::Raptor, -250.f, 200.f);
		addEnemy(AircraftType::Raptor, 0.f, 200.f);
		addEnemy(AircraftType::Raptor, 250.f, 200.f);

		addEnemy(AircraftType::Raptor, -250.f, 600.f);
		addEnemy(AircraftType::Raptor, 0.f, 600.f);

		addEnemy(AircraftType::Raptor, -700.f, 800.f);
		addEnemy(AircraftType::Raptor, 70.f, 800.f);

		addEnemy(AircraftType::Avenger, -170.f, 850.f);
		addEnemy(AircraftType::Avenger, 170.f, 850.f);
							   
		addEnemy(AircraftType::Avenger, -150.f, 900.f);
		addEnemy(AircraftType::Avenger, -150.f, 900.f);

		std::sort(enemySpawnPoints_.begin(), enemySpawnPoints_.end(),
			[](SpawnPoint lhs, SpawnPoint rhs)
			{
			return lhs.y < rhs.y;
			}
		);

	}

	void World::addEnemy(AircraftType type, float relX, float relY)
	{
		SpawnPoint spawnPoint(type, spawnPosition_.x - relX, spawnPosition_.y - relY);
		enemySpawnPoints_.push_back(spawnPoint);
	}

	void World::spawnEnemies()
	{
		while (!enemySpawnPoints_.empty() && enemySpawnPoints_.back().y > getBattlefieldBounds().top) {
			auto spawnPoint = enemySpawnPoints_.back();
			std::unique_ptr<AirCraft> enemy(new AirCraft(spawnPoint.type, textures_));
			enemy->setPosition(spawnPoint.x, spawnPoint.y);
			enemy->rotate(180);
			sceneLayers_[UpperAir]->attachChild(std::move(enemy));
			enemySpawnPoints_.pop_back();
		}
	}

	sf::FloatRect World::getViewBounds() const
	{
		return sf::FloatRect(worldView_.getCenter() - (worldView_.getSize() / 2.f), worldView_.getSize());	// center minus half size
	}

	sf::FloatRect World::getBattlefieldBounds() const
	{
		sf::FloatRect bounds = getViewBounds();
		bounds.top -= 100.f;						// to up
		bounds.height += 100.f;						// to bottom

		return bounds;
	}

	void World::guideMissile()
	{
		//build a list of active Enemies

		Command enemyCollector;
		enemyCollector.category = Category::EnemyAircraft;
		enemyCollector.action = derivedAction<AirCraft>([this](AirCraft& enemy, sf::Time dt)
		{
			if (!enemy.isDestroyed()) {
				activeEnemies_.push_back(&enemy);
			}

		});

		Command missileGuider;
		missileGuider.category = Category::Type::AlliedProjectile;
		missileGuider.action = derivedAction<Projectile>([this](Projectile& missile, sf::Time dt) 
		{
			//ignore bullets
			if (!missile.isGuided()) {
				return;
			}

			float minDistance = std::numeric_limits<float>::max();
			AirCraft* closestEnemy = nullptr;

			for (AirCraft* e : activeEnemies_) {
				auto d = distance(missile, *e);
				if (d < minDistance) {
					minDistance = d;
					closestEnemy = e;
				}
			}
			if (closestEnemy)
				missile.guidedTowards(closestEnemy->getWorldPosition());
		});
		commandQueue_.push(enemyCollector);
		commandQueue_.push(missileGuider);
		activeEnemies_.clear();
	}

	bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
	{
		unsigned int category1 = colliders.first->getCategory();
		unsigned int category2 = colliders.second->getCategory();

		//compare first pair entry has category type1 and second has type2
		if (type1 & category1 && type2 & category2)
		{
			return true;
		}
		else if (type1 & category2 && type2 & category1) {
			std::swap(colliders.first, colliders.second);
			return true;
		}
		else {
			return false;
		}
	}

	void World::handleCollision()
	{
		//build a list of colliding pairs of SceneNodes
		std::set<SceneNode::Pair> collisionPairs;
		sceneGraph_.checkSceneCollision(sceneGraph_, collisionPairs);

		for (SceneNode::Pair pair : collisionPairs) {
			if (matchesCategories(pair, Category::Type::PlayerAircraft, Category::Type::EnemyAircraft) )
			{
				auto& player = static_cast<AirCraft&>(*(pair.first));
				auto& enemy = static_cast<AirCraft&>(*(pair.second));
				
				player.damage(enemy.getHitpoints());
				enemy.destroy();

			}
			else if (matchesCategories(pair, Category::Type::PlayerAircraft, Category::Type::Pickup)) {
				auto& player = static_cast<AirCraft&>(*(pair.first));
				auto& pickup = static_cast<Pickup&>(*(pair.second));

				pickup.apply(player);
				pickup.destroy();

			}
			else if (matchesCategories(pair, Category::Type::PlayerAircraft, Category::Type::EnemyProjectile) || matchesCategories(pair, Category::Type::EnemyAircraft, Category::Type::AlliedProjectile)) {
				auto& aircraft = static_cast<AirCraft&>(*(pair.first));
				auto& projectile = static_cast<Projectile&>(*(pair.second));

				aircraft.damage(projectile.getDamage());
				projectile.destroy();
			}
		}
	}

	void World::destroyEntitiesOutOfViews()
	{
		Command command;
		command.category = Category::Type::Projectile | Category::Type::EnemyAircraft | Category::Type::Pickup;
		command.action = derivedAction<Entity>([this](Entity& e, sf::Time dt)
		{
			if (!getBattlefieldBounds().intersects(e.getBoundingBox()))
				e.remove();
		});

		commandQueue_.push(command);
	}

	void World::adaptPlayerPosition()
	{
		const float BORDER_DISTANCE = 40.f;
		sf::FloatRect viewBounds(worldView_.getCenter() - worldView_.getSize() / 2.f, worldView_.getSize());

		sf::Vector2f position = playerAircraft_->getPosition();
		position.x = std::max(position.x, viewBounds.left + BORDER_DISTANCE);
		position.x = std::min(position.x, viewBounds.left + viewBounds.width - BORDER_DISTANCE);

		position.y = std::max(position.y, viewBounds.top + BORDER_DISTANCE);
		position.y = std::min(position.y, viewBounds.top + viewBounds.height - BORDER_DISTANCE);

		playerAircraft_->setPosition(position);
	}

	void World::draw()
	{
		window_.setView(worldView_);
		window_.draw(sceneGraph_);

	}

	CommandQueue & World::getCommandQueue()
	{
		return commandQueue_;
	}

	bool World::hasAlivePlayer() const
	{
		return !playerAircraft_->isDestroyed();
	}

	bool World::hasPlayerReachedEnd() const
	{
		return !worldBounds_.contains(playerAircraft_->getPosition());
	}

	void World::loadTextures()
	{
		textures_.load(GEX::TextureID::Entities, "Media/Textures/Entities.png");
		//map type
		textures_.load(GEX::TextureID::Jungle, "Media/Textures/JungleBig.png");
		textures_.load(GEX::TextureID::Particle, "Media/Textures/Particle.png");
		textures_.load(GEX::TextureID::Explosion, "Media/Textures/Explosion.png");
		textures_.load(GEX::TextureID::FinishLine, "Media/Textures/FinishLine.png");
	}

	void World::buildScene()
	{
		for (int i = 0; i < LayerCount; ++i) {
			auto category = (i == UpperAir) ? Category::Type::AirSceneLayer : Category::Type::None;
			SceneNode::Ptr layer(new SceneNode(category));
			sceneLayers_.push_back(layer.get());
			sceneGraph_.attachChild(std::move(layer));
		}
		
		//particle system
		std::unique_ptr<ParticleNode>smoke(new ParticleNode(Particle::Type::Smoke, textures_));
		sceneLayers_[LowerAir]->attachChild(std::move(smoke));


		std::unique_ptr<ParticleNode>fire(new ParticleNode(Particle::Type::Propellant, textures_));
		sceneLayers_[LowerAir]->attachChild(std::move(fire));

		//background
		sf::Texture& texture = textures_.get(TextureID::Jungle);
		sf::IntRect textureRect(worldBounds_);
		texture.setRepeated(true);

		std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
		backgroundSprite->setPosition(worldBounds_.left, worldBounds_.top);
		sceneLayers_[Background]->attachChild(std::move(backgroundSprite));

		//add player aircraft & gam object
		std::unique_ptr<AirCraft> leader(new AirCraft(AircraftType::Eagle, textures_));
		leader->setPosition(spawnPosition_);
		leader->setVelocity(500.f, scrollSpeeds_);

		playerAircraft_ = leader.get();
		sceneLayers_[UpperAir]->attachChild(std::move(leader));

		addEnemies();
		//add escort planes
		/*std::unique_ptr<AirCraft> leftEscort(new AirCraft(AircraftType::Avenger, textures_));
		leftEscort->setPosition(-80.f, 50.f);
		playerAircraft_->attachChild(std::move(leftEscort));

		std::unique_ptr<AirCraft> rightEscort(new AirCraft(AircraftType::Raptor, textures_));
		rightEscort->setPosition(80.f, 50.f);
		playerAircraft_->attachChild(std::move(rightEscort));
*/
		//add enemy airvarft
		/*std::unique_ptr<AirCraft> enemy(new AirCraft(AircraftType::Avenger, textures_));
		enemy->setPosition(spawnPosition_.x - 200, spawnPosition_.y - 600);
		enemy->setVelocity(0.f, -scrollSpeeds_);
		enemy->setRotation(180);
		sceneLayers_[Air]->attachChild(std::move(enemy));

		enemy = std::unique_ptr<AirCraft>(new AirCraft(AircraftType::Raptor, textures_));
		enemy->setPosition(spawnPosition_.x + 400, spawnPosition_.y - 600);
		enemy->setVelocity(50.f, -scrollSpeeds_);
		enemy->setRotation(180);
		sceneLayers_[Air]->attachChild(std::move(enemy));*/

	}
}