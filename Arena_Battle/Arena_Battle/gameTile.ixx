module;

#include <SFML/Graphics.hpp>

export module gameMap:gameTile;

export class gameTile {

public:
	bool isPassable;
	bool isEntry;
	sf::Texture texture;
	sf::Sprite sprite;

	gameTile(std::string texture_name, float x, float y , bool passable, bool entry) {
		if (!initialize_sprite(texture_name))
			return;
		sprite.setPosition(x, y);
		isPassable = passable;
		isEntry = entry;
	}
	~gameTile() = default;

	bool initialize_sprite(std::string texture_name) {
		if (!texture.loadFromFile(texture_name))
			return false;
		texture.setSmooth(true);
		sprite.setTexture(texture);
		return true;
	}
};