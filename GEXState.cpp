#include "GEXState.h"

namespace GEX {

	GEXState::GEXState()
	{
	}


	GEXState::~GEXState()
	{
	}
	bool GEXState::isSelectable() const
	{
		return false;
	}
	bool GEXState::isSelected() const
	{
		return false;
	}
	void GEXState::select()
	{
	}
	void GEXState::deselect()
	{
	}
	bool GEXState::isActive() const
	{
		return false;
	}
	void GEXState::activate()
	{
	}
	void GEXState::deactivate()
	{
	}
	void GEXState::handleEvent(const sf::Event & event)
	{
	}
}