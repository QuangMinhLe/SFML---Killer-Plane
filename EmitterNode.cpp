#include "EmitterNode.h"
#include "Command.h"

namespace GEX {

	EmitterNode::EmitterNode(Particle::Type type):
		SceneNode(),
		accumulatedTime_(sf::Time::Zero),
		type_(type),
		particleSystem_(nullptr)
	{

	}
	
	void EmitterNode::updateCurrent(sf::Time dt, CommandQueue & commands)
	{
		if (particleSystem_) {
			emitParticle(dt);
		}
		else
		{
			auto finder = [this](ParticleNode& container, sf::Time) {
				if (container.getParticleType() == type_){
					particleSystem_ = &container;
				}
			};


			Command command;
			command.category = Category::ParticleSystem;
			command.action = derivedAction<ParticleNode>(finder);

			commands.push(command);
		}
	}
	
	void EmitterNode::emitParticle(sf::Time dt)
	{
		const float emissionRate = 30.f;
		const sf::Time interval = sf::seconds(1.f) / emissionRate;

		accumulatedTime_ += dt;
		
		while (accumulatedTime_ > interval) {
			accumulatedTime_ -= interval;
			particleSystem_->addParticle(getWorldPosition());
		}
	}
}