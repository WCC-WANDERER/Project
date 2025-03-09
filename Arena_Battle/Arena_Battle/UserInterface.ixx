module;

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

export module UserInterface;
import <iostream>;
import <concepts>;
import Player;
import gameMap;
import Arena;


template<typename T>
concept StringInput = requires (T event_enter)
{
	{ sf::String(event_enter.text.unicode) } -> std::convertible_to<std::string>;  // std::string type is accepted for character's name
};


export class UserInterface {
	
protected:
	sf::Texture bg_texture, lass_texture, lire_texture, arme_texture;
	sf::Sprite background, lass, lire, arme;
	sf::Font font;
	sf::Text GameName, StartGame, LoadSave, HowtoPlay, Quit, team_info, team_description;
	sf::Text char_class, char_description, char_name, char_atk, char_def, char_hp, previous, next, OK;
	sf::RectangleShape textbox1, textbox2, textbox3, textbox4, team_box, previous_box, next_box, OK_box;
	sf::Music music;
	std::vector<std::string> char_names;   // swordsman, archer, mage
	int char_index = 0;
	bool isMusic = true;
	bool isHomepage = true;
	bool isTeamInfo = false;
	bool isCreateTeam = false;
	bool goBack = false;
	bool loadSave = false;

public:

	UserInterface() {

		InitialState();
	}
	~UserInterface() = default;

	void InitialState() {

		Set_Texture(bg_texture, "image/background.png");
		Set_Texture(lass_texture, "image/lass.png");
		Set_Texture(lire_texture, "image/lire.png");
		Set_Texture(arme_texture, "image/arme.png");
		background.setTexture(bg_texture);
		lass.setTexture(lass_texture);
		lass.setScale(0.7f, 0.7f);
		lass.setPosition(400.f - lass.getGlobalBounds().width / 2, 400.f - lass.getGlobalBounds().height / 2);
		lire.setTexture(lire_texture);
		lire.setScale(0.75f, 0.75f);
		lire.setPosition(340.f - lass.getGlobalBounds().width / 2, 400.f - lass.getGlobalBounds().height / 2);
		arme.setTexture(arme_texture);
		arme.setScale(0.75f, 0.75f);
		arme.setPosition(340.f - lass.getGlobalBounds().width / 2, 400.f - lass.getGlobalBounds().height / 2);
		Set_Music(music, "music/start.mp3");
		std::string swd_name, arc_name, mag_name;
		char_names.push_back(swd_name);
		char_names.push_back(arc_name);
		char_names.push_back(mag_name);
		init_text_shape();
	}

	void Set_Texture(sf::Texture& texture, const std::string& texture_name) {
		if (!texture.loadFromFile(texture_name))
			return;
		texture.setSmooth(true);
		return;
	}

	void Set_Music(sf::Music& music, const std::string& music_name) {
		if (!music.openFromFile(music_name))
			return;
		music.setVolume(50);
		music.setLoop(true);
	}

	void playMusic() {
		if (isMusic) {
			music.play();
			music.setPlayingOffset(sf::seconds(39));
			isMusic = false;
		}
	}

