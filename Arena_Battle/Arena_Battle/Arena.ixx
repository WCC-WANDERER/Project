module;

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <ranges>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

export module Arena;
import Player;
import Chars;
import Monsters;
import Entity;
import gameMap;
import Skill;
import Weapon;
import Armour;


template<typename T>
concept StringOnly = requires (T str)
{
	{ str } -> std::convertible_to<std::string>;  // std::string type is accepted for message to be shown
};


export class Arena : public Entity  {

protected:

	sf::Font font;
	sf::Text message, text1, text2, text3, text4, char_text, monster_text, hp_text, atk_text, def_text;
	sf::RectangleShape messagebox, textbox1, textbox2, textbox3, textbox4, char_hp_bar, monster_hp_bar;
	sf::Music music;
	sf::Clock clock;
	std::vector<std::shared_ptr<Characters>> chars;
	std::vector<std::vector<std::unique_ptr<Monsters>>> monsters;
	std::vector<std::unique_ptr<Entity>> char_effects;
	std::vector<std::unique_ptr<Entity>> monster_effects;
	std::vector<std::unique_ptr<Entity>> char_messagebox;
	std::vector<std::unique_ptr<Entity>> background;
	std::vector<std::vector<skill>> skill_list;
	std::vector<std::vector<weapon>> weapon_list;
	std::vector<std::vector<armour>> armour_list;
	int _difficulty = 0;
	int char_index = 1;
	int monster_index = 0;
	int char_damage = 0;
	int monster_damage = 0;
	bool my_turn = false;
	bool monster_turn = false;
	bool atk_select = false;
	bool char_select = false;
	bool set_message_hp = false;
	bool isSkill = false;
	bool isBuff = false;
	bool shield = false;
	bool finish_turn = true;
	bool isBattle = true;
	bool isLose = false;
	bool isMusic = true;

public:

	Arena() {
		
		this->chars.push_back(std::make_shared<Swordsman>("Swordsman", 30, 50, 150));
		this->chars.push_back(std::make_shared<Archer>("Archer", 40, 40, 120));
		this->chars.push_back(std::make_shared<Mage>("Mage", 50, 30, 100));
		this->char_effects.push_back(std::move(std::make_unique<Entity>("image/sword.png", 200.f, 200.f, 0.8f, 0.8f)));
		this->char_effects.push_back(std::move(std::make_unique<Entity>("image/arrow.png", 200.f, 200.f, 0.5f, 0.5f)));
		this->char_effects.push_back(std::move(std::make_unique<Entity>("image/fireball.png", 200.f, 150.f, 0.5f, 0.5f)));
		this->char_effects.push_back(std::move(std::make_unique<Entity>("image/explosion.png", 0.5f)));
		this->char_effects.push_back(std::move(std::make_unique<Entity>("image/shield.png", 270.f, 210.f, 0.2f, 0.2f)));
		this->char_effects.push_back(std::move(std::make_unique<Entity>("image/thunder.png", 900.f, 150.f, 0.4f, 0.4f)));
		this->monster_effects.push_back(std::move(std::make_unique<Entity>("image/animal/stone.png", 700.f, 210.f, 0.3f, 0.3f)));
		this->monster_effects.push_back(std::move(std::make_unique<Entity>("image/animal/rock.png", 700.f, 200.f, 0.3f, 0.3f)));
		this->monster_effects.push_back(std::move(std::make_unique<Entity>("image/animal/wood.png", 600.f, 230.f, 0.4f, 0.4f)));
		this->monster_effects.push_back(std::move(std::make_unique<Entity>("image/animal/tree.png", 700.f, 170.f, 0.4f, 0.4f)));
		this->monster_effects.push_back(std::move(std::make_unique<Entity>("image/animal/water.png", 700.f, 160.f, 0.4f, 0.4f)));
		this->monster_effects.push_back(std::move(std::make_unique<Entity>("image/animal/tsunami.png", 700.f, 180.f, 0.3f, 0.3f)));
		this->monster_effects.push_back(std::move(std::make_unique<Entity>("image/explosion.png", 0.5f)));
		this->char_messagebox.push_back(std::move(std::make_unique<Entity>("image/lass2.png", 90.f, 410.f, 0.59f, 0.59f)));  // Swordsman 
		this->char_messagebox.push_back(std::move(std::make_unique<Entity>("image/lire2.png", 55.f, 410.f, 0.64f, 0.64f)));  // Archer
		this->char_messagebox.push_back(std::move(std::make_unique<Entity>("image/arme2.png", 40.f, 410.f, 0.7f, 0.7f)));    // Mage	
		this->background.push_back(std::move(std::make_unique<Entity>("image/land.png")));
		this->background.push_back(std::move(std::make_unique<Entity>("image/sky.png")));
		this->background.push_back(std::move(std::make_unique<Entity>("image/sea.png")));
		loadMonsters();
		setMonsters();
		loadSkills();
		loadWeapons();
		loadArmours();
		init_text_shape();
		setMusic(music, "music/battle.mp3");
	}
	~Arena() = default;

	void loadSave(Player& player_update) {

		// Update from player save
		chars.clear();
		chars = player_update.getCharList();
		_difficulty = player_update.getStage();
		reset_data();
	}

	void loadMonsters() {

		monsters.clear();
		std::ifstream monster_file("monsters_list.txt");
		if (monster_file)
		{
			std::string line;
			std::vector<std::unique_ptr<Monsters>> easy;
			std::vector<std::unique_ptr<Monsters>> medium;
			std::vector<std::unique_ptr<Monsters>> hard;
			std::stringstream ss;
			std::string difficulty, type, name, path;
			int atk, def, hp;
			while (std::getline(monster_file, line))
			{
				ss.str(line);
				ss >> difficulty >> type >> name >> atk >> def >> hp >> path;

				if (difficulty == "easy") {
					if (type == "land")
					{
						easy.push_back(std::move(std::make_unique<LandMonsters>(name, atk, def, hp, difficulty, path)));
					}
					else if (type == "sky")
					{	
						easy.push_back(std::move(std::make_unique<SkyMonsters>(name, atk, def, hp, difficulty, path)));
					}
					else if (type == "sea")
					{
						easy.push_back(std::move(std::make_unique<SeaMonsters>(name, atk, def, hp, difficulty, path)));
					}
					else if (type == "boss")
					{
						easy.push_back(std::move(std::make_unique<BossMonsters>(name, atk, def, hp, difficulty, path)));
					}
				}
				else if (difficulty == "medium") {
					if (type == "land")
					{
						medium.push_back(std::move(std::make_unique<LandMonsters>(name, atk, def, hp, difficulty, path)));
					}
					else if (type == "sky")
					{
						medium.push_back(std::move(std::make_unique<SkyMonsters>(name, atk, def, hp, difficulty, path)));
					}
					else if (type == "sea")
					{
						medium.push_back(std::move(std::make_unique<SeaMonsters>(name, atk, def, hp, difficulty, path)));
					}
					else if (type == "boss")
					{
						medium.push_back(std::move(std::make_unique<BossMonsters>(name, atk, def, hp, difficulty, path)));
					}
				}
				else if (difficulty == "hard") {
					if (type == "land")
					{
						hard.push_back(std::move(std::make_unique<LandMonsters>(name, atk, def, hp, difficulty, path)));
					}
					else if (type == "sky")
					{
						hard.push_back(std::move(std::make_unique<SkyMonsters>(name, atk, def, hp, difficulty, path)));
					}
					else if (type == "sea")
					{
						hard.push_back(std::move(std::make_unique<SeaMonsters>(name, atk, def, hp, difficulty, path)));
					}
					else if (type == "boss")
					{
						hard.push_back(std::move(std::make_unique<BossMonsters>(name, atk, def, hp, difficulty, path)));
					}
				}
				ss.clear();
			}
			monsters.push_back(std::move(easy));
			monsters.push_back(std::move(medium));
			monsters.push_back(std::move(hard));
			monster_file.close();
		}
		else
			std::cerr << "Cannot open the file.\n";
	}

