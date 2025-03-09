module;

export module Armour;

import <iostream>;
import <string>;

export class armour {

protected:
	std::string armour_class;
	std::string armour_name;
	int extra_def = 0;
	int extra_hp = 0;

public:
	armour() = default;
	armour(std::string c, std::string n, int def, int hp) : armour_class(c), armour_name(n), extra_def(def), extra_hp(hp) {}
	~armour() = default;

	std::string getClass() { return armour_class; }
	std::string getName() { return armour_name; }
	int getDef() { return extra_def; }
	int getHp() { return extra_hp; }

};