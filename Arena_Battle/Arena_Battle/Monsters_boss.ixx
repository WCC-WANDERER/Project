module;

#include <SFML/Graphics.hpp>

export module Monsters:boss;
import :base;
import <string>;

export class BossMonsters : public Monsters
{
protected:
	std::string name;
	sf::Texture texture;
	sf::Sprite sprite;

public:
	BossMonsters() = default;
	BossMonsters(std::string n, int atk, int def, int hp, std::string str, std::string path) : name(n), Monsters(atk, def, hp, str, "Boss", true) 
	{
		if (!texture.loadFromFile(path))
			return;
		texture.setSmooth(true);
		sprite.setTexture(texture);
	}
	~BossMonsters() = default;

	sf::Sprite& getSprite() { return sprite; }
	std::string getName() { return name; }

};
