#include "PlayerControl.h"
#include "AirCraft.h"
#include "Command.h"
#include "CommandQueue.h"
#include <functional>

namespace GEX {
	struct AircraftMover {
	
		AircraftMover(float vx, float vy) :
			velocity(vx, vy)
		{}
		void operator() (AirCraft& aircraft, sf::Time dt) const
		{
			aircraft.accelerate(velocity);
		}

		sf::Vector2f velocity;
	};

	PlayerControl::PlayerControl():
		currentMissionStatus_(MissionStatus::MissionRunning)
	{
		//setup bindings
		keyBindings_[sf::Keyboard::Left] = Action::MoveLeft;

		keyBindings_[sf::Keyboard::Right] = Action::MoveRight;

		keyBindings_[sf::Keyboard::Up] = Action::MoveUp;

		keyBindings_[sf::Keyboard::Down] = Action::MoveDown;		

		keyBindings_[sf::Keyboard::Space] = Action::Fire;
		keyBindings_[sf::Keyboard::M] = Action::LaunchMissile;


		keyBindings_[sf::Keyboard::Z] = Action::RR;

		keyBindings_[sf::Keyboard::X] = Action::RL;

		//setup action bindings
		initializeActions();

		
	}

	void PlayerControl::handleEvent(const sf::Event & event, CommandQueue & commands)
	{
		if (event.type == sf::Event::KeyPressed) 
		{
			auto found = keyBindings_.find(event.key.code);
			if (found != keyBindings_.end())
			{
				commands.push(actionBindings_[found->second]);
			}
		}
	}

	void PlayerControl::handleRealtimeInput(CommandQueue& commands)
	{
		//traverse all assigned keys, look up action, generate command
		for (auto pair : keyBindings_) {
			if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second)) {
				commands.push(actionBindings_[pair.second]);
			}
		}
	}

	void PlayerControl::setMissionStatus(MissionStatus status)
	{
		currentMissionStatus_ = status;
	}

	MissionStatus PlayerControl::getMissionStatus() const
	{
		return currentMissionStatus_;
	}

	bool PlayerControl::isRealtimeAction(Action action)
	{
		switch (action)
		{
		case Action::MoveLeft:
		case Action::MoveRight:
		case Action::MoveUp:
		case Action::MoveDown:
		case Action::RR:
		case Action::RL:
			return true;
			break;
		default:
			return false;
			break;
		}
	}

	void PlayerControl::initializeActions()
	{
		const float playerSpeed = 200.f;

		actionBindings_[Action::MoveLeft].action = derivedAction<AirCraft>(AircraftMover(-playerSpeed, 0.f));
		actionBindings_[Action::MoveRight].action = derivedAction<AirCraft>(AircraftMover(+playerSpeed, 0.f));
		actionBindings_[Action::MoveUp].action = derivedAction<AirCraft>(AircraftMover(0.f, -playerSpeed));
		actionBindings_[Action::MoveDown].action = derivedAction<AirCraft>(AircraftMover(0.f, +playerSpeed));

		actionBindings_[Action::RR].action = derivedAction<AirCraft>([](SceneNode& node, sf::Time dt) {node.rotate(+0.1f); });
		actionBindings_[Action::RL].action = derivedAction<AirCraft>([](SceneNode& node, sf::Time dt) {node.rotate(-0.1f); });
		actionBindings_[Action::RR].category = Category::EnemyAircraft;
		actionBindings_[Action::RL].category = Category::EnemyAircraft;

		for (auto& pair : actionBindings_) {
			pair.second.category = Category::PlayerAircraft;
		}

		actionBindings_[Action::Fire].action = derivedAction<AirCraft>(std::bind(&AirCraft::fire, std::placeholders::_1));
		actionBindings_[Action::Fire].category = Category::PlayerAircraft;

		actionBindings_[Action::LaunchMissile].action = derivedAction<AirCraft>(std::bind(&AirCraft::launchMissile, std::placeholders::_1));
		actionBindings_[Action::LaunchMissile].category = Category::PlayerAircraft;




	}


}