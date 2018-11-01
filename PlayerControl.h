#pragma once
#include <map>
#include "Command.h"
#include "Category.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>


namespace GEX {
	//forward declaration
	class CommandQueue;

	enum class Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		RR,
		RL,
		Fire,
		LaunchMissile
	};

	enum class MissionStatus
	{
		MissionRunning,
		MissionAccomplished,
		MissionFailure
	};
	
	class PlayerControl
	{
	public:
		PlayerControl();
		void									handleEvent(const sf::Event& event, CommandQueue& commands);
		void									handleRealtimeInput(CommandQueue& commands);

		void									setMissionStatus(MissionStatus status);
		MissionStatus							getMissionStatus()const;
	private:

		void									initializeActions();
		static bool								isRealtimeAction(Action action);

	private:
		std::map<sf::Keyboard::Key, Action>		keyBindings_;
		std::map<Action, Command>				actionBindings_;
		MissionStatus							currentMissionStatus_;
	};

}