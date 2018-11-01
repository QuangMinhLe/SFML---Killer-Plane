//#pragma once
//#include <SFML/Graphics.hpp>
//#include "TextureManager.h"
//#include "World.h"
//#include "PlayerControl.h"
//#include "CommandQueue.h"
//
//enum class Directions {UP, DOWN, RIGHT, LEFT};
//
//class Game
//{
//public:
//	Game();
//	~Game();
//
//	void run();
//
//private:
//	void				processEvents();
//	void				update(sf::Time deltaTime, GEX::CommandQueue& commands);
//	void				render();
//
//	void				handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
//	void				updateStatistics(sf::Time deltaTime);
//
//	//void				drawBoundingBox();
//	void				loadTextures();
//
//private:
//	sf::RenderWindow	window_;
//	GEX::World			world_;
//
//	GEX::PlayerControl		player_;
//	//sf::Sprite			player_;
//	//GEX::TextureManager	textures_;
//
//	sf::Text			statisticsText_;
//	sf::Time			statisticsUpdateTime_;
//	sf::Font			font_;
//	unsigned int		statisticsNumFrames_;
//
//	/*bool				isRotatingLeft_;
//	bool				isRotatingRight_;
//	bool				isMovingUp_;
//	bool				isMovingDown_;
//	bool				isMovingLeft_;
//	bool				isMovingRight_;
//	sf::Vector2f		velocity_;
//	float				angularVelocity_;*/
//};
//
