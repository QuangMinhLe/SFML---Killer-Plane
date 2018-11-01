#pragma once
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>
#include <vector>
#include <memory>
#include "Command.h"
#include "CommandQueue.h"
#include "Ultility.h"
#include <set>

//forward declaration
struct Command;

namespace GEX {

	class CommandQueue;
	struct Command;

	class SceneNode: public sf::Transformable, public sf::Drawable
	{
	public:
		using Ptr = std::unique_ptr<SceneNode>;
		using Pair = std::pair<SceneNode*, SceneNode*>;

	public:
									SceneNode(Category::Type category = Category::Type::None);
		virtual						~SceneNode();
									SceneNode(const SceneNode&) = delete;
		SceneNode&					operator=(SceneNode&) = delete;

		void						attachChild(Ptr child);
		Ptr							detachChild(const SceneNode& node);

		void						onCommand(const Command& command, sf::Time dt);
		virtual unsigned int		getCategory() const;

		void						update(sf::Time dt, CommandQueue& commands);
		sf::Vector2f				getWorldPosition() const;
		sf::Transform				getWorldTransform() const;

		virtual sf::FloatRect		getBoundingBox() const;
		void						drawBoundingBox(sf::RenderTarget& target, sf::RenderStates states) const;

		void						checkSceneCollision(SceneNode& rootNode, std::set<Pair>& collisionPair);
		void						checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPair);

		virtual bool				isDestroyed() const;
		virtual bool				isMarkedForRemoval() const;
		void						removeWrecks();

	private:
		//update the tree
		virtual void				updateCurrent(sf::Time dt, CommandQueue& commands);
		void						updateChildren(sf::Time dt, CommandQueue& commands);

		//draw the tree
		void						draw(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void				drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		void						drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		SceneNode*					parent_;	
		std::vector<Ptr>			children_;
		Category::Type				defaultCategory_;
	};

	float							distance(const SceneNode& lhs, const SceneNode& rhs);
	bool							collision(const SceneNode& lhs, const SceneNode& rhs);
}
