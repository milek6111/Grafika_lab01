//Uwaga! Co najmniej C++17!!!
//Project-> ... Properties->Configuration Properties->General->C++ Language Standard = ISO C++ 17 Standard (/std:c++17)

#include "Sokoban.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "GFK Lab 01", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
	Sokoban sokoban;

	sokoban.LoadMapFromFile("plansza.txt");
	sokoban.SetDrawParameters(window.getSize());

	sf::RectangleShape startwindow;
	startwindow.setSize(sf::Vector2f(800.0f, 600.0f));

	sf::Font font;
	font.loadFromFile("Valorant Font.ttf");
	sf::Text surr;
	sf::Text win;
	surr.setFont(font);
	win.setFont(font);

	sf::Text t;
	t.setFont(font);
	t.setPosition(0.0f, 0.0f);
	t.setCharacterSize(35);
	t.setFillColor(sf::Color::White);

	sf::Clock gameTime;
	int tm = 0;

	sf::Clock time;
	sf::Time elapsed;


	while (window.isOpen())
	{
		sf::Event event;

		if (gameTime.getElapsedTime().asSeconds() > 1.0) {
			gameTime.restart();
			tm++;
			window.clear(sf::Color::Black);
			t.setString(std::to_string(tm));
			window.draw(t);
		}


		while (window.pollEvent(event))
		{

			if (event.type == sf::Event::Closed) window.close();

			if (event.type == sf::Event::Resized)
			{
				float width = static_cast<float>(event.size.width);
				float height = static_cast<float>(event.size.height);
				window.setView(sf::View(sf::FloatRect(0, 0, width, height)));
				sokoban.SetDrawParameters(window.getSize());
				t.setCharacterSize(unsigned int(35 * (width/startwindow.getSize().x)));
				t.setCharacterSize(unsigned int(35 * (height / startwindow.getSize().y)));
			}
			
			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code)
				{
				case sf::Keyboard::Left:
					sokoban.Move_Player_Left();
					break;
				case sf::Keyboard::Right:
					sokoban.Move_Player_Right();
					break;
				case sf::Keyboard::Up:
					sokoban.Move_Player_Up();
					break;
				case sf::Keyboard::Down:
					sokoban.Move_Player_Down();
					break;
				case::sf::Keyboard::Escape:
					surr.setString("YOU SURRENDERED!");
					surr.setCharacterSize(50);
					sf::FloatRect textRect = surr.getLocalBounds();
					surr.setOrigin(textRect.width / 2, textRect.height / 2);
					surr.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
					surr.setFillColor(sf::Color::Black);
					window.clear(sf::Color::Black);
					window.draw(sokoban);
					window.draw(t);
					window.draw(surr);
					window.display();

					time.restart();

					do {
						elapsed = time.getElapsedTime();
					} while (elapsed.asSeconds()< 3);
					

					window.close();
					break;
				}
			}
			if (sokoban.Is_Victory()) {
				win.setString("YOU WON IN " + std::to_string(tm) + " SECONDS");
				win.setCharacterSize(50);
				sf::FloatRect textRect = win.getLocalBounds();
				win.setOrigin(textRect.width / 2, textRect.height / 2);
				win.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
				win.setFillColor(sf::Color::Black);

				window.clear(sf::Color::Black);
				window.draw(sokoban);
				window.draw(t);
				window.draw(win);
				window.display();

				time.restart();

				do {
					elapsed = time.getElapsedTime();
				}while (elapsed.asSeconds() < 5);
				
				window.close();
			}
			window.clear(sf::Color::Black);
			window.draw(sokoban);
			window.draw(t);
			window.display();
		}
		window.clear(sf::Color::Black);
		window.draw(sokoban);
		window.draw(t);
		window.display();
	}

	return 0;
}