	void init_text_shape() {

		// Initialize font and texts
		if (!font.loadFromFile("arialnarrow.ttf"))
			return;

		GameName.setString("ARENA  BATTLE");
		GameName.setFont(font);
		GameName.setCharacterSize(180);
		GameName.setFillColor(sf::Color::Yellow);
		GameName.setStyle(sf::Text::Bold | sf::Text::Italic);
		GameName.setPosition(800.f - GameName.getGlobalBounds().width / 2, 30.f);

		StartGame.setString("Start New Game");
		StartGame.setFont(font);
		StartGame.setCharacterSize(60);
		StartGame.setFillColor(sf::Color::Black);

		LoadSave.setString("Load Previous Save");
		LoadSave.setFont(font);
		LoadSave.setCharacterSize(60);
		LoadSave.setFillColor(sf::Color::Black);

		HowtoPlay.setString("How to Play");
		HowtoPlay.setFont(font);
		HowtoPlay.setCharacterSize(60);
		HowtoPlay.setFillColor(sf::Color::Black);

		Quit.setString("Quit");
		Quit.setFont(font);
		Quit.setCharacterSize(60);
		Quit.setFillColor(sf::Color::Black);

		char_class.setString("Swordsman");
		char_class.setFont(font);
		char_class.setCharacterSize(60);
		char_class.setFillColor(sf::Color::Blue);
		char_class.setPosition(1200.f - char_class.getGlobalBounds().width / 2 - 150.f, 50.f);

		char_description.setString("Pure talent and techniques on using swords.\nDealing tons of damage on land monsters.\nPlease enter your character's name below.\n");
		char_description.setFont(font);
		char_description.setCharacterSize(30);
		char_description.setFillColor(sf::Color::Black);
		char_description.setPosition(char_class.getGlobalBounds().left, 150.f);

		char_name.setString("Name: ");
		char_name.setFont(font);
		char_name.setCharacterSize(30);
		char_name.setFillColor(sf::Color::Black);
		char_name.setPosition(char_class.getGlobalBounds().left, 300.f);

		char_atk.setString("Attack: 30");
		char_atk.setFont(font);
		char_atk.setCharacterSize(30);
		char_atk.setFillColor(sf::Color::Black);
		char_atk.setPosition(char_class.getGlobalBounds().left, 380.f);

		char_def.setString("Defence: 50");
		char_def.setFont(font);
		char_def.setCharacterSize(30);
		char_def.setFillColor(sf::Color::Black);
		char_def.setPosition(char_class.getGlobalBounds().left, 460.f);

		char_hp.setString("Health: 150");
		char_hp.setFont(font);
		char_hp.setCharacterSize(30);
		char_hp.setFillColor(sf::Color::Black);
		char_hp.setPosition(char_class.getGlobalBounds().left, 540.f);

		team_info.setString("Information");
		team_info.setFont(font);
		team_info.setCharacterSize(60);
		team_info.setStyle(sf::Text::Underlined);
		team_info.setFillColor(sf::Color::Black);
		team_info.setPosition(800.f - team_info.getGlobalBounds().width / 2, 60.f);
		
		std::string team_temp = "Let's start to create your team!\nYour team consists of three characters:\nSwordsman, Archer and Mage.\n";
		std::string team_temp2 = "You will name those characters and then start the game.\nAfter creation of your team, you will enter the world map of the game.\n";
		std::string team_temp3 = "In the world map, you can move by traditional W-A-S-D and enter the arena to start battle.\n";
		std::string team_temp4 = "There are three levels of difficulty for the arena: Easy, Medium and Hard.\n";
		std::string team_temp5 = "You can challenge the next level after clearing current level of the arena.\n";
		std::string team_temp6 = "Skills, weapons and armours are dropped during the battle and equipped automatically.\n";
		std::string team_temp7 = "Also, you can always quit the game with Escape key.\n";
		std::string team_temp8 = "Your progress will be saved(except inside of the arena) after closing or quitting the game.\n";
		team_description.setString(team_temp + team_temp2 + team_temp3 + team_temp4 + team_temp5 + team_temp6 + team_temp7 + team_temp8);
		team_description.setFont(font);
		team_description.setCharacterSize(30);
		team_description.setFillColor(sf::Color::Black);

		previous.setString("Previous");
		previous.setFont(font);
		previous.setCharacterSize(40);
		previous.setFillColor(sf::Color::Black);		

		next.setString("Next");
		next.setFont(font);
		next.setCharacterSize(40);
		next.setFillColor(sf::Color::Black);

		OK.setString("OK");
		OK.setFont(font);
		OK.setCharacterSize(40);
		OK.setFillColor(sf::Color::Black);


		// Initialize shapes
		textbox1.setSize(sf::Vector2f(600.f, 150.f));
		textbox1.setPosition(100.f, 300.f);
		textbox1.setFillColor(sf::Color(255, 255, 255, 128));
		textbox1.setOutlineThickness(5.f);
		textbox1.setOutlineColor(sf::Color::Black);

		textbox2.setSize(sf::Vector2f(600.f, 150.f));
		textbox2.setPosition(900.f, 300.f);
		textbox2.setFillColor(sf::Color(255, 255, 255, 128));
		textbox2.setOutlineThickness(5.f);
		textbox2.setOutlineColor(sf::Color::Black);

		textbox3.setSize(sf::Vector2f(600.f, 150.f));
		textbox3.setPosition(100.f, 550.f);
		textbox3.setFillColor(sf::Color(255, 255, 255, 128));
		textbox3.setOutlineThickness(5.f);
		textbox3.setOutlineColor(sf::Color::Black);

		textbox4.setSize(sf::Vector2f(600.f, 150.f));
		textbox4.setPosition(900.f, 550.f);
		textbox4.setFillColor(sf::Color(255, 255, 255, 128));
		textbox4.setOutlineThickness(5.f);
		textbox4.setOutlineColor(sf::Color::Black);

		previous_box.setSize(sf::Vector2f(200.f, 60.f));
		previous_box.setPosition(char_class.getGlobalBounds().left, 650.f);
		previous_box.setFillColor(sf::Color::Green);
		previous_box.setOutlineThickness(3.f);
		previous_box.setOutlineColor(sf::Color::Black);

		next_box.setSize(sf::Vector2f(200.f, 60.f));
		next_box.setPosition(1300.f, 650.f);
		next_box.setFillColor(sf::Color::Yellow);
		next_box.setOutlineThickness(3.f);
		next_box.setOutlineColor(sf::Color::Black);

		OK_box.setSize(sf::Vector2f(200.f, 60.f));
		OK_box.setPosition(800.f - OK_box.getGlobalBounds().width / 2, 600.f);
		OK_box.setFillColor(sf::Color::Red);
		OK_box.setOutlineThickness(3.f);
		OK_box.setOutlineColor(sf::Color::Black);

		team_box.setSize(sf::Vector2f(1200.f, 700.f));
		team_box.setPosition(800.f - team_box.getGlobalBounds().width / 2, 50.f);
		team_box.setFillColor(sf::Color(255, 255, 255, 128));
		team_box.setOutlineThickness(5.f);
		team_box.setOutlineColor(sf::Color::Black);

		team_description.setPosition(team_box.getGlobalBounds().left + 30.f, team_box.getGlobalBounds().top + 120.f);

		text_center(StartGame, textbox1);
		text_center(LoadSave, textbox2);
		text_center(HowtoPlay, textbox3);
		text_center(Quit, textbox4);
		text_center(previous, previous_box);
		text_center(next, next_box);
		text_center(OK, OK_box);
	}

