#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include <fstream>
#include <string>

enum class Field { VOID, FLOOR, WALL, BOX, PARK, PLAYER };

class Sokoban : public sf::Drawable
{
public:
	void LoadMapFromFile(std::string fileName);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void SetDrawParameters(sf::Vector2u draw_area_size);
	void Move_Player_Left();
	void Move_Player_Right();
	void Move_Player_Up();
	void Move_Player_Down();
	sf::Vector2f Get_Tile_Size() const;
	bool Is_Victory() const;

private:
	std::vector<std::vector<Field>> map;
	sf::Vector2f shift, tile_size;
	sf::Vector2i player_position;
	std::vector<sf::Vector2i> park_positions;

	void move_player(int dx, int dy);
};
