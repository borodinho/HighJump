#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <random>
#include <ctime>

using namespace sf;
using namespace std;

int main()
{
	setlocale(LC_ALL, "Rus");
	RenderWindow window(VideoMode(500, 700), "High jump", Style::Close); 
	window.setFramerateLimit(60);
	Texture bgTexture;
	Texture playerTexture;
	Texture platformTexture;
	bgTexture.loadFromFile("resources/images/background.png");
	playerTexture.loadFromFile("resources/images/kapybara.png");
	platformTexture.loadFromFile("resources/images/platform.png");
	Sprite background(bgTexture);
	Sprite player(playerTexture);
	Sprite platform(platformTexture);
	

	RectangleShape gameoverBg(Vector2f(500, 700));
	gameoverBg.setFillColor(Color::Black);

	Font font;
	font.loadFromFile("resources/font/ariali.ttf");
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(50);
	scoreText.setFillColor(Color::White);
	Text gameoverText;
	gameoverText.setFont(font);
	gameoverText.setString(L"Конец игры!");
	gameoverText.setCharacterSize(80);
	gameoverText.setFillColor(Color::White);

	
	SoundBuffer buffer;
	buffer.loadFromFile("resources/sound/jump.wav");
	Sound sound;
	sound.setBuffer(buffer);

	
	Vector2u platformPosition[10];
	uniform_int_distribution<unsigned> x(0, 500 - platformTexture.getSize().x);
	uniform_int_distribution<unsigned> y(100, 700);
	default_random_engine e(time(0));
	for (size_t i = 0; i < 10; ++i)
	{
		platformPosition[i].x = x(e);
		platformPosition[i].y = y(e);
	}

	
	int playerX = 250;
	int playerY = 151;
	float dy = 0;
	int height = 150;
	int score = 0;

	
	const int KAPYBARA_LEFT_BOUNDING_BOX = 20;
	const int KAPYBARA_RIGHT_BOUNDING_BOX = 60;
	const int KAPYBARA_BOTTOM_BOUNDING_BOX = 70;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
			playerX -= 4;
		if (Keyboard::isKeyPressed(Keyboard::Right))
			playerX += 4;

		// капибара выходит за границу и меняет сторону
		if (playerX > 500)
			playerX = 0;
		if (playerX < -40)
			playerX = window.getSize().x - playerTexture.getSize().x;

		// подсчет очков
	
		if (playerY == height && dy < (-1.62)) 
		{
			score += 1;
			scoreText.setString(L"Очки: " + to_string(score));
		}

		// прыжок капибары
		dy += 0.2;
		playerY += dy;

		if (playerY < height)
			for (size_t i = 0; i < 10; ++i)
			{
				playerY = height;
				platformPosition[i].y -= dy;  // вертикаль
				if (platformPosition[i].y > 700) 
				{
					platformPosition[i].y = 0;
					platformPosition[i].x = x(e);
				}

			}
		// прыжок на платформу
		for (size_t i = 0; i < 10; ++i)
		{
			if ((playerX + KAPYBARA_RIGHT_BOUNDING_BOX > platformPosition[i].x) && (playerX + KAPYBARA_LEFT_BOUNDING_BOX < platformPosition[i].x + platformTexture.getSize().x)        // player's horizontal range can touch the platform
				&& (playerY + KAPYBARA_BOTTOM_BOUNDING_BOX > platformPosition[i].y) && (playerY + KAPYBARA_BOTTOM_BOUNDING_BOX < platformPosition[i].y + platformTexture.getSize().y)  // player's vertical   range can touch the platform
				&& (dy > 0)) // капибарка упала(
			{
				sound.play();
				dy = -10;
			}

		}
		player.setPosition(playerX, playerY);

		window.draw(background);
		window.draw(player);

		// отрисовка платформ вверху
		for (size_t i = 0; i < 10; ++i)
		{
			platform.setPosition(platformPosition[i].x, platformPosition[i].y);
			window.draw(platform);
		}

		 // конец игры
		if (playerY > 700)
		{
			gameoverText.setPosition(30, 200);
			scoreText.setPosition(150, 400);
			goto gameover;
		}
		window.draw(scoreText);
		window.display();
	}

	// вывод на экран об окончании игры
gameover:
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		window.draw(gameoverBg);
		window.draw(gameoverText);
		window.draw(scoreText);
		window.display();
	}
	return 0;
}