	void text_center(sf::Text& text, sf::RectangleShape& textbox) {

		float x = textbox.getGlobalBounds().left + textbox.getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2;
		float y = textbox.getGlobalBounds().top + textbox.getGlobalBounds().height / 2 - text.getGlobalBounds().height / 2 - 10.f;
		text.setPosition(x, y);
	}

	void reset_data () {

		int char_index = 0;
		bool isMusic = true;
		bool isHomepage = true;
		bool isTeamInfo = false;
		bool isCreateTeam = false;
		bool goBack = false;
		bool loadSave = false;
		char_names.clear();
		std::string swd_name, arc_name, mag_name;
		char_names.push_back(swd_name);
		char_names.push_back(arc_name);
		char_names.push_back(mag_name);
		init_text_shape();
	}

	bool InitializeTeam(sf::RenderWindow& window) {

		if (isHomepage || CreateTeam(window) || isTeamInfo)
			return false;
		else
			return true;		
	}

	void UpdateName(StringInput auto& event) {

		if (event.text.unicode != '\b') 
			char_names[char_index] += sf::String(event.text.unicode);
		else {
			if (!char_names[char_index].empty())
				char_names[char_index].pop_back();
		}
	}

	void EventHandling(sf::RenderWindow& window, sf::Event& event, Player& player, gameMap& world_map, Arena& arena) {

		if (event.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
			sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

			// Main Menu
			if (isHomepage) {

				if (textbox1.getGlobalBounds().contains(worldPos))
				{
					isHomepage = false;
					isTeamInfo = true;
					team_description.setPosition(team_box.getGlobalBounds().left + 30.f, team_box.getGlobalBounds().top + 120.f);
				}
				else if (textbox2.getGlobalBounds().contains(worldPos))
				{
					isHomepage = false;
					isTeamInfo = true;
					loadSave = true;
				}
				else if (textbox3.getGlobalBounds().contains(worldPos))
				{
					isHomepage = false;
					isTeamInfo = true;
					goBack = true;
				}
				else if (textbox4.getGlobalBounds().contains(worldPos))
				{
					window.close();
				}
			}
			// Team Information Page
			else if (isTeamInfo) {

				if (OK_box.getGlobalBounds().contains(worldPos)) {
					
					isTeamInfo = false;

					if (goBack) {
						isHomepage = true;
						goBack = false;
					}
					else if (loadSave) {
						music.stop();
						player.LoadAll();      // Load data from save
						arena.loadSave(player);    // Load settings from player
						world_map.setArenaPos(player.getStage());    // Setting the arena position according to player's progress
						world_map.getPlayer().setPosition(0.f, 0.f);
						world_map.setBattleStatus(false);
						world_map.setDefaultView(window);
						window.setMouseCursorVisible(false);
						reset_data();
					}
					else
						isCreateTeam = true;					

					if (!char_names[0].empty() && !char_names[1].empty() && !char_names[2].empty()) {
						music.stop();						
						isCreateTeam = false;
						player.getCharacter(0)->setName(char_names[0]);     // Update the assigned name to player object
						player.getCharacter(1)->setName(char_names[1]);
						player.getCharacter(2)->setName(char_names[2]);
						arena.update_char_name(0, char_names[0]);       // Update the assigned name to characters in arena
						arena.update_char_name(1, char_names[1]);
						arena.update_char_name(2, char_names[2]);
						world_map.setDefaultView(window);
						window.setMouseCursorVisible(false);
						reset_data();
					}
				}
			}
			// Create Team of characters
			else if (isCreateTeam) {

				if (next_box.getGlobalBounds().contains(worldPos))
				{
					if (char_index < 2)
						char_index += 1;
					else
						isTeamInfo = true;					
				}
				else if (previous_box.getGlobalBounds().contains(worldPos))
				{
					if (char_index > 0)
						char_index -= 1;
					else
						isHomepage = true;
				}
			}
		}
	}
		