	void setMonsters() {

		float scale = 0;
		for (auto& v : monsters) {

			for (auto& m : v) {

				if (m->getSprite().getGlobalBounds().width > m->getSprite().getGlobalBounds().height)
					scale = 250 / m->getSprite().getGlobalBounds().width;
				else
					scale = 250 / m->getSprite().getGlobalBounds().height;

				m->getSprite().setScale(scale, scale);
				float x = 1130 - m->getSprite().getGlobalBounds().width;
				float y = 360 - m->getSprite().getGlobalBounds().height;
				m->getSprite().setPosition(x, y);
			}
		}
	}

	void loadSkills() {

		skill_list.clear();
		std::ifstream skill_file("skill_list.txt");
		if (skill_file)
		{
			std::string line;
			std::vector<skill> easy;
			std::vector<skill> medium;
			std::vector<skill> hard;
			std::stringstream ss;
			std::string difficulty, skill_class, name, name2;
			int atk, def, hp;
			
			while (std::getline(skill_file, line))
			{
				ss.str(line);				
				ss >> difficulty >> skill_class >> name >> name2 >> atk >> def >> hp;

				if (difficulty == "easy") 						
					easy.push_back(skill(skill_class, name + " " + name2, atk, def, hp));
				else if (difficulty == "medium") 
					medium.push_back(skill(skill_class, name + " " + name2, atk, def, hp));
				else if (difficulty == "hard") 
					hard.push_back(skill(skill_class, name + " " + name2, atk, def, hp));
				
				ss.clear();				
			}
			skill_list.push_back(easy);
			skill_list.push_back(medium);
			skill_list.push_back(hard);
			skill_file.close();
		}
		else
			std::cerr << "Cannot open the file.\n";
	}

	void loadWeapons() {

		weapon_list.clear();
		std::ifstream weapon_file("weapon_list.txt");
		if (weapon_file)
		{
			std::string line;
			std::vector<weapon> easy;
			std::vector<weapon> medium;
			std::vector<weapon> hard;
			std::stringstream ss;
			std::string difficulty, skill_class, name, name2;
			int atk;

			while (std::getline(weapon_file, line))
			{
				ss.str(line);
				ss >> difficulty >> skill_class >> name >> name2 >> atk;

				if (difficulty == "easy")
					easy.push_back(weapon(skill_class, name + " " + name2, atk));
				else if (difficulty == "medium")
					medium.push_back(weapon(skill_class, name + " " + name2, atk));
				else if (difficulty == "hard")
					hard.push_back(weapon(skill_class, name + " " + name2, atk));

				ss.clear();
			}
			weapon_list.push_back(easy);
			weapon_list.push_back(medium);
			weapon_list.push_back(hard);
			weapon_file.close();
		}
		else
			std::cerr << "Cannot open the file.\n";
	}

	void loadArmours() {
		armour_list.clear();
		std::ifstream armour_file("armour_list.txt");
		if (armour_file)
		{
			std::string line;
			std::vector<armour> easy;
			std::vector<armour> medium;
			std::vector<armour> hard;
			std::stringstream ss;
			std::string difficulty, skill_class, name, name2;
			int def, hp;

			while (std::getline(armour_file, line))
			{
				ss.str(line);
				ss >> difficulty >> skill_class >> name >> name2 >> def >> hp;

				if (difficulty == "easy")
					easy.push_back(armour(skill_class, name + " " + name2, def, hp));
				else if (difficulty == "medium")
					medium.push_back(armour(skill_class, name + " " + name2, def, hp));
				else if (difficulty == "hard")
					hard.push_back(armour(skill_class, name + " " + name2, def, hp));

				ss.clear();
			}
			armour_list.push_back(easy);
			armour_list.push_back(medium);
			armour_list.push_back(hard);
			armour_file.close();
		}
		else
			std::cerr << "Cannot open the file.\n";
	}

	void setMusic(sf::Music& music, const std::string& music_name)
	{
		if (!music.openFromFile(music_name))
			return;
		music.setVolume(70);
		music.setLoop(true);
	}

	void playMusic() {
		if (isMusic) {
			music.play();
			isMusic = false;
		}
	}

	void reset_data() {

		// Reset clock and initialize default data except difficulty
		clock.restart();
		chars[0]->reset();       // reset attack, defence and health of characters
		chars[1]->reset();
		chars[2]->reset();
		char_index = 1;
		monster_index = 0;
		char_damage = 0;
		monster_damage = 0;
		my_turn = false;
		monster_turn = false;
		atk_select = false;
		char_select = false;
		set_message_hp = false;
		isSkill = false;
		isBuff = false;
		shield = false;
		finish_turn = true;
		isBattle = true;
		isLose = false;
		isMusic = true;
		monsters.clear();          // reload monster to reset their hp
		char_effects.clear();      // reload effects to reset their position
		monster_effects.clear();   // reload effects to reset their position
		this->char_effects.push_back(std::move(std::make_unique<Entity>("image/sword.png", 200.f, 200.f, 0.8f, 0.8f)));
		this->char_effects.push_back(std::move(std::make_unique<Entity>("image/arrow.png", 200.f, 200.f, 0.5f, 0.5f)));
		this->char_effects.push_back(std::move(std::make_unique<Entity>("image/fireball.png", 200.f, 150.f, 0.5f, 0.5f)));
		this->char_effects.push_back(std::move(std::make_unique<Entity>("image/explosion.png", 0.5f)));
		this->char_effects.push_back(std::move(std::make_unique<Entity>("image/shield.png", 270.f, 210.f, 0.2f, 0.2f)));
		this->char_effects.push_back(std::move(std::make_unique<Entity>("image/thunder.png", 900.f, 150.f, 0.4f, 0.4f)));
		this->monster_effects.push_back(std::move(std::make_unique<Entity>("image/animal/stone.png", 700.f, 210.f, 0.3f, 0.3f)));
		this->monster_effects.push_back(std::move(std::make_unique<Entity>("image/animal/rock.png", 700.f, 200.f, 0.3f, 0.3f)));
		this->monster_effects.push_back(std::move(std::make_unique<Entity>("image/animal/wood.png", 600.f, 230.f, 0.4f, 0.4f)));
		this->monster_effects.push_back(std::move(std::make_unique<Entity>("image/animal/tree.png", 700.f, 170.f, 0.4f, 0.4f)));
		this->monster_effects.push_back(std::move(std::make_unique<Entity>("image/animal/water.png", 700.f, 160.f, 0.4f, 0.4f)));
		this->monster_effects.push_back(std::move(std::make_unique<Entity>("image/animal/tsunami.png", 700.f, 180.f, 0.3f, 0.3f)));
		this->monster_effects.push_back(std::move(std::make_unique<Entity>("image/explosion.png", 0.5f)));	
		loadMonsters();
		setMonsters();
		init_text_shape();
	}

