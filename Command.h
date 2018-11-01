#pragma once
#include <functional>
#include <cassert>
#include "SFML/System/Time.hpp"
#include "Category.h"

namespace GEX {
	//forward declaration
	class SceneNode;

	struct Command
	{
	public:
		Command();

		std::function<void(SceneNode&, sf::Time)>		action;
		unsigned int									category;
	};

	template <typename GameObject, typename Function>
	std::function<void(SceneNode&, sf::Time)> derivedAction(Function fn)
	{
		return [=](SceneNode& node, sf::Time dt)
		{
			assert(dynamic_cast<GameObject*>(&node) != nullptr);

			fn(static_cast<GameObject&>(node), dt);
		};
	};
}