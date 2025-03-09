module;

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

export module gameMap;
export import :gameTile;

export class gameMap {

protected:
	float width;
	float height;
	float movement = 3.f;
	int stage = 0;
	sf::Texture player_left, player_right, arena_texture, clear_texture;
	sf::Sprite player, arena, game_clear;
	sf::Music music;
	sf::Font font;
	sf::Text text;
	sf::View view;
	std::vector<std::vector<std::unique_ptr<gameTile>>> tiles;
	std::vector<std::unique_ptr<gameTile>> bushes;
	bool isBattle = false;
	bool isClear = false;
	bool isMusic = true;

public:	
	gameMap() = default;
	gameMap(float window_width, float window_height) {
		this->width = window_width;
		this->height = window_height;
		this->view.setCenter(sf::Vector2f(window_width / 4, window_height / 4));
		this->view.setSize(sf::Vector2f(window_width / 2, window_height / 2));
		InitialState(); 
	}
	~gameMap() = default;

	void InitialState() {

		Set_Texture(player_right, "image/player.png");
		Set_Texture(player_left, "image/player2.png");
		Set_Texture(arena_texture, "image/arena.png");
		Set_Texture(clear_texture, "image/background.png");
		player.setTexture(player_right);
		player.setScale(0.35f, 0.35f);
		arena.setTexture(arena_texture);
		arena.setPosition(1200.f, 700.f);
		game_clear.setTexture(clear_texture);
		Set_Music(music, "music/worldmap.mp3");
		SetUpTiles();
		SetUpBushes();		
	}

	void Set_Texture(sf::Texture& texture, const std::string& texture_name) {
		if (!texture.loadFromFile(texture_name))
			return;
		texture.setSmooth(true);
		return;
	}

	void Set_Music(sf::Music& music, const std::string& music_name)
	{
		if (!music.openFromFile(music_name))
			return;
		music.setVolume(70);
		music.setLoop(true);
	}

	void SetUpTiles() {

		tiles.clear();

		std::vector<std::unique_ptr<gameTile>> firstrow;
		firstrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 0, 0, true, false)));
		firstrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 100, 0, true, false)));
		firstrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 200, 0, true, false)));
		firstrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 300, 0, true, false)));
		firstrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 400, 0, true, false)));
		firstrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 500, 0, true, false)));
		firstrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 600, 0, true, false)));
		firstrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 700, 0, true, false)));
		firstrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 800, 0, true, false)));
		firstrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 900, 0, true, false)));
		firstrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1000, 0, true, false)));
		firstrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1100, 0, true, false)));
		firstrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1200, 0, true, false)));
		firstrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1300, 0, true, false)));
		firstrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1400, 0, true, false)));
		firstrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1500, 0, true, false)));
		tiles.push_back(std::move(firstrow));

		std::vector<std::unique_ptr<gameTile>> secondrow;
		secondrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 0, 100, true, false)));
		secondrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 100, 100, true, false)));
		secondrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 200, 100, true, false)));
		secondrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 300, 100, true, false)));
		secondrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 400, 100, true, false)));
		secondrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 500, 100, true, false)));
		secondrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 600, 100, true, false)));
		secondrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 700, 100, true, false)));
		secondrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 800, 100, true, false)));
		secondrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 900, 100, true, false)));
		secondrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1000, 100, true, false)));
		secondrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1100, 100, true, false)));
		secondrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1200, 100, true, false)));
		secondrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1300, 100, true, false)));
		secondrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1400, 100, true, false)));
		secondrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1500, 100, true, false)));
		tiles.push_back(std::move(secondrow));

		std::vector<std::unique_ptr<gameTile>> thirdrow;
		thirdrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 0, 200, true, false)));
		thirdrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 100, 200, true, false)));
		thirdrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 200, 200, true, false)));
		thirdrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 300, 200, true, false)));
		thirdrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 400, 200, true, false)));
		thirdrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 500, 200, true, false)));
		thirdrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 600, 200, true, false)));
		thirdrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 700, 200, true, false)));
		thirdrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 800, 200, true, false)));
		thirdrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 900, 200, true, false)));
		thirdrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1000, 200, true, false)));
		thirdrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1100, 200, true, false)));
		thirdrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1200, 200, true, false)));
		thirdrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1300, 200, true, false)));
		thirdrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1400, 200, true, false)));
		thirdrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1500, 200, true, false)));
		tiles.push_back(std::move(thirdrow));

		std::vector<std::unique_ptr<gameTile>> fourthrow;
		fourthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 0, 300, true, false)));
		fourthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 100, 300, true, false)));
		fourthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 200, 300, true, false)));
		fourthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 300, 300, true, false)));
		fourthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 400, 300, true, false)));
		fourthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 500, 300, true, false)));
		fourthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 600, 300, true, false)));
		fourthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 700, 300, true, false)));
		fourthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 800, 300, true, false)));
		fourthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 900, 300, true, false)));
		fourthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1000, 300, true, false)));
		fourthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1100, 300, true, false)));
		fourthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1200, 300, true, false)));
		fourthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1300, 300, true, false)));
		fourthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1400, 300, true, false)));
		fourthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1500, 300, true, false)));
		tiles.push_back(std::move(fourthrow));

		std::vector<std::unique_ptr<gameTile>> fifthrow;
		fifthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 0, 400, true, false)));
		fifthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 100, 400, true, false)));
		fifthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 200, 400, true, false)));
		fifthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 300, 400, true, false)));
		fifthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 400, 400, true, false)));
		fifthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 500, 400, true, false)));
		fifthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 600, 400, true, false)));
		fifthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 700, 400, true, false)));
		fifthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 800, 400, true, false)));
		fifthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 900, 400, true, false)));
		fifthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1000, 400, true, false)));
		fifthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1100, 400, true, false)));
		fifthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1200, 400, true, false)));
		fifthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1300, 400, true, false)));
		fifthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1400, 400, true, false)));
		fifthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1500, 400, true, false)));
		tiles.push_back(std::move(fifthrow));

		std::vector<std::unique_ptr<gameTile>> sixthrow;
		sixthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 0, 500, true, false)));
		sixthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 100, 500, true, false)));
		sixthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 200, 500, true, false)));
		sixthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 300, 500, true, false)));
		sixthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 400, 500, true, false)));
		sixthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 500, 500, true, false)));
		sixthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 600, 500, true, false)));
		sixthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 700, 500, true, false)));
		sixthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 800, 500, true, false)));
		sixthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 900, 500, true, false)));
		sixthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1000, 500, true, false)));
		sixthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1100, 500, true, false)));
		sixthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1200, 500, true, false)));
		sixthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1300, 500, true, false)));
		sixthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1400, 500, true, false)));
		sixthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1500, 500, true, false)));
		tiles.push_back(std::move(sixthrow));

		std::vector<std::unique_ptr<gameTile>> seventhrow;
		seventhrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 0, 600, true, false)));
		seventhrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 100, 600, true, false)));
		seventhrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 200, 600, true, false)));
		seventhrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 300, 600, true, false)));
		seventhrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 400, 600, true, false)));
		seventhrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 500, 600, true, false)));
		seventhrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 600, 600, true, false)));
		seventhrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 700, 600, true, false)));
		seventhrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 800, 600, true, false)));
		seventhrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 900, 600, true, false)));
		seventhrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1000, 600, true, false)));
		seventhrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1100, 600, true, false)));
		seventhrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1200, 600, true, false)));
		seventhrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1300, 600, true, false)));
		seventhrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1400, 600, true, false)));
		seventhrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1500, 600, true, false)));
		tiles.push_back(std::move(seventhrow));

		std::vector<std::unique_ptr<gameTile>> eighthrow;
		eighthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 0, 700, true, false)));
		eighthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 100, 700, true, false)));
		eighthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 200, 700, true, false)));
		eighthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 300, 700, true, false)));
		eighthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 400, 700, true, false)));
		eighthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 500, 700, true, false)));
		eighthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 600, 700, true, false)));
		eighthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 700, 700, true, false)));
		eighthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 800, 700, true, false)));
		eighthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 900, 700, true, false)));
		eighthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1000, 700, true, false)));
		eighthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1100, 700, true, false)));
		eighthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1200, 700, true, false)));
		eighthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1300, 700, true, false)));
		eighthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1400, 700, true, false)));
		eighthrow.push_back(std::move(std::make_unique<gameTile>("image/grass.png", 1500, 700, true, false)));
		tiles.push_back(std::move(eighthrow));

	}

	void SetUpBushes() {

		bushes.clear();

		//bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 0, 300, false, false)));
		//bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 100, 300, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 200, 300, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 200, 300, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 200, 400, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 200, 500, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 200, 600, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 200, 700, false, false)));

		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 700, 0, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 700, 100, false, false)));
		//bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 700, 200, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 800, 100, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 900, 100, false, false)));
		//bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 1000, 200, false, false)));
		//bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 1100, 200, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 1200, 200, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 1300, 200, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 1400, 200, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 1500, 200, false, false)));

		//bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 1500, 700, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 1400, 600, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 1300, 600, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 1200, 600, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 1200, 500, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 1100, 400, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 1000, 400, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 900, 400, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 800, 400, false, false)));
		//bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 800, 400, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 700, 400, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 600, 400, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 500, 300, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 500, 400, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 500, 500, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 500, 600, false, false)));
		bushes.push_back(std::move(std::make_unique<gameTile>("image/bush.png", 500, 700, false, false)));

	}

	void playMusic() {
		if (isMusic) {
			music.play();
			music.setPlayingOffset(sf::seconds(39));
			isMusic = false;
		}
	}

	void setDefaultView(sf::RenderWindow& window) { 
		view.setCenter(sf::Vector2f(width / 4, height / 4));
		view.setSize(sf::Vector2f(width / 2, height / 2));
		window.setView(view); 
	}

	void setArenaPos(int level) {      
		stage = level;      
		if (stage == 0)	   // Set new location of arena entrance according to level of difficulty
			arena.setPosition(1200.f, 700.f);
		else if (stage == 1)
			arena.setPosition(1500.f, 0.f);
		else if (stage == 2)
			arena.setPosition(0.f, 700.f);
		else
			arena.setPosition(2000.f, 2000.f);		
	}

	void setBattleStatus(bool status) { isBattle = status; isMusic = true; }
	bool& getBattleStatus() { return isBattle; }
	float getWidth() { return width; }
	float getHeight() { return height; }
	sf::Sprite& getPlayer() { return player; }
	sf::Sprite& getArena() { return arena; }

	void EventHandling(sf::RenderWindow& window) {

		if (!isBattle && !isClear) {
			
			// gameWorld movement control
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W) && player.getGlobalBounds().top > 0) 
			{
				player.move(0.f, -movement);
				for (auto& bush : bushes) {
					if (player.getGlobalBounds().intersects(bush->sprite.getGlobalBounds())) {
						player.move(0.f, movement);
						return;
					}
				}
				if (view.getCenter().y - view.getSize().y / 2 <= 0)
					view.setCenter(view.getCenter().x, view.getSize().y / 2);
				else
					view.move(0.f, -movement);
				window.setView(view);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S) && player.getGlobalBounds().top 
				+ player.getGlobalBounds().height < window.getSize().y) 
			{
				player.move(0.f, movement);
				for (auto& bush : bushes) {
					if (player.getGlobalBounds().intersects(bush->sprite.getGlobalBounds())) {
						player.move(0.f, -movement);
						return;
					}
				}
				if (view.getCenter().y + view.getSize().y / 2 >= window.getSize().y)
					view.setCenter(view.getCenter().x, window.getSize().y - view.getSize().y / 2);
				else
					view.move(0.f, movement);
				window.setView(view);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A) && player.getGlobalBounds().left > 0) 
			{
				player.move(-movement, 0.f);
				player.setTexture(player_left);
				for (auto& bush : bushes) {
					if (player.getGlobalBounds().intersects(bush->sprite.getGlobalBounds())) {
						player.move(movement, 0.f);
						return;
					}
				}
				if (view.getCenter().x - view.getSize().x / 2 <= 0)
					view.setCenter(view.getSize().x / 2, view.getCenter().y);
				else
					view.move(-movement, 0.f);
				window.setView(view);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D) && player.getGlobalBounds().left 
				+ player.getGlobalBounds().width < window.getSize().x) 
			{
				player.move(movement, 0.f);
				player.setTexture(player_right);
				for (auto& bush : bushes) {
					if (player.getGlobalBounds().intersects(bush->sprite.getGlobalBounds())) {
						player.move(-movement, 0.f);
						return;
					}
				}
				if (view.getCenter().x + view.getSize().x / 2 >= window.getSize().x)
					view.setCenter(window.getSize().x - view.getSize().x / 2, view.getCenter().y);
				else
					view.move(movement, 0.f);
				window.setView(view);
			}

			// Entry to Arena
			if (player.getGlobalBounds().contains(arena.getPosition().x + arena.getGlobalBounds().width / 2 
				- player.getGlobalBounds().width / 2, arena.getPosition().y + arena.getGlobalBounds().height / 2)) 
			{
				music.stop();
				isBattle = true;
				arena.setPosition(2000.f, 1000.f);
				player.setPosition(-2000.f, -1000.f);
				view.setCenter(sf::Vector2f(window.getSize().x * 3 / 8, window.getSize().y * 3 / 8));
				view.setSize(sf::Vector2f(window.getSize().x * 3 / 4, window.getSize().y * 3 / 4));
				window.setView(view);
			}

			// Game clear settings
			if (stage == 3) 
			{
				Set_Music(music, "music/korea.mp3");
				playMusic();
				std::string clear = "GAME CLEAR";
				if (!font.loadFromFile("arialnarrow.ttf"))
					return;
				text.setFont(font);	
				text.setString(clear);
				text.setCharacterSize(150);
				text.setFillColor(sf::Color::Black);
				text.setStyle(sf::Text::Bold | sf::Text::Italic);
				text.setPosition(800.f - text.getGlobalBounds().width / 2, 350.f - text.getGlobalBounds().height / 2);
				view.setCenter(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
				view.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
				window.setView(view);
				isClear = true;
			}
		}
	}

	void draw_all(sf::RenderWindow& window) {

		if (!isClear) {
			for (auto& rows : tiles)
				for (auto& columns : rows)
					window.draw(columns->sprite);

			for (auto& bush : bushes)
				window.draw(bush->sprite);

			window.draw(arena);
			window.draw(player);
		}
		else {
			window.draw(game_clear);
			window.draw(text);
			text.move(0.f, 0.5f);
			if (text.getGlobalBounds().top >= window.getSize().y)
				text.setPosition(800.f - text.getGlobalBounds().width / 2, 0.f);
		}
	}

};