	void EventHandling(sf::RenderWindow& window, sf::Event& event, Player& player_update, gameMap& world_map) {

		if (!atk_select && !char_select && !shield) {
			if (event.key.scancode == sf::Keyboard::Scan::A && isBattle) {

				std::string atk_message = "You can choose basic attack or skills (if available).";
				update_message(atk_message, "Basic Attack (A)", chars[char_index]->getSkillName(1) + " (B)", chars[char_index]->getSkillName(2) + " (C)", chars[char_index]->getSkillName(3) + " (D)");
				atk_select = true;
			}
			else if (event.key.scancode == sf::Keyboard::Scan::D && isBattle) {

				std::string message = "Shield active. Damage is reduced by half.";
				update_message(message, "", "", "", "");
				shield = true;
				monster_turn = true;
			}
			else if (event.key.scancode == sf::Keyboard::Scan::C && isBattle) {

				std::string change_char = "Please select the character to change.\nDifferent monsters get more damage\nfrom specific character.\nFor example, archer deals more damage\nto sky monsters.";
				std::string not_available = "Not available since other two characters are dead.\n";
				switch (char_index) {

				case 0:
					if (chars[1]->getHp() > 0 && chars[2]->getHp() > 0)				
						update_message(change_char, "Archer (A)", "Mage (B)", "", "");
					else if (chars[1]->getHp() > 0 && chars[2]->getHp() <= 0)
						update_message(change_char, "Archer (A)", "", "", "");
					else if (chars[1]->getHp() <= 0 && chars[2]->getHp() > 0)
						update_message(change_char, "", "Mage (B)", "", "");
					else if (chars[1]->getHp() <= 0 && chars[2]->getHp() <= 0)
						update_message(not_available, "", "", "", "Back (B)");
					break;
				case 1:
					if (chars[0]->getHp() > 0 && chars[2]->getHp() > 0)
						update_message(change_char, "Swordsman (A)", "Mage (B)", "", "");
					else if (chars[0]->getHp() > 0 && chars[2]->getHp() <= 0)
						update_message(change_char, "Swordsman (A)", "", "", "");
					else if (chars[0]->getHp() <= 0 && chars[2]->getHp() > 0)
						update_message(change_char, "", "Mage (B)", "", "");
					else if (chars[0]->getHp() <= 0 && chars[2]->getHp() <= 0)
						update_message(not_available, "", "", "", "Back (B)");
					break;
				case 2:
					if (chars[0]->getHp() > 0 && chars[1]->getHp() > 0)
						update_message(change_char, "Swordsman (A)", "Archer (B)", "", "");
					else if (chars[0]->getHp() > 0 && chars[1]->getHp() <= 0)
						update_message(change_char, "Swordsman (A)", "", "", "");
					else if (chars[0]->getHp() <= 0 && chars[1]->getHp() > 0)
						update_message(change_char, "", "Archer (B)", "", "");
					else if (chars[0]->getHp() <= 0 && chars[1]->getHp() <= 0)
						update_message(not_available, "", "", "", "Back (B)");
					break;
				}
				char_select = true;
				clock.restart();
			}
			else if (event.key.scancode == sf::Keyboard::Scan::E) {

				if (!isBattle && !isLose) {   // Check if exit with stage clear; if yes, update to next level of difficulty
					if (_difficulty < 2) {    // prevent vector out of range error
						_difficulty += 1;
						world_map.setArenaPos(_difficulty);           // Set new location of arena entrance after stage clear
					}
					else
						world_map.setArenaPos(_difficulty + 1);       // Notify game clear
				}
				else
					world_map.setArenaPos(_difficulty);       // Reset location of arena entrance after exit or losing
				
				// Back to worldmap
				music.stop();
				reset_data();	// reset data no matter exit or stage clear
				player_update.setChars(chars);      // Update player information
				player_update.setStage(_difficulty);
				world_map.getPlayer().setPosition(0.f, 0.f);
				world_map.setBattleStatus(false);
				world_map.setDefaultView(window);   // change to world map's view
				return;
			}
		}
		else
			EventSelection(event);
	}

	void EventSelection(sf::Event& event) {

		if (atk_select) {
			std::string temp;
			if (event.key.scancode == sf::Keyboard::Scan::A && finish_turn) {
				temp = "You have used basic attack!\n";			
				update_message(temp, "", "", "", "");
				my_turn = true;
				finish_turn = false;
				char_damage_calculation();
			}
			else if (event.key.scancode == sf::Keyboard::Scan::B && finish_turn) {

				if (chars[char_index]->getSkillName(1) != "N/A") {
					temp = "You have used ";
					update_message(temp + chars[char_index]->getSkillName(1) + "!\n", "", "", "", "");
					isSkill = true;
					my_turn = true;
					finish_turn = false;
					char_damage_calculation();
				}
			}
			else if (event.key.scancode == sf::Keyboard::Scan::C && finish_turn) {

				if (chars[char_index]->getSkillName(2) != "N/A") {
					
					// Attack or Defence buff
					chars[char_index]->buff_active();
					atk_text.setString("Atk: " + std::to_string(chars[char_index]->getAttack()));
					def_text.setString("Def: " + std::to_string(chars[char_index]->getDefence()));
					temp = "You have used ";
					std::string temp2 = "No damage in this turn, but the buff will last until\nthe end of the battle.\n";
					update_message(temp + chars[char_index]->getSkillName(2) + "!\n" + temp2, "", "", "", "");					
					char_damage = 0;
					my_turn = true;
					finish_turn = false;
				}
			}
			else if (event.key.scancode == sf::Keyboard::Scan::D && finish_turn) {

				if (chars[char_index]->getSkillName(3) != "N/A") {
					
					// Hp Recovery
					chars[char_index]->HpRecovery();
					hp_text.setString("Hp: " + std::to_string(chars[char_index]->getHp()));
					temp = "You have used ";
					std::string temp2 = "No damage in this turn as hp recovery is chosen.\n";
					update_message(temp + chars[char_index]->getSkillName(3) + "!\n" + temp2, "", "", "", "");
					char_damage = 0;
					my_turn = true;				
					finish_turn = false;
				}
			}			
		}
		else if (char_select) {
			char_selection(event);
		}
	}

