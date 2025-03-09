module;

#include <SFML/Graphics.hpp>

export module Monsters:sea;
import :base;
import <string>;

export class SeaMonsters : public Monsters
{
protected:
	std::string name;
	sf::Texture texture;
	sf::Sprite sprite;

public:
	SeaMonsters() = default;
	SeaMonsters(std::string n, int atk, int def, int hp, std::string str, std::string path) : name(n), Monsters(atk, def, hp, str, "Sea", false) 
	{
		if (!texture.loadFromFile(path))
			return;
		texture.setSmooth(true);
		sprite.setTexture(texture);
	}
	~SeaMonsters() = default;

	sf::Sprite& getSprite() { return sprite; }
	std::string getName() { return name; }

};