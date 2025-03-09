module;

export module Weapon;

import <iostream>;
import <string>;

export class weapon {

protected:
	std::string weapon_class;
	std::string weapon_name;
	int extra_atk = 0;

public:
	weapon() = default;
	weapon(std::string c, std::string n, int atk) : weapon_class(c), weapon_name(n), extra_atk(atk) {}
	~weapon() = default;

	std::string getClass() { return weapon_class; }
	std::string getName() { return weapon_name; }
	int getAtk() { return extra_atk; }

};