	void char_selection(sf::Event& event) {

		std::string temp;
		if (text1.getString() != "" && text2.getString() != "") {
			if (event.key.scancode == sf::Keyboard::Scan::A) {
				switch (char_index) {

				case 0:
					if (chars[1]->getHp() > 0) {
						char_index = 1;
						temp = "You have changed to Archer character!";
						char_text.setString(chars[char_index]->getName());
						char_text.setPosition(chars[char_index]->getSprite().getGlobalBounds().left 
							+ chars[char_index]->getSprite().getGlobalBounds().width / 2 - char_text.getGlobalBounds().width / 2, 350.f);  // archer
					}
					break;
				case 1:
					if (chars[0]->getHp() > 0) {
						char_index = 0;
						temp = "You have changed to Swordsman character!";
						char_text.setString(chars[char_index]->getName());
						char_text.setPosition(chars[char_index]->getSprite().getGlobalBounds().left
							+ chars[char_index]->getSprite().getGlobalBounds().width / 2 - char_text.getGlobalBounds().width / 2, 360.f);    // swordsman
					}
					break;
				case 2:
					if (chars[0]->getHp() > 0) {
						char_index = 0;
						temp = "You have changed to Swordsman character!";
						char_text.setString(chars[char_index]->getName());
						char_text.setPosition(chars[char_index]->getSprite().getGlobalBounds().left
							+ chars[char_index]->getSprite().getGlobalBounds().width / 2 - char_text.getGlobalBounds().width / 2, 360.f);    // swordsman
					}
					break;
				}
				atk_text.setString("Atk: " + std::to_string(chars[char_index]->getAttack()));   // Update attack status shown in messagebox for changed character
				def_text.setString("Def: " + std::to_string(chars[char_index]->getDefence()));  // Update defence status shown in messagebox for changed character
				hp_text.setString("Hp: " + std::to_string(chars[char_index]->getHp()));   // Update hp status shown in messagebox for changed character
				update_hp_bar();  // Set char hp bar for changed character
				init_string();
				temp.append("\n");
				temp.append(message.getString());
				update_message(temp);
				char_select = false;
			}
			else if (event.key.scancode == sf::Keyboard::Scan::B) {
				switch (char_index) {
				case 0:
					if (chars[2]->getHp() > 0) {
						char_index = 2;
						temp = "You have changed to Mage character!";
						char_text.setString(chars[char_index]->getName());
						char_text.setPosition(chars[char_index]->getSprite().getGlobalBounds().left
							+ chars[char_index]->getSprite().getGlobalBounds().width / 2 - char_text.getGlobalBounds().width / 2, 355.f);    // mage
					}
					break;
				case 1:
					if (chars[2]->getHp() > 0) {
						char_index = 2;
						temp = "You have changed to Mage character!";
						char_text.setString(chars[char_index]->getName());
						char_text.setPosition(chars[char_index]->getSprite().getGlobalBounds().left
							+ chars[char_index]->getSprite().getGlobalBounds().width / 2 - char_text.getGlobalBounds().width / 2, 355.f);    // mage
					}
					break;
				case 2:
					if (chars[1]->getHp() > 0) {
						char_index = 1;
						temp = "You have changed to Archer character!";
						char_text.setString(chars[char_index]->getName());
						char_text.setPosition(chars[char_index]->getSprite().getGlobalBounds().left
							+ chars[char_index]->getSprite().getGlobalBounds().width / 2 - char_text.getGlobalBounds().width / 2, 350.f);  // archer
					}
					break;
				}
				atk_text.setString("Atk: " + std::to_string(chars[char_index]->getAttack()));   // Update attack status shown in messagebox for changed character
				def_text.setString("Def: " + std::to_string(chars[char_index]->getDefence()));  // Update defence status shown in messagebox for changed character
				hp_text.setString("Hp: " + std::to_string(chars[char_index]->getHp()));   // Update hp status shown in messagebox for changed character
				update_hp_bar();  // Set char hp bar for changed character
				init_string();
				temp.append("\n");
				temp.append(message.getString());
				update_message(temp);
				char_select = false;
			}
		}
		else if (text1.getString() != "" && text2.getString() == "") {
			if (event.key.scancode == sf::Keyboard::Scan::A) {
				switch (char_index) {

				case 0:
					if (chars[1]->getHp() > 0) {
						char_index = 1;
						temp = "You have changed to Archer character!";
						char_text.setString(chars[char_index]->getName());
						char_text.setPosition(chars[char_index]->getSprite().getGlobalBounds().left
							+ chars[char_index]->getSprite().getGlobalBounds().width / 2 - char_text.getGlobalBounds().width / 2, 350.f);  // archer
					}
					break;
				case 1:
					if (chars[0]->getHp() > 0) {
						char_index = 0;
						temp = "You have changed to Swordsman character!";
						char_text.setString(chars[char_index]->getName());
						char_text.setPosition(chars[char_index]->getSprite().getGlobalBounds().left 
							+ chars[char_index]->getSprite().getGlobalBounds().width / 2 - char_text.getGlobalBounds().width / 2, 360.f);    // swordsman
					}
					break;
				case 2:
					if (chars[0]->getHp() > 0) {
						char_index = 0;
						temp = "You have changed to Swordsman character!";
						char_text.setString(chars[char_index]->getName());
						char_text.setPosition(chars[char_index]->getSprite().getGlobalBounds().left
							+ chars[char_index]->getSprite().getGlobalBounds().width / 2 - char_text.getGlobalBounds().width / 2, 360.f);    // swordsman
					}
					break;
				}
				atk_text.setString("Atk: " + std::to_string(chars[char_index]->getAttack()));   // Update attack status shown in messagebox for changed character
				def_text.setString("Def: " + std::to_string(chars[char_index]->getDefence()));  // Update defence status shown in messagebox for changed character
				hp_text.setString("Hp: " + std::to_string(chars[char_index]->getHp()));   // Update hp status shown in messagebox for changed character
				update_hp_bar();  // Set char hp bar for changed character
				init_string();
				temp.append("\n");
				temp.append(message.getString());
				update_message(temp);
				char_select = false;
			}
		}
		else if (text1.getString() == "" && text2.getString() != "") {
			if (event.key.scancode == sf::Keyboard::Scan::B) {
				switch (char_index) {
				case 0:
					if (chars[2]->getHp() > 0) {
						char_index = 2;
						temp = "You have changed to Mage character!";
						char_text.setString(chars[char_index]->getName());
						char_text.setPosition(chars[char_index]->getSprite().getGlobalBounds().left
							+ chars[char_index]->getSprite().getGlobalBounds().width / 2 - char_text.getGlobalBounds().width / 2, 355.f);    // mage
					}
					break;
				case 1:
					if (chars[2]->getHp() > 0) {
						char_index = 2;
						temp = "You have changed to Mage character!";
						char_text.setString(chars[char_index]->getName());
						char_text.setPosition(chars[char_index]->getSprite().getGlobalBounds().left 
							+ chars[char_index]->getSprite().getGlobalBounds().width / 2 - char_text.getGlobalBounds().width / 2, 355.f);    // mage
					}
					break;
				case 2:
					if (chars[1]->getHp() > 0) {
						char_index = 1;
						temp = "You have changed to Archer character!";
						char_text.setString(chars[char_index]->getName());
						char_text.setPosition(chars[char_index]->getSprite().getGlobalBounds().left
							+ chars[char_index]->getSprite().getGlobalBounds().width / 2 - char_text.getGlobalBounds().width / 2, 350.f);  // archer
					}
					break;
				}
				atk_text.setString("Atk: " + std::to_string(chars[char_index]->getAttack()));   // Update attack status shown in messagebox for changed character
				def_text.setString("Def: " + std::to_string(chars[char_index]->getDefence()));  // Update defence status shown in messagebox for changed character
				hp_text.setString("Hp: " + std::to_string(chars[char_index]->getHp()));   // Update hp status shown in messagebox for changed character
				update_hp_bar();  // Set char hp bar for changed character
				init_string();
				temp.append("\n");
				temp.append(message.getString());
				update_message(temp);
				char_select = false;
			}
		}
		else if (text1.getString() == "" && text2.getString() == "") {
			if (event.key.scancode == sf::Keyboard::Scan::B) {

				init_string();
				char_select = false;
			}
		}
	}

