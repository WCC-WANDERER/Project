module;

#include <SFML/Graphics.hpp>

export module Chars:Archer;
import :base;

export class Archer : public Characters
{
protected:
	std::string name;
	sf::Texture texture;
	sf::Sprite sprite;

public:
	Archer() = default;
	Archer(std::string n, int atk, int def, int hp) : name(n), Characters(atk, def, hp)
	{
		if (!texture.loadFromFile("image/archer.png"))
			return;
		texture.setSmooth(true);
		sprite.setTexture(texture);
		sprite.setPosition(70.f, 150.f);
		sprite.setScale(0.8f, 0.8f);
	}
	~Archer() = default;

	std::string getName() { return name; }

	void setName(std::string& new_name) { name = new_name; }

	void buff_active() {
		if (!isBuff) {
			attack += skillSet[1].getAtk();
			isBuff = true;
		}
	}
	sf::Sprite& getSprite() { return sprite; }

};