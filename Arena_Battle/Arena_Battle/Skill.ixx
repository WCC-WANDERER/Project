module;

export module Skill;

import <iostream>;
import <string>;


export class skill {

protected:
	std::string skill_class;
	std::string skill_name;
	int extra_atk;
	int extra_def;
	int extra_hp;

public:
	skill() = default;
	skill(std::string c, std::string n, int atk, int def, int hp) : skill_class(c), skill_name(n), extra_atk(atk), extra_def(def), extra_hp(hp) {}
	~skill() = default;

	std::string getClass() { return skill_class; }
	std::string getName() { return skill_name; }
	int getAtk() { return extra_atk; }
	int getDef() { return extra_def; }
	int getHp() { return extra_hp; }

};