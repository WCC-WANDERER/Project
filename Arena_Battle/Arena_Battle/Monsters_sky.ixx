module;

#include <SFML/Graphics.hpp>

export module Monsters:sky;
import :base;
import <string>;

export class SkyMonsters : public Monsters
{
protected:
	std::string name;
	sf::Texture texture;
	sf::Sprite sprite;

public:
	SkyMonsters() = default;
	SkyMonsters(std::string n, int atk, int def, int hp, std::string str, std::string path) : name(n), Monsters(atk, def, hp, str, "Sky", false) 
	{
		if (!texture.loadFromFile(path))
			return;
		texture.setSmooth(true);
		sprite.setTexture(texture);
	}
	~SkyMonsters() = default;

	sf::Sprite& getSprite() { return sprite; }
	std::string getName() { return name; }

};