#include "Pickup.h"
#include "DataTable.h"
#include "Category.h"

namespace {
	const std::map<GEX::Pickup::Type, GEX::PickupData> TABLE = GEX::initializePickupData();
}
namespace GEX {

	Pickup::Pickup(Type type, const TextureManager & textures): 
		Entity(1),
		type_(type),
		sprite_(textures.get(TABLE.at(type).texture), TABLE.at(type).textureRect)
	{
		centerOrigin(sprite_);
	}

	unsigned int Pickup::getCategory() const
	{
		return Category::Pickup;
	}

	sf::FloatRect Pickup::getBoundingBox() const
	{
		return getWorldTransform().transformRect(sprite_.getGlobalBounds());
	}

	void Pickup::apply(AirCraft & player)
	{
		TABLE.at(type_).action(player);
	}

	void Pickup::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		target.draw(sprite_, states);
	}


}