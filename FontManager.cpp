#include "FontManager.h"
#include <cassert>


namespace GEX {
	FontManager* FontManager::instance_ = nullptr;

	FontManager & FontManager::getInstance()
	{
		if (!instance_)
			FontManager::instance_ = new FontManager();

		return *FontManager::instance_;
	}

	void FontManager::load(FontID id, const std::string & path)
	{
		std::unique_ptr<sf::Font> font(new sf::Font);

		if (!font->loadFromFile(path))
			throw std::runtime_error("Font load failed" + path);

		auto rc = fonts_.insert(std::make_pair(id, std::move(font)));
		if (!rc.second)
			assert(0);
	}

	sf::Font & FontManager::get(FontID id) const
	{
		auto found = fonts_.find(id);
		assert(found != fonts_.end());
		
		return *found->second;
	}
}