	bool CreateTeam(sf::RenderWindow& window) {

		if (isCreateTeam) {
			// Display the team
			if (char_index == 0) {
				char_class.setString("Swordsman");
				char_class.setFillColor(sf::Color::Blue);
				char_description.setString("Pure talent and techniques on using swords.\nDealing tons of damage on land monsters.\nPlease enter your character's name below.\n");
				char_atk.setString("Attack: 30");
				char_def.setString("Defence: 50");
				char_hp.setString("Health: 150");
			}
			else if (char_index == 1) {
				char_class.setString("Archer");
				char_class.setFillColor(sf::Color::Green);
				char_description.setString("Excellent arrows shooting penetrate everything.\nDealing tons of damage on sky monsters.\nPlease enter your character's name below.\n");
				char_atk.setString("Attack: 40");
				char_def.setString("Defence: 40");
				char_hp.setString("Health: 120");
			}
			else if (char_index == 2) {
				char_class.setString("Mage");
				char_class.setFillColor(sf::Color::Magenta);
				char_description.setString("Magnificent power to cast any spells.\nDealing tons of damage on sea monsters.\nPlease enter your character's name below.\n");
				char_atk.setString("Attack: 50");
				char_def.setString("Defence: 30");
				char_hp.setString("Health: 100");
			}

			// Define characters' name by user
			char_name.setString("Name: " + char_names[char_index]);
		}		
		return isCreateTeam;
	}

	void draw_all(sf::RenderWindow& window) {

		if (isHomepage) {
			window.draw(background);
			window.draw(textbox1);
			window.draw(textbox2);
			window.draw(textbox3);
			window.draw(textbox4);
			window.draw(GameName);
			window.draw(StartGame);
			window.draw(LoadSave);
			window.draw(HowtoPlay);
			window.draw(Quit);
		}
		else if (isTeamInfo) {
			window.draw(background);
			window.draw(team_box);
			window.draw(team_info);
			window.draw(team_description);
			window.draw(OK_box);
			window.draw(OK);
		}
		else if (isCreateTeam) {
			switch (char_index) {
			case 0:
				window.draw(lass);
				break;
			case 1:
				window.draw(lire);
				break;
			case 2:
				window.draw(arme);
				break;
			}
			window.draw(previous_box);
			window.draw(next_box);
			window.draw(char_description);
			window.draw(char_class);
			window.draw(char_name);
			window.draw(char_atk);
			window.draw(char_def);
			window.draw(char_hp);
			window.draw(previous);
			window.draw(next);
		}
	}
};