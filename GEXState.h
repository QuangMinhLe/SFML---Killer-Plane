#pragma once
#include <SFML/Graphics.hpp>

namespace GEX {

	class GEXState : public sf::Drawable,
					 public sf::Transformable,
					 private sf::NonCopyable
	{
	public:
		typedef std::shared_ptr<GEXState> ptr;

	public:
							GEXState();
		virtual				~GEXState();
		virtual bool		isSelectable() const = 0;
		bool				isSelected() const;
		virtual void		select();
		virtual void		deselect();        
		virtual bool		isActive() const;
		virtual void		activate();        
		virtual void		deactivate();
		virtual void		handleEvent(const sf::Event& event) = 0;    
	private:        
		bool				mIsSelected;
		bool				mIsActive;
	};

}