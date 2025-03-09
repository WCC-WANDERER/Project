
export module Player;
import Chars;
import <iostream>;
import <fstream>;
import <sstream>;
import <string>;
import <filesystem>;


export class Player {

protected:
	std::vector<std::shared_ptr<Characters>> chars;
	int current_stage = 0;

public:

	Player() {

		chars.push_back(std::make_shared<Swordsman>("Swordsman", 30, 50, 150));
		chars.push_back(std::make_shared<Archer>("Archer", 40, 40, 120));
		chars.push_back(std::make_shared<Mage>("Mage", 50, 30, 100));
	}
	~Player() = default;

	auto& getCharList() { return chars; }
	int getStage() { return current_stage; }
	void setChars(std::vector<std::shared_ptr<Characters>>& chars_update) {

		chars.clear();
		chars = chars_update;
	}
	void setStage(int level) { current_stage = level; }
	auto& getCharacter(int index) {

		switch (index) {

		case 0:
			return chars[0];

		case 1:
			return chars[1];

		case 2:
			return chars[2];
		}
	}

	void SaveAll() {

		if (!std::filesystem::exists("save"))
			std::filesystem::create_directory("save");

		std::ofstream Save_file("save/LatestSave.txt");
		if (Save_file) {
			Save_file << "Stage" << " " << current_stage << "\n";

			for (auto& c : chars) {

				std::string name = c->getName();
				int atk = c->getAttack();
				int def = c->getDefence();
				int hp = c->getHp();
				Save_file << name << " " << atk << " " << def << " " << hp << "\n";

				// Only save skills if the character has any
				std::vector<skill> skill_set = c->getSkillSet();
				if (!skill_set.empty()) {
					for (auto& skill : skill_set) {
						std::string skill_class = skill.getClass();
						std::string skill_name = skill.getName();
						int skill_atk = skill.getAtk();
						int skill_def = skill.getDef();
						int skill_hp = skill.getHp();
						Save_file << "Skill" << " " << skill_class << " " << skill_name
							<< " " << skill_atk << " " << skill_def << " " << skill_hp << "\n";
					}
				}

				// Only save weapon if it is valid
				if (!c->getCurrentWeapon().getName().empty()) {
					std::string weapon_class = c->getCurrentWeapon().getClass();
					std::string weapon_name = c->getCurrentWeapon().getName();
					int weapon_atk = c->getCurrentWeapon().getAtk();
					Save_file << "Weapon" << " " << weapon_class << " " << weapon_name
						<< " " << weapon_atk << "\n";
				}

				// Only save armor if it is valid
				if (!c->getCurrentArmour().getName().empty()) {
					std::string armour_class = c->getCurrentArmour().getClass();
					std::string armour_name = c->getCurrentArmour().getName();
					int armour_def = c->getCurrentArmour().getDef();
					int armour_hp = c->getCurrentArmour().getHp();
					Save_file << "Armour" << " " << armour_class << " " << armour_name
						<< " " << armour_def << " " << armour_hp << "\n";
				}
			}
		}
		else
			std::cerr << "Cannot open the file.\n";
	}

	void LoadAll() {

		if (std::filesystem::exists("save")) {

			std::ifstream player_file("save/LatestSave.txt");
			if (player_file) {

				std::string line;
				std::stringstream ss;
				std::string Stage = "Stage";
				std::string Skill = "Skill";
				std::string Weapon = "Weapon";
				std::string Armour = "Armour";
				std::string name, skill_class, skill_name, skill_name2, weapon_class, weapon_name, weapon_name2, armour_class, armour_name, armour_name2;
				std::string sAtk, sDef, sHp;
				int current_stage, atk, def, hp, skill_atk, skill_def, skill_hp, weapon_atk, armour_def, armour_hp;
				int char_index = -1;

				while (std::getline(player_file, line)) {

					ss.str(line);

					if (line.find(Stage) != std::string::npos) {
						ss >> Stage >> current_stage;
						this->setStage(current_stage);
					}
					else if (line.find(Skill) != std::string::npos) {
						ss >> Skill >> skill_class >> skill_name >> skill_name2 >> skill_atk >> skill_def >> skill_hp;
						skill temp(skill_class, skill_name + " " + skill_name2, skill_atk, skill_def, skill_hp);
						chars[char_index]->addSkill(temp);
					}
					else if (line.find(Weapon) != std::string::npos) {
						ss >> Weapon >> weapon_class >> weapon_name >> weapon_name2 >> weapon_atk;
						weapon temp(weapon_class, weapon_name + " " + weapon_name2, weapon_atk);
						chars[char_index]->addWeapon(temp);
					}
					else if (line.find(Armour) != std::string::npos) {
						ss >> Armour >> armour_class >> armour_name >> armour_name2 >> armour_def >> armour_hp;
						armour temp(armour_class, armour_name + " " + armour_name2, armour_def, armour_hp);
						chars[char_index]->addArmour(temp);
					}
					else {

						// Ensure the line contains four valid tokens (name, atk, def, hp)
						if (!(ss >> name >> sAtk >> sDef >> sHp)) {
							continue; // Skip invalid lines
						}

						if (char_index < 2)
							char_index += 1;

						// Convert the string values to integers
						int atk = std::stoi(sAtk);
						int def = std::stoi(sDef);
						int hp = std::stoi(sHp);

						this->chars[char_index]->setName(name);
						this->chars[char_index]->setAttack(atk);
						this->chars[char_index]->setDefence(def);
						this->chars[char_index]->setHealth(hp);
					}

					ss.clear();

				}
			}
			else
				std::cerr << "Cannot open the file.\n";
		}
		else
			std::cerr << "Directory not found.\n";
	}

};


