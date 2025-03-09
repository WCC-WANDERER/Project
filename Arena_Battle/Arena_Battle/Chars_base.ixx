module;

#include <SFML/Graphics.hpp>

export module Chars:base;

import <iostream>;
import <vector>;
import <string>;
import <filesystem>;
import <ranges>;
import <concepts>;
import Skill;
import Weapon;
import Armour;

export class Characters
{
protected:
	int attack, defence, health, base_attack, base_defence, base_health;
	std::vector<skill> skillSet;
	std::vector<weapon> weaponSet;
	std::vector<armour> armourSet;
	weapon current_weapon;
	armour current_armour;
	bool isBuff = false;

public:
	Characters() = default;
	Characters(int atk, int def, int hp)
	{
		this->attack = atk;
		this->defence = def;
		this->health = hp;
		this->base_attack = atk;
		this->base_defence = def;
		this->base_health = hp;
	}
	~Characters() = default;

	virtual int getAttack() { return attack; }
	virtual int getFullAttack() { return base_attack + current_weapon.getAtk(); }
	virtual void setAttack(int atk) { attack = atk; }
	virtual int getDefence() { return defence; }
	virtual int getFullDefence() { return base_defence + current_armour.getDef(); }
	virtual void setDefence(int def) { defence = def; }
	virtual int getHp() { return health; }
	virtual int getFullHp() { return base_health + current_armour.getHp(); }
	virtual void setHealth(int hp) { health = hp; }
	virtual void reset() {
		health = getFullHp(); 
		attack = getFullAttack(); 
		defence = getFullDefence();
		isBuff = false;
	}
	virtual void damage_taken(int damage) { health -= damage; }
	virtual void addSkill(skill& s) { 
			
		bool repeat = false;
		for (auto& ss : skillSet) {
			if (s.getName() == ss.getName())
				repeat = true;
		}
		if (!repeat)
			skillSet.push_back(s);	
	}
	virtual void addWeapon(weapon& w) {

		weaponSet.push_back(w);

		// Using Ranges to find maximum element
		auto weapon_it = std::ranges::max_element(weaponSet, [](weapon a, weapon b) { return a.getAtk() < b.getAtk(); });

		attack -= current_weapon.getAtk();
		current_weapon = *weapon_it;
		attack += current_weapon.getAtk();
	}
	virtual void addArmour(armour& a) {

		armourSet.push_back(a);

		// Using Ranges to find maximum element
		auto armour_it = std::ranges::max_element(armourSet, [](armour a, armour b) { return a.getHp() < b.getHp(); });
		defence -= current_armour.getDef();
		health -= current_armour.getHp();
		current_armour = *armour_it;
		defence += current_armour.getDef();
		health += current_armour.getHp();
	}
	virtual std::string getSkillName(int number) {

		if (number <= skillSet.size())
			return skillSet[number - 1].getName();
		else
			return "N/A";
	}
	virtual void HpRecovery() {

		if ((health + skillSet[2].getHp()) > getFullHp())
			health = getFullHp();
		else
			health += skillSet[2].getHp();
	}
	virtual skill getSkill(int index) { return skillSet[index - 1]; }
	virtual std::vector<skill> getSkillSet() { return skillSet; }
	virtual weapon& getCurrentWeapon() { return current_weapon; }
	virtual armour& getCurrentArmour() { return current_armour; }
	virtual void buff_active() = 0;
	virtual std::string getName() = 0;
	virtual void setName(std::string& new_name) = 0;
	virtual sf::Sprite& getSprite() = 0;

};