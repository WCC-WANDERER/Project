module;

#include <SFML/Graphics.hpp>

export module Chars:Swordsman;
import :base;

export class Swordsman : public Characters
{
protected:
	std::string name;
	sf::Texture texture;
	sf::Sprite sprite;

public:
	Swordsman() = default;
	Swordsman(std::string n, int atk, int def, int hp) : name(n), Characters(atk, def, hp)
	{
		if (!texture.loadFromFile("image/swordsman.png"))
			return;
		texture.setSmooth(true);
		sprite.setTexture(texture);
		sprite.setPosition(70.f, 150.f);
		sprite.setScale(0.8f, 0.8f);
	}
	~Swordsman() = default;

	std::string getName() { return name; }

	void setName(std::string& new_name) { name = new_name; }
	
	void buff_active() { 
		if (!isBuff) {
			defence += skillSet[1].getDef();
			isBuff = true;
		}
	}
	sf::Sprite& getSprite() { return sprite; }

};