	void char_damage_calculation() {

		int char_dmg = 0;

		if (!isSkill) {
			// Basic attack
			if (chars[char_index]->getAttack() - monsters[_difficulty][monster_index]->getDefence() > 0)  // Check if negative damage
				char_dmg = chars[char_index]->getAttack() - monsters[_difficulty][monster_index]->getDefence();
		}
		else {   
			// Attack skill
			if (char_dmg = chars[char_index]->getAttack() + chars[char_index]->getSkill(1).getAtk() - monsters[_difficulty][monster_index]->getDefence() > 0)  // Check if negative damage
				char_dmg = chars[char_index]->getAttack() + chars[char_index]->getSkill(1).getAtk() - monsters[_difficulty][monster_index]->getDefence();
		}

		// Final damage adjustment according to character and monster type
		if (char_index == 0 && monsters[_difficulty][monster_index]->getType() == "Land")
			char_dmg *= 1.5;
		else if (char_index == 1 && monsters[_difficulty][monster_index]->getType() == "Sky")
			char_dmg *= 1.5;
		if (char_index == 2 && monsters[_difficulty][monster_index]->getType() == "Sea")
			char_dmg *= 1.5;

		char_damage = static_cast<int>(char_dmg);
		monsters[_difficulty][monster_index]->damage_taken(char_damage);
	}

	void monster_damage_calculation() {

		int monster_dmg = 0;

		if (monsters[_difficulty][monster_index]->getAttack() - chars[char_index]->getDefence() > 0) {     // Check for negative damage
			monster_dmg = monsters[_difficulty][monster_index]->getAttack() - chars[char_index]->getDefence();
			if (shield)
				monster_dmg *= 0.5;  // Half the damage if the player chose defence with sheild active
		}

		monster_damage = static_cast<int>(monster_dmg);
		chars[char_index]->damage_taken(monster_damage);
	}

	void init_string() {

		message.setString("Please select your operation.");
		text1.setString("Attack (A)");
		text2.setString("Defence (D)");
		text3.setString("Character (C)");
		text4.setString("Exit (E)");
	}

	void init_text_shape() {

		// Initialize font and texts
		if (!font.loadFromFile("arialnarrow.ttf"))
			return;

		init_string();  // Initialize string for repeatable usage
		std::string init_message = 
		"Welcome to Arena Battle.\nPlease select your operation.\nFor example, press key 'A' of your keyboard\nis equivalent to Attack (A),\npress key 'C' for change of character and so on.";
		message.setString(init_message);
		message.setFont(font);
		message.setCharacterSize(24);
		message.setFillColor(sf::Color::Black);
		message.setPosition(340.f, 420.f);
		
		char_text.setString(chars[char_index]->getName());
		char_text.setFont(font);
		char_text.setCharacterSize(20);
		char_text.setFillColor(sf::Color::Black);
		char_text.setPosition(chars[char_index]->getSprite().getGlobalBounds().left 
		+ chars[char_index]->getSprite().getGlobalBounds().width / 2 - char_text.getGlobalBounds().width / 2, 350.f);  // archer
		//char_text.setPosition(chars[char_index]->getSprite().getGlobalBounds().left 
		//+ chars[char_index]->getSprite().getGlobalBounds().width / 2 - char_text.getGlobalBounds().width / 2, 360.f);    // swordsman
		//char_text.setPosition(chars[char_index]->getSprite().getGlobalBounds().left 
		//+ chars[char_index]->getSprite().getGlobalBounds().width / 2 - char_text.getGlobalBounds().width / 2, 355.f);    // mage

		monster_text.setString(monsters[_difficulty][monster_index]->getName());
		monster_text.setFont(font);
		monster_text.setCharacterSize(20);
		monster_text.setFillColor(sf::Color::Black);
		monster_text.setPosition(monsters[_difficulty][monster_index]->getSprite().getGlobalBounds().left
			+ monsters[_difficulty][monster_index]->getSprite().getGlobalBounds().width / 2 - monster_text.getGlobalBounds().width / 2, 360.f);

		hp_text.setString("Hp: " + std::to_string(chars[char_index]->getFullHp()));
		hp_text.setFont(font);
		hp_text.setCharacterSize(20);
		hp_text.setFillColor(sf::Color::Black);
		hp_text.setPosition(260.f, 420.f);

		atk_text.setString("Atk: " + std::to_string(chars[char_index]->getFullAttack()));
		atk_text.setFont(font);
		atk_text.setCharacterSize(20);
		atk_text.setFillColor(sf::Color::Black);
		atk_text.setPosition(260.f, 450.f);

		def_text.setString("Def: " + std::to_string(chars[char_index]->getFullDefence()));
		def_text.setFont(font);
		def_text.setCharacterSize(20);
		def_text.setFillColor(sf::Color::Black);
		def_text.setPosition(260.f, 480.f);

		text1.setFont(font);
		text1.setCharacterSize(22);
		text1.setFillColor(sf::Color::Black);

		text2.setFont(font);
		text2.setCharacterSize(22);
		text2.setFillColor(sf::Color::Black);

		text3.setFont(font);
		text3.setCharacterSize(22);
		text3.setFillColor(sf::Color::Black);

		text4.setFont(font);
		text4.setCharacterSize(22);
		text4.setFillColor(sf::Color::Black);


		// Initialize shapes
		messagebox.setSize(sf::Vector2f(1180.f, 190.f));
		messagebox.setPosition(10.f, 400.f);
		messagebox.setFillColor(sf::Color(255, 255, 255, 128));
		messagebox.setOutlineThickness(3.f);
		messagebox.setOutlineColor(sf::Color::Black);

		textbox1.setSize(sf::Vector2f(150.f, 50.f));
		textbox1.setPosition(800.f, 430.f);
		textbox1.setFillColor(sf::Color::Red);
		textbox1.setOutlineThickness(1.f);
		textbox1.setOutlineColor(sf::Color::Black);

		textbox2.setSize(sf::Vector2f(150.f, 50.f));
		textbox2.setPosition(1000.f, 430.f);
		textbox2.setFillColor(sf::Color::Green);
		textbox2.setOutlineThickness(1.f);
		textbox2.setOutlineColor(sf::Color::Black);

		textbox3.setSize(sf::Vector2f(150.f, 50.f));
		textbox3.setPosition(800.f, 510.f);
		textbox3.setFillColor(sf::Color::Yellow);
		textbox3.setOutlineThickness(1.f);
		textbox3.setOutlineColor(sf::Color::Black);

		textbox4.setSize(sf::Vector2f(150.f, 50.f));
		textbox4.setPosition(1000.f, 510.f);
		textbox4.setFillColor(sf::Color::White);
		textbox4.setOutlineThickness(1.f);
		textbox4.setOutlineColor(sf::Color::Black);

		char_hp_bar.setSize(sf::Vector2f(130.f, 10.f));
		char_hp_bar.setScale(1.f, 1.f);
		char_hp_bar.setPosition(90.f, 130.f);
		char_hp_bar.setFillColor(sf::Color::Green);
		char_hp_bar.setOutlineThickness(1.f);
		char_hp_bar.setOutlineColor(sf::Color::Black);

		monster_hp_bar.setSize(sf::Vector2f(250.f, 10.f));
		monster_hp_bar.setScale(1.f, 1.f);
		monster_hp_bar.setPosition(monsters[_difficulty][monster_index]->getSprite().getGlobalBounds().left, monsters[_difficulty][monster_index]->getSprite().getGlobalBounds().top - 20.f);
		monster_hp_bar.setFillColor(sf::Color::Green);
		monster_hp_bar.setOutlineThickness(1.f);
		monster_hp_bar.setOutlineColor(sf::Color::Black);
	}

	void update_message(StringOnly auto m, StringOnly auto t1, StringOnly auto t2, StringOnly auto t3, StringOnly auto t4) {

		message.setString(m);
		text1.setString(t1);
		text2.setString(t2);
		text3.setString(t3);
		text4.setString(t4);
	}

	void update_message(StringOnly auto& m) {

		message.setString(m);
	}

	void update_char_name(int index, StringOnly auto& new_name) {

		chars[index]->setName(new_name);
		char_text.setString(chars[char_index]->getName());
	}

	void draw_all(sf::RenderWindow& window) {

		draw_background(window);
		draw_messagebox(window);
		draw_monster(window);
		draw_chars(window);
		draw_atk_effect(window);
		draw_def_effect(window);
	}

	void draw_background(sf::RenderWindow& window) {
		
		if (_difficulty == 0)
			window.draw(background[0]->getSprite());	
		else if (_difficulty == 1)
			window.draw(background[1]->getSprite());
		else if (_difficulty == 2)
			window.draw(background[2]->getSprite());
	}

	void draw_chars(sf::RenderWindow& window) {
	
		if (char_index == 0) {
			window.draw(chars[0]->getSprite());
			window.draw(char_messagebox[0]->getSprite());
		}
		else if (char_index == 1) {
			window.draw(chars[1]->getSprite());
			window.draw(char_messagebox[1]->getSprite());
		}
		else if (char_index == 2) {
			window.draw(chars[2]->getSprite());
			window.draw(char_messagebox[2]->getSprite());
		}
	}

	void draw_monster(sf::RenderWindow& window) {
		window.draw(monsters[_difficulty][monster_index]->getSprite());
	}

	void stage_clear() {

		isBattle = false;
		monster_hp_bar.setScale(0.f, 1.f);  // Make it invisible
		std::string temp = "Congrats!\nYou have cleared the Arena ";
		std::string easy = "(Easy).\nYou can challenge the next stage now.";
		std::string medium = "(Medium).\nYou can challenge the next stage now.";
		std::string hard = "(Hard).\nGame clear! Thanks for playing.";
		std::stringstream ss;
		switch (_difficulty) {
		case 0:
			ss << temp << easy;
			break;
		case 1:
			ss << temp << medium;
			break;
		case 2:
			ss << temp << hard;
			break;
		}
		update_message(ss.str(), "", "", "", "Exit (E)");
	}

	void defeat_team() {

		isBattle = false;
		isLose = true;
		char_hp_bar.setScale(0.f, 1.f);  // Make it invisible
		std::string defeat = "Your team is totally defeated.\nPress exit to start again.";
		update_message(defeat, "", "", "", "Exit (E)");
	}

	void update_hp_bar() {

		float scale = 1.f;
		scale = static_cast<float>(chars[char_index]->getHp()) / static_cast<float>(chars[char_index]->getFullHp());

		if (scale < 0.f)
			scale = 0.f;

		char_hp_bar.setScale(scale, 1.f);
	}

	void collision_result(sf::Sprite& s, sf::RenderWindow& window) {

		if (my_turn) {

			if (s.getGlobalBounds().intersects(monsters[_difficulty][monster_index]->getSprite().getGlobalBounds())) {
				char_effects[3]->getSprite().setPosition(s.getGlobalBounds().left + s.getGlobalBounds().width / 2,
					monsters[_difficulty][monster_index]->getSprite().getGlobalBounds().top);

				// Set battle damage message and hp bar once
				if (!set_message_hp) {

					std::string prev_message = message.getString();
					std::string temp = "You have dealt ";
					std::string temp2 = " damage to the opponent!";
					std::stringstream ss;
					if (monsters[_difficulty][monster_index]->getHp() > 0)
						ss << prev_message << temp << char_damage << temp2;
					else {
						std::string monster_name = monsters[_difficulty][monster_index]->getName();
						std::string defeat = " is defeated.";
						ss << prev_message << temp << char_damage << temp2 << "\n" << monster_name << defeat;
					}
					message.setString(ss.str());

					// Adjust monster hp bar
					float scale = static_cast<float>(monsters[_difficulty][monster_index]->getHp()) / static_cast<float>(monsters[_difficulty][monster_index]->getFullHp());
					if (scale < 0.f)
						scale = 0.f;
					monster_hp_bar.setScale(scale, 1.f);

					set_message_hp = true;
				}			

				if (clock.getElapsedTime().asSeconds() < 0.5f) {
					window.draw(char_effects[3]->getSprite());  // draw explosion
					if (isSkill)
						window.draw(char_effects[5]->getSprite());
				}
				else {
					// Check if the monster is dead
					if (monsters[_difficulty][monster_index]->getHp() <= 0) {    
						atk_select = false;                                          
						monster_turn = false;
						finish_turn = true;
						
						// Check if the stage is cleared
						if (monsters[_difficulty].size() - 1 == monster_index) {
							stage_clear();    // stage clear
						}
						else {

							init_string();

							// Check if monster drop skills, weapons and armours
							if (monsters[_difficulty][monster_index]->drop(_difficulty)) {

								std::string char_awarded;

								if (monsters[_difficulty][monster_index]->getType() == "Land") {

									// Using Ranges to filter the skill/weapon/armour belongs to swordsman
									for (auto skill_swd : skill_list[_difficulty] | std::views::filter([](skill s) {return s.getClass() == "swordsman";}))
										chars[0]->addSkill(skill_swd);

									for (auto& weapon_swd : weapon_list[_difficulty] | std::views::filter([](weapon w) {return w.getClass() == "swordsman";}))
										chars[0]->addWeapon(weapon_swd);
									
									for (auto& armour_swd : armour_list[_difficulty] | std::views::filter([](armour a) {return a.getClass() == "swordsman";}))
										chars[0]->addArmour(armour_swd);

									char_awarded = chars[0]->getName();
								}
								else if (monsters[_difficulty][monster_index]->getType() == "Sky") {
								
									// Using Ranges to filter the skill/weapon/armour belongs to archer
									for (auto skill_arc : skill_list[_difficulty] | std::views::filter([](skill s) {return s.getClass() == "archer";}))
										chars[1]->addSkill(skill_arc);

									for (auto& weapon_arc : weapon_list[_difficulty] | std::views::filter([](weapon w) {return w.getClass() == "archer";}))
										chars[1]->addWeapon(weapon_arc);

									for (auto& armour_arc : armour_list[_difficulty] | std::views::filter([](armour a) {return a.getClass() == "archer";}))
										chars[1]->addArmour(armour_arc);

									char_awarded = chars[1]->getName();							
								}
								else if (monsters[_difficulty][monster_index]->getType() == "Sea") {

									// Using Ranges to filter the skill/weapon/armour belongs to mage
									for (auto skill_mag : skill_list[_difficulty] | std::views::filter([](skill s) {return s.getClass() == "mage";}))
										chars[2]->addSkill(skill_mag);

									for (auto& weapon_mag : weapon_list[_difficulty] | std::views::filter([](weapon w) {return w.getClass() == "mage";}))
										chars[2]->addWeapon(weapon_mag);

									for (auto& armour_mag : armour_list[_difficulty] | std::views::filter([](armour a) {return a.getClass() == "mage";}))
										chars[2]->addArmour(armour_mag);

									char_awarded = chars[2]->getName();
								}

								atk_text.setString("Atk: " + std::to_string(chars[char_index]->getAttack()));
								def_text.setString("Def: " + std::to_string(chars[char_index]->getDefence()));
								hp_text.setString("Hp: " + std::to_string(chars[char_index]->getHp()));

								// Update message for drops from monster
								std::string temp = "You have dropped skill, weapon and armour from ";
								std::string temp2 = "All of them are equipped automatically.\n";
								std::string sum = temp + "\n" + monsters[_difficulty][monster_index]->getName() + " for your character: " + 
									char_awarded + ".\n" + temp2 + message.getString();
								update_message(sum);
							}

							monster_index += 1;     // Replace next monster from the vector
							monster_hp_bar.setScale(1.f, 1.f);  // Reset hp bar
							monster_text.setString(monsters[_difficulty][monster_index]->getName());       // Set name text for next monster
							monster_text.setPosition(monsters[_difficulty][monster_index]->getSprite().getGlobalBounds().left
								+ monsters[_difficulty][monster_index]->getSprite().getGlobalBounds().width / 2 - monster_text.getGlobalBounds().width / 2, 360.f);
							monster_hp_bar.setPosition(monsters[_difficulty][monster_index]->getSprite().getGlobalBounds().left, // Set hp bar for next monster
								monsters[_difficulty][monster_index]->getSprite().getGlobalBounds().top - 10.f);
						}				
					}
					else {
						monster_turn = true;
					}
					isSkill = false;
					my_turn = false;  // finish your turn
					set_message_hp = false;
					s.setPosition(200.f, chars[char_index]->getSprite().getGlobalBounds().top 
						+ chars[char_index]->getSprite().getGlobalBounds().height / 2 
						- s.getGlobalBounds().height / 2);
					sf::sleep(sf::seconds(0.5));
				}
			}
			else {
				s.move(3.f, 0.f);
				clock.restart();
			}
		}
		else if (monster_turn) {

			if (s.getGlobalBounds().intersects(chars[char_index]->getSprite().getGlobalBounds())) {
				monster_effects[6]->getSprite().setPosition(s.getGlobalBounds().left - s.getGlobalBounds().width,  
					chars[char_index]->getSprite().getGlobalBounds().top);
				
				// Set battle damage message and hp bar once
				if (!set_message_hp) {

					monster_damage_calculation();
					std::string prev_message = message.getString();
					std::string monster_name = monsters[_difficulty][monster_index]->getName();
					std::string temp = " has dealt ";
					std::string temp2 = " damage to your character!";
					std::stringstream ss;
					if (chars[char_index]->getHp() > 0)
						ss << prev_message << "\n" << monster_name << temp << monster_damage << temp2;
					else {
						std::string defeat = "Your character is defeated.\nSwitching to another character by default.";
						ss << prev_message << "\n" << monster_name << temp << monster_damage << temp2 << "\n" << defeat;
					}
					message.setString(ss.str());

					// Adjust char hp bar
					update_hp_bar();							

					// Update the hp status shown in messagebox
					if (chars[char_index]->getHp() > 0)
						hp_text.setString("Hp: " + std::to_string(chars[char_index]->getHp()));   
					else
						hp_text.setString("Hp: 0");

					set_message_hp = true;
				}

				// draw explosion effect with delay
				if (clock.getElapsedTime().asSeconds() < 0.5f) {
					window.draw(monster_effects[6]->getSprite());  
				}
				else {
					monster_turn = false;  // finish monster turn
					atk_select = false;
					set_message_hp = false;
					shield = false;
					finish_turn = true;
					init_string();
					s.setPosition(monsters[_difficulty][monster_index]->getSprite().getGlobalBounds().left - s.getGlobalBounds().width / 2,
						monsters[_difficulty][monster_index]->getSprite().getGlobalBounds().top
						+ monsters[_difficulty][monster_index]->getSprite().getGlobalBounds().height / 2
						- s.getGlobalBounds().height / 4);
					sf::sleep(sf::seconds(0.5));

					// Check if character dead and if yes, change to other character automatically
					if (!(chars[char_index]->getHp() > 0)) {   					
						if (char_index == 0) {               
							if (chars[1]->getHp() > 0) {
								char_index = 1;
								char_text.setString(chars[char_index]->getName());
								char_text.setPosition(chars[char_index]->getSprite().getGlobalBounds().left
									+ chars[char_index]->getSprite().getGlobalBounds().width / 2 - char_text.getGlobalBounds().width / 2, 350.f);  // archer
							}
							else if (chars[2]->getHp() > 0) {
								char_index = 2;
								char_text.setString(chars[char_index]->getName());
								char_text.setPosition(chars[char_index]->getSprite().getGlobalBounds().left
									+ chars[char_index]->getSprite().getGlobalBounds().width / 2 - char_text.getGlobalBounds().width / 2, 355.f);    // mage
							}
							else {
								defeat_team();    // all characters are defeated
								return;
							}
						}
						else if (char_index == 1) {
							if (chars[2]->getHp() > 0) {
								char_index = 2;
								char_text.setString(chars[char_index]->getName());
								char_text.setPosition(chars[char_index]->getSprite().getGlobalBounds().left
									+ chars[char_index]->getSprite().getGlobalBounds().width / 2 - char_text.getGlobalBounds().width / 2, 355.f);    // mage
							}
							else if (chars[0]->getHp() > 0) {
								char_index = 0;
								char_text.setString(chars[char_index]->getName());
								char_text.setPosition(chars[char_index]->getSprite().getGlobalBounds().left
									+ chars[char_index]->getSprite().getGlobalBounds().width / 2 - char_text.getGlobalBounds().width / 2, 360.f);    // swordsman
							}
							else {
								defeat_team();    // all characters are defeated
								return;
							}
						}
						else if (char_index == 2) {
							if (chars[1]->getHp() > 0) {
								char_index = 1;
								char_text.setString(chars[char_index]->getName());
								char_text.setPosition(chars[char_index]->getSprite().getGlobalBounds().left
									+ chars[char_index]->getSprite().getGlobalBounds().width / 2 - char_text.getGlobalBounds().width / 2, 350.f);  // archer
							}
							else if (chars[0]->getHp() > 0) {
								char_index = 0;
								char_text.setString(chars[char_index]->getName());
								char_text.setPosition(chars[char_index]->getSprite().getGlobalBounds().left
									+ chars[char_index]->getSprite().getGlobalBounds().width / 2 - char_text.getGlobalBounds().width / 2, 360.f);    // swordsman
							}
							else {
								defeat_team();    // all characters are defeated
								return;
							}
						}
						atk_text.setString("Atk: " + std::to_string(chars[char_index]->getAttack()));   // Update attack status shown in messagebox for next character
						def_text.setString("Def: " + std::to_string(chars[char_index]->getDefence()));  // Update defence status shown in messagebox for next character
						hp_text.setString("Hp: " + std::to_string(chars[char_index]->getHp()));   // Update hp status shown in messagebox for next character
						update_hp_bar();   // Set char hp bar for next character
					}					
				}
			}
			else {
				s.move(-3.f, 0.f);
				clock.restart();
			}
		}
	}

	void draw_atk_effect(sf::RenderWindow& window) {

		if (my_turn) {
			if (char_index == 0) {
				window.draw(char_effects[0]->getSprite());
				collision_result(char_effects[0]->getSprite(), window);
			}
			else if (char_index == 1) {
				window.draw(char_effects[1]->getSprite());
				collision_result(char_effects[1]->getSprite(), window);
			}
			else if (char_index == 2) {
				window.draw(char_effects[2]->getSprite());
				collision_result(char_effects[2]->getSprite(), window);
			}
		}
		else if (monster_turn) {

			if (!monsters[_difficulty][monster_index]->checkBoss()) {
				if (monsters[_difficulty][monster_index]->getType() == "Land") {

					window.draw(monster_effects[0]->getSprite());
					collision_result(monster_effects[0]->getSprite(), window);
				}
				else if (monsters[_difficulty][monster_index]->getType() == "Sky") {

					window.draw(monster_effects[2]->getSprite());
					collision_result(monster_effects[2]->getSprite(), window);
				}
				else if (monsters[_difficulty][monster_index]->getType() == "Sea") {

					window.draw(monster_effects[4]->getSprite());
					collision_result(monster_effects[4]->getSprite(), window);
				}
			}
			else {
				if (monsters[_difficulty][monster_index]->getName() == "Bear" || monsters[_difficulty][monster_index]->getName() == "Elephant") {

					window.draw(monster_effects[1]->getSprite());
					collision_result(monster_effects[1]->getSprite(), window);
				}
				else if (monsters[_difficulty][monster_index]->getName() == "Eagle") {

					window.draw(monster_effects[3]->getSprite());
					collision_result(monster_effects[3]->getSprite(), window);
				}
				else if (monsters[_difficulty][monster_index]->getName() == "Orca") {
					window.draw(monster_effects[5]->getSprite());
					collision_result(monster_effects[5]->getSprite(), window);
				}

			}

		}
	}

	void draw_def_effect(sf::RenderWindow& window) {

		if (shield)
			window.draw(char_effects[4]->getSprite());
	}

	void text_center(sf::Text& text, sf::RectangleShape& textbox) {

		float x = textbox.getGlobalBounds().left + textbox.getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2;
		float y = textbox.getGlobalBounds().top + textbox.getGlobalBounds().height / 2 - text.getGlobalBounds().height / 2 - 5.f;
		text.setPosition(x, y);
	}

	void draw_messagebox(sf::RenderWindow& window) {
	
		text_center(text1, textbox1);		
		text_center(text2, textbox2);		
		text_center(text3, textbox3);
		text_center(text4, textbox4);

		window.draw(messagebox);
		window.draw(message);
		window.draw(textbox1);
		window.draw(text1);
		window.draw(textbox2);
		window.draw(text2);
		window.draw(textbox3);
		window.draw(text3);
		window.draw(textbox4);
		window.draw(text4);
		window.draw(char_text);
		window.draw(monster_text);
		window.draw(hp_text);
		window.draw(atk_text);
		window.draw(def_text);
		window.draw(char_hp_bar);
		window.draw(monster_hp_bar);
	}

};