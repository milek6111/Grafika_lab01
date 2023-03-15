#include "Sokoban.h"

void Sokoban::LoadMapFromFile(std::string fileName)
{
	std::string str;
	std::vector<std::string> vos;

	std::ifstream in(fileName.c_str());
	while (std::getline(in, str)) { vos.push_back(str); }
	in.close();

	map.clear();
	map.resize(vos.size(), std::vector<Field>(vos[0].size()));
	for (auto [row, row_end, y] = std::tuple{ vos.cbegin(), vos.cend(), 0 }; row != row_end; ++row, ++y)
		for (auto [element, end, x] = std::tuple{ row->begin(), row->end(), 0 }; element != end; ++element, ++x)
			switch (*element)
			{
			case 'X': map[y][x] = Field::WALL; break;
			case '*': map[y][x] = Field::VOID; break;
			case ' ': map[y][x] = Field::FLOOR; break;
			case 'B': map[y][x] = Field::BOX; break;
			case 'P': map[y][x] = Field::PARK; park_positions.push_back(sf::Vector2i(x, y));  break;
			case 'S': map[y][x] = Field::PLAYER; player_position = sf::Vector2i(x, y);  break;
			}
}

void Sokoban::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//sf::RectangleShape vd, box, wall, floor, player, park;
	//sf::Vector2f start_position(0.0f, 0.0f);
	
	sf::Texture vdtex, floortex, walltex, boxtex, parktex, playertex, playerparktex;
	vdtex.loadFromFile("VOID.png");
	floortex.loadFromFile("FLOOR.png");
	walltex.loadFromFile("WALL.png");
	boxtex.loadFromFile("BOX.png");
	parktex.loadFromFile("PARK.png");
	playertex.loadFromFile("PLAYER.png");
	playerparktex.loadFromFile("PLAYERPARK.png");

	sf::Sprite spr;


	for (int y = 0; y < map.size(); ++y)
		for (int x = 0; x < map[y].size(); ++x)
		{		
			switch (map[y][x]) {
			case Field::VOID:
				spr.setTexture(vdtex);
				spr.setScale(sf::Vector2f(tile_size.x / vdtex.getSize().x, tile_size.y / vdtex.getSize().y));
				break;
			case Field::FLOOR:
				spr.setTexture(floortex);
				spr.setScale(sf::Vector2f(tile_size.x / floortex.getSize().x, tile_size.y / floortex.getSize().y));
				break;
			case Field::WALL:
				spr.setTexture(walltex);
				spr.setScale(sf::Vector2f(tile_size.x / walltex.getSize().x, tile_size.y / walltex.getSize().y));
				break;
			case Field::BOX:
				spr.setTexture(boxtex);
				spr.setScale(sf::Vector2f(tile_size.x / boxtex.getSize().x, tile_size.y / boxtex.getSize().y));
				break;
			case Field::PARK:
				spr.setTexture(parktex);
				spr.setScale(sf::Vector2f(tile_size.x / parktex.getSize().x, tile_size.y / parktex.getSize().y));
				break;
			case Field::PLAYER:
				bool flag = false;
				for (auto pos : park_positions)
					if ((x == pos.x) && (y == pos.y)) {
						flag = true;
						break;
					}

				if (flag) {
					spr.setTexture(playerparktex);
					spr.setScale(sf::Vector2f(tile_size.x / playerparktex.getSize().x, tile_size.y / playerparktex.getSize().y));
				}
				else {
					spr.setTexture(playertex);
					spr.setScale(sf::Vector2f(tile_size.x / playertex.getSize().x, tile_size.y / playertex.getSize().y));
				}
				break;
			}
			spr.setPosition(shift.x + x * tile_size.x, shift.y + y * tile_size.y);
			target.draw(spr);
		}
}

void Sokoban::SetDrawParameters(sf::Vector2u draw_area_size)
{
	this->tile_size = sf::Vector2f(
		std::min(std::floor((float)draw_area_size.x / (float)map[0].size()), std::floor((float)draw_area_size.y / (float)map.size())),
		std::min(std::floor((float)draw_area_size.x / (float)map[0].size()), std::floor((float)draw_area_size.y / (float)map.size()))
	);
	this->shift = sf::Vector2f(
		((float)draw_area_size.x - this->tile_size.x * map[0].size()) / 2.0f,
		((float)draw_area_size.y - this->tile_size.y * map.size()) / 2.0f
	);
}

void Sokoban::Move_Player_Left()
{
	move_player(-1, 0);
}

void Sokoban::Move_Player_Right()
{
	move_player(1, 0);
}

void Sokoban::Move_Player_Up()
{
	move_player(0, -1);
}

void Sokoban::Move_Player_Down()
{
	move_player(0, 1);
}

sf::Vector2f Sokoban::Get_Tile_Size()const {
	return tile_size;
}

void Sokoban::move_player(int dx, int dy)
{
	bool allow_move = false; // Pesymistyczne za³ó¿my, ¿e gracz nie mo¿e siê poruszyæ.
	sf::Vector2i new_pp(player_position.x + dx, player_position.y + dy); //Potencjalna nowa pozycja gracza.
	Field fts = map[new_pp.y][new_pp.x]; //Element na miejscu na które gracz zamierza przejœæ.
	Field ftsa; //Element na miejscu ZA miejscem na które gracz zamierza przejœæ. :-D

	//Gracz mo¿e siê poruszyæ jeœli pole na którym ma stan¹æ to pod³oga lub miejsce na skrzynki.
	if (fts == Field::FLOOR || fts == Field::PARK) allow_move = true;
	//Jeœli pole na które chce siê poruszyæ gracz zawiera skrzynkê to mo¿e siê on poruszyæ jedynie jeœli kolejne pole jest puste lub zawiera miejsce na skrzynkê  - bo wtedy mo¿e przepchn¹æ skrzynkê.
	if (fts == Field::BOX) {
		ftsa = map[new_pp.y + dy][new_pp.x + dx];
		if (ftsa == Field::FLOOR || ftsa == Field::PARK)
		{
			allow_move = true;
			//Przepychamy skrzynkê.
			map[new_pp.y + dy][new_pp.x + dx] = Field::BOX;
			//Oczywiœcie pole na którym sta³a skrzynka staje siê teraz pod³og¹.
			map[new_pp.y][new_pp.x] = Field::FLOOR;
			}
	}

	if (allow_move)
	{
		//Przesuwamy gracza.
		map[player_position.y][player_position.x] = Field::FLOOR;
		player_position = new_pp;
		map[player_position.y][player_position.x] = Field::PLAYER;
	}

	//Niestety w czasie ruchu mog³y „ucierpieæ” miejsca na skrzynkê. ;-(
	for (auto park_position : park_positions) if (map[park_position.y][park_position.x] == Field::FLOOR) map[park_position.y][park_position.x] = Field::PARK;
}

bool Sokoban::Is_Victory() const
{
	//Tym razem dla odmiany optymistycznie zak³adamy, ¿e gracz wygra³.
	//No ale jeœli na którymkolwiek miejscu na skrzynki nie ma skrzynki to chyba za³o¿enie by³o zbyt optymistyczne... : -/
	for (auto park_position : park_positions) if (map[park_position.y][park_position.x] != Field::BOX) return false;
	return true;
}