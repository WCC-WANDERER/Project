module;

#include <stdlib.h>   // std::rand
#include <SFML/Graphics.hpp>

export module Monsters:base;

import <iostream>;
import <string>;


export class Monsters
{
protected:
	int attack, defence, health, base_health;
	std::string difficulty;
	std::string type;
	bool isBoss;

public:
	Monsters() = default;
	Monsters(int atk, int def, int hp, std::string str, std::string type, bool boss) 
	{
		this->attack = atk;
		this->defence = def;
		this->health = hp;
		this->base_health = hp;
		this->difficulty = str;
		this->type = type;
		this->isBoss = boss;
	}
	~Monsters() = default;

	virtual int getAttack() { return attack; }
	virtual int getDefence() { return defence; }
	virtual int getHp() { return health; }
	virtual int getFullHp() { return base_health; }
	virtual std::string getType() { return type; }
	virtual std::string getDifficulty() { return difficulty; }
	virtual bool checkBoss() { return isBoss; }
	virtual void damage_taken(int damage) { health -= damage; }
	virtual sf::Sprite& getSprite() = 0;
	virtual std::string getName() = 0;
	virtual bool drop(int level) {

		int number = std::rand() % 10 + 1;
		if (level == 0)
			return true;
		else if (level == 1) {
			if (number > 3)
				return true;
			else
				return false;
		}
		else if (level == 2) {
			if (number > 5)
				return true;
			else
				return false;
		}
		else
			return false;
	}

};