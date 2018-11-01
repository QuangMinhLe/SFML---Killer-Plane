//#include "Game.h"
//
//namespace {
//	const sf::IntRect NO_ROLL(0, 0, 48, 64);
//	const sf::IntRect ROLL_LEFT(48, 0, 48, 64);
//	const sf::IntRect ROLL_RIGHT(96, 0, 48, 64);
//}
//
//Game::Game() : window_(sf::VideoMode(800, 600), "Killer Circle")
////, player_()
////, velocity_(0.f, 0.f)
////, textures_()
////, angularVelocity_(0.f)
////, isRotatingLeft_(false)
////, isRotatingRight_(false)
////, isMovingDown_(false)
////, isMovingLeft_(false)
////, isMovingRight_(false)
////, isMovingUp_(false)
//, world_(window_)
//, statisticsNumFrames_(0)
//, statisticsText_()
//, statisticsUpdateTime_(sf::Time::Zero)
//{
//	loadTextures();
//
//	//player_.setTexture(textures_.get(GEX::TextureID::Airplane));
//	//player_.setTextureRect(sf::IntRect(0, 0, 48, 64));
//	//player_.setPosition(300, 200);
///*
//	sf::FloatRect bounds = player_.getLocalBounds();
//	player_.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
//*/
//	if (!font_.loadFromFile("Media/Sansation.ttf")) {
//		//error handling
//	}
//
//	statisticsText_.setFont(font_);
//	statisticsText_.setPosition(5.0f, 5.0f);
//	statisticsText_.setCharacterSize(15.0f);
//	statisticsText_.setString("Frames / Second = \nTime / Update = ");
//}
//
//Game::~Game()
//{
//}
//
//void Game::run()
//{
//	const sf::Time SPF = sf::seconds(1.0f / 60.0f);
//
//	sf::Clock clock;
//	sf::Time timeSinceLastUpdate = sf::Time::Zero;
//
//	while (window_.isOpen()) {
//		processEvents();
//		timeSinceLastUpdate += clock.restart();
//		while (timeSinceLastUpdate > SPF) {
//			updateStatistics(SPF);
//			update(SPF);
//			timeSinceLastUpdate -= SPF;
//		}/*
//		updateStatistics(timeSinceLastUpdate);
//		update(timeSinceLastUpdate);*/
//		render();
//
//	}
//
//}
//
//void Game::processEvents() 
//{
//	GEX::CommandQueue& commands = world_.getCommandQueue();
//
//	sf::Event event;
//
//	while (window_.pollEvent(event))
//	{
//		/*switch (event.type) {
//
//		case sf::Event::KeyPressed:
//			handlePlayerInput(event.key.code, true);
//			break;
//
//		case sf::Event::KeyReleased:
//			handlePlayerInput(event.key.code, false);
//			break;
//
//		case sf::Event::Closed:
//			window_.close();
//			break;
//*/
//		player_.handleEvent(event, commands);
//	
//		if(event.type == sf::Event::Closed){
//			window_.close();
//		}
//
//		player_.handleRealtimeInput(commands);
//	}
//}
//void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
//{
//	//if (key == sf::Keyboard::W)
//	//	isMovingUp_ = isPressed;
//
//	//if (key == sf::Keyboard::S)
//	//	isMovingDown_ = isPressed;
//
//	//if (key == sf::Keyboard::D) {
//	//	isMovingRight_ = isPressed;
//	//}
//	//if (key == sf::Keyboard::A) {
//	//	isMovingLeft_ = isPressed;
//	//}
//	//if (key == sf::Keyboard::J){
//	//	isRotatingLeft_ = isPressed;
//	//	player_.setTextureRect(sf::IntRect(0, 48, 48, 64));
//	//}
//	//if (key == sf::Keyboard::K) {
//	//	isRotatingRight_ = isPressed;
//	//	player_.setTextureRect(sf::IntRect(48, 0, 48, 64));
//	//}
//}
//
//void Game::updateStatistics(sf::Time deltaTime)
//{
//	statisticsUpdateTime_ += deltaTime;
//	statisticsNumFrames_ += 1;
//
//	if (statisticsUpdateTime_ > sf::seconds(1)) {
//		statisticsText_.setString(
//			"Frames / Second = " + std::to_string(statisticsNumFrames_) + "\n" +
//			"Time / Updte = " + std::to_string(statisticsUpdateTime_.asMicroseconds() / statisticsNumFrames_) + "ms"
//		);
//		
//		statisticsUpdateTime_ -= sf::seconds(1);
//		statisticsNumFrames_ = 0;
//	}
//
//}
//
//
//void Game::render()
//{
//	window_.clear();
//	world_.draw();
//	
//	window_.setView(window_.getDefaultView());
//	window_.draw(statisticsText_);
//	window_.display();
//}
//
////void Game::drawBoundingBox()
////{
////	auto rect = player_.getGlobalBounds();
////
////	sf::RectangleShape boundingBox;
////	boundingBox.setPosition(sf::Vector2f(rect.left, rect.top));
////	boundingBox.setSize(sf::Vector2f(rect.width, rect.height));
////	boundingBox.setFillColor(sf::Color::Transparent);
////	boundingBox.setOutlineColor(sf::Color::Green);
////	boundingBox.setOutlineThickness(1.0f);
////
////	window_.draw(boundingBox);
////
////}
//
//void Game::loadTextures()
//{
//	//textures_.load(GEX::TextureID::Airplane, "Media/Textures/eagles.png");
//}
//
//
//void Game::update(sf::Time deltaTime, GEX::CommandQueue& commands)
//{
//	world_.update(deltaTime, commands);
//
//	//const float PLAYER_SPEED = 0.5f;
//
//	//if (isMovingUp_) {
//	//	velocity_.y -= PLAYER_SPEED;
//	//}
//	//if (isMovingDown_) {
//	//	velocity_.y += PLAYER_SPEED;
//	//}
//	//if (isMovingLeft_) {
//	//	velocity_.x -= PLAYER_SPEED;
//	//}
//	//if (isMovingRight_) {
//	//	velocity_.x += PLAYER_SPEED;
//	//}
//	//if (isRotatingLeft_) {
//	//	angularVelocity_ += PLAYER_SPEED;
//	//}
//	//if (isRotatingRight_) {
//	//	angularVelocity_ -= PLAYER_SPEED;
//	//}
//
//	//player_.move(velocity_ * deltaTime.asSeconds());
//	//player_.rotate(angularVelocity_ * deltaTime.asSeconds());
//	//player_.setTextureRect(NO_ROLL);
//	//if (velocity_.x < -0.4) {
//	//	player_.setTextureRect(ROLL_LEFT);
//	//}
//	//if (velocity_.x > -0.4) {
//	//	player_.setTextureRect(ROLL_RIGHT);
//	//}
//	//auto p = player_.getPosition();
//
//
//	//p.x = p.x > 800 ? 0 : p.x;
//	//p.y = p.y > 600 ? 0 : p.y;
//
//	//p.x = p.x < 0 ? 800 : p.x;
//	//p.y = p.y < 0 ? 600 : p.y;
//
//
//	//player_.setPosition(p);
//}