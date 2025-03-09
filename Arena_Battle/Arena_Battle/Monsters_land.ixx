module;

#include <SFML/Graphics.hpp>

export module Monsters:land;
import :base;
import <string>;

export class LandMonsters : public Monsters
{
protected:
	std::string name;
	sf::Texture texture;
	sf::Sprite sprite;

public:
	LandMonsters() = default;
	LandMonsters(std::string n, int atk, int def, int hp, std::string str, std::string path) : name(n), Monsters(atk, def, hp, str, "Land", false) 
	{
		if (!texture.loadFromFile(path))
			return;
		texture.setSmooth(true);
		sprite.setTexture(texture);
	}
	~LandMonsters() = default;
	
	sf::Sprite& getSprite() { return sprite; }
	std::string getName() { return name; }

};