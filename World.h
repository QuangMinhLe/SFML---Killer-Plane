//
//the game World
//

#pragma once
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "SceneNode.h"
#include "SpriteNode.h"
#include "TextureManager.h"
#include "AirCraft.h"
#include "CommandQueue.h"
#include "Entity.h"

namespace sf {
	class RenderWindow;
}

namespace GEX {

	class World
	{
	public:
		explicit				World(sf::RenderWindow& window);
		void					update(sf::Time dt, CommandQueue& commands);
		void					draw();
		
		CommandQueue&			getCommandQueue();
		bool					hasAlivePlayer() const;
		bool					hasPlayerReachedEnd() const;

	private:
		void					loadTextures();
		void					buildScene();
		void					adaptPlayerPosition();
		void					adaptPlayerVelocity();
		
		void					addEnemies();
		void					addEnemy(AircraftType type, float relX, float relY);
		void					spawnEnemies();

		sf::FloatRect			getViewBounds() const;
		sf::FloatRect			getBattlefieldBounds() const;

		void					guideMissile();
		void					handleCollision();

		void					destroyEntitiesOutOfViews();
	private:
		enum Layer{
			Background,
			LowerAir,
			UpperAir,
			LayerCount
		};

		struct SpawnPoint {
			SpawnPoint(AircraftType _type, float _x, float _y) :
				type(_type),
				x(_x),
				y(_y) {}
			AircraftType type;
			float x;
			float y;
		};

	private:
		sf::RenderWindow&		window_;
		sf::View				worldView_;
		TextureManager			textures_;

		SceneNode				sceneGraph_;
		std::vector<SceneNode*> sceneLayers_;
		
		CommandQueue			commandQueue_;
		sf::FloatRect			worldBounds_;
		sf::Vector2f			spawnPosition_;
		float					scrollSpeeds_;
		AirCraft*				playerAircraft_;

		int							counter_;
		int							orientation_;
		
		std::vector<SpawnPoint>	enemySpawnPoints_;

		std::vector<AirCraft*>	activeEnemies_;
	};



}