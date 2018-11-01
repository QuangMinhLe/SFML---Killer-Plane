#pragma once
#include "SceneNode.h"
#include "Particle.h"
#include "TextureManager.h"
#include <SFML/Graphics/VertexArray.hpp>

namespace GEX {
	class ParticleNode : public SceneNode
	{
	public:
		ParticleNode(Particle::Type type, const TextureManager& textures);

		void			addParticle(sf::Vector2f position);
		Particle::Type	getParticleType() const;
		unsigned int	getCategory() const override;

	private:
		void			updateCurrent(sf::Time dt, CommandQueue& commands) override;
		void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

		void			addVertex(float worldX, float worldY, float texCoordU, float texCoordV, const sf::Color color) const;
		void			computeVertices() const;

	private:
		std::deque<Particle>	particles_;
		const sf::Texture&		textures_;
		Particle::Type			type_;

		mutable sf::VertexArray	vertexArray_;
		mutable bool			needsVertexUpdate;
	};

}