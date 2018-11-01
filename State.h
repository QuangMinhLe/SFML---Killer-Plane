#pragma once
#include "TextureManager.h"
#include <SFML/Graphics.hpp>
#include "StateIdentifier.h"
#include <memory>

namespace GEX {
	//forward declaration
	class StateStack;
	class PlayerControl;
 
	class State
	{
	public:
		typedef std::unique_ptr<State> Ptr;

		struct Context
		{
			Context(
				sf::RenderWindow&		window,
				TextureManager&			textures,
				PlayerControl&			player
			);



			sf::RenderWindow*			window;
			TextureManager*				textures;
			PlayerControl*				player;
		};

	public:
		State(StateStack& stack, Context context);
		virtual ~State();
		virtual void					draw() = 0;
		virtual bool					update(sf::Time) = 0;
		virtual bool					handleEvent(const sf::Event& event) = 0;

	protected:
		void							requestStackPush(StateID stateID);
		void							requestStackPop();
		void							requestStackClear();

		Context getContext() const;

		
	private:
		StateStack*		stack_;
		Context			context_;
	
	};

}