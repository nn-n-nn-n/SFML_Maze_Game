#include <iostream>
#include <SFML\Graphics.hpp>
#include "windows.h"

using namespace std;

const int LY = 23; // размер лабиринта по вертикали
const int LX = 28; // размер лабиринта по горизонтали
const int sprSize = 24; // размер спрайта в пикселах
const int dashboardSprSize = 48; // размер спрайтов информационных сообщений
const int spritesCount = 9; //Количество спрайтов

int maze[LY][LX] = { // это наш лабиринт, структура та же
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,1,4,4,1,5,0,0,0,0,0,0,0,0,1,1,0,7,7,2,0,0,0,0,0,8,1,1},
{1,1,0,8,4,1,1,1,0,0,1,1,1,0,1,1,0,0,1,4,4,1,1,1,0,4,1,1},
{1,1,0,1,1,1,1,1,4,1,1,1,1,4,0,0,0,0,1,1,1,1,1,1,0,1,1,1},
{1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,4,4,1,1,1,0,1,1,1},
{1,1,0,1,0,0,5,1,0,1,1,0,1,1,8,1,1,1,1,4,0,1,7,1,0,1,1,1},
{1,1,0,1,0,1,1,1,0,1,1,0,6,1,7,4,1,1,1,0,0,0,0,1,0,1,1,1},
{1,1,0,1,4,0,1,1,0,1,1,0,1,1,0,4,1,1,1,1,1,1,0,0,0,1,1,1},
{1,1,0,1,1,0,4,1,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,1,1,1},
{1,1,0,1,8,0,1,1,0,0,0,1,1,1,1,1,1,1,4,1,1,8,1,1,0,1,1,1},
{1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,0,6,1,0,5,1,1},
{1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,5,1,1,0,1,1,1},
{1,0,7,1,0,1,1,1,0,0,0,0,0,0,4,1,1,1,0,0,0,0,1,1,0,1,1,1},
{1,0,0,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,4,1,1},
{1,1,0,1,0,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,0,0,1,0,1,1,1,0,1,1,1,1,4,1,1,1,0,1,1,1,0,1,1,0,1,1,1},
{1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,0,1,1,0,1,1,1},
{1,0,0,1,0,1,1,1,1,1,1,1,0,1,1,1,1,0,1,4,1,0,1,1,0,1,1,1},
{1,1,0,1,0,1,5,0,0,0,4,0,0,1,0,0,0,0,0,0,1,0,1,1,0,1,1,1},
{1,1,0,1,0,1,1,1,6,1,1,1,1,1,1,6,1,1,1,1,4,0,4,1,0,1,1,1},
{1,4,8,1,0,1,1,1,1,1,1,1,1,4,1,1,1,1,1,4,1,1,1,1,0,4,1,1},
{1,5,6,1,3,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

// переменные их консольной версии игры
int score = 0; // счет игры

int gameState = 0; // 0 - игра продолжается, 1 - выигрыш, 2 - закончилось время, 3 - не валидный лабиринт
sf::Time timeLimit = sf::milliseconds(90000); // лимит игры в миллисекундах
sf::Time gameTime; // оставшееся время
sf::Time elapsedTime; // счетчик прошедшего времени
sf::Clock gameClock; // таймер

sf::Vector2f scoreTextPosition, timeTextPosition; // позиция текста для счета и текста для времени

struct PlayerPosition // структура, в которой храним позицию игрока
{
	int x, y; // координаты х и у игрока
};

PlayerPosition playerPos; // объявляем переменную, которая будет хранить позицию игрока
// это структура PlayerPosition с полями х и у
// в этом варианте игры будем хранить ее именно так

// объявляем окно, в которой отображается игра. Задаем размер, кратный размеру спрайта и заголовок окна
sf::RenderWindow window(sf::VideoMode(800, 688), "SFML maze game");

// Объекты игры
sf::Texture textures[spritesCount]; // это массив для хранения текстур
sf::Sprite sprites[spritesCount]; // это массив для спрайтов, при этом позиция каждого спрайта в массиве соответствует кодировке в матрице maze

std::string textureNames[spritesCount]{ // объявляем массив строк для хранения имен файлов, которые хранят текстуры
"assets\\bitmaps\\empty.png",
"assets\\bitmaps\\wall.png",
"assets\\bitmaps\\door.png",
"assets\\bitmaps\\man.png",
"assets\\bitmaps\\money.png",
"assets\\bitmaps\\diamond.png",
"assets\\bitmaps\\time.png",
"assets\\bitmaps\\mine.png",
"assets\\bitmaps\\mine.png"
};

// Объекты фонового изображения
sf::Texture backgroundTexture;
sf::Sprite backgroundSprite;
string backgroundImage = "assets\\background\\background.jpg";

// Объект "счет"
sf::Texture scoreTexture;
sf::Sprite scoreSprite;
string scoreImage = "assets\\bitmaps\\score.png";

// Объект "таймер"
sf::Texture timerTexture;
sf::Sprite timerSprite;
string timerImage = "assets\\bitmaps\\timer.png";

// текст названия игры
sf::Text headerText;
sf::Font headerFont;
string headerTextFile = "assets\\fonts\\Novartis-Deco.ttf";

// текст индикаторов игры
sf::Text dashboardText;
sf::Font dashboardFont;
string dashboardTextFile = "assets\\fonts\\Grotesque-Bourgeoisie.ttf";

void UpdateScore(int score)
{
	dashboardText.setString(to_string(score)); // устанавливаем текст для вывода to_string переводит число в строковое представление
	dashboardText.setPosition(scoreTextPosition); // устанавливаем позицию текста для счета
	window.draw(dashboardText); // отрисовываем текст в буфере кадра
}

// Функция обновляет оставшееся время до окончания игры
void UpdateClock(sf::Time elapsed)
{
	gameTime = timeLimit - elapsed; /* вычисляем оставшееся время в секундах
	timeLimit - лимит времени, elapsed - прошедшее время с момента старта игры
	*/
	if (gameTime.asSeconds() < 0) // проверяем, закончилось ли время
		// gameTime.asSeconds() - превращает время из объекта Time в секунды
	{
		gameState = 2; // если да, то обновляем статус игры на 2 - игрок ПРОИГРАЛ
	}
	else
	{ // если время осталось
		dashboardText.setPosition(timeTextPosition); // устанавливаем позицию текста для счета
		// устанавливаем текст для вывода to_string переводит число в строковое представление
		// для получения времени в секундах используем функцию asSeconds. Она возвращает float
		// поэтому явно преобразуем ее в int, иначе возможны десятичные дроби при выводе оставшегося времени
		dashboardText.setString(to_string((int)gameTime.asSeconds()));
		window.draw(dashboardText); // отрисовываем текст в буфере кадра
	}
}

//Функция подготовки фонового спрайта
void PrepareBackgroundImage(string imageFileName)
{
	backgroundTexture.loadFromFile(imageFileName); // загружаем фоновую текстуру
	backgroundSprite.setTexture(backgroundTexture); // натягиваем ее на спрайт
	backgroundSprite.setColor(sf::Color(255, 255, 255, 100)); // устанавливаем прозрачность фонового рисунка
}

//Функция загрузки спрайтов индикаторов игры
void PrepareDashboardImage(string scoreImageFileName, string timerImageFileName)
{
	scoreTexture.loadFromFile(scoreImageFileName); // загружаем текстуру
	scoreSprite.setTexture(scoreTexture); // натягиваем ее на спрайт

	timerTexture.loadFromFile(timerImageFileName); // загружаем текстуру
	timerSprite.setTexture(timerTexture); // натягиваем ее на спрайт

	// Центрирование. Получаем размеры окна игры
	sf::Vector2u wSize = window.getSize();
	// вычисляем координаты расположения иконок
	unsigned int scoreXPos = wSize.x / 2 - 8 * sprSize;
	unsigned int scoreYPos = wSize.y - dashboardSprSize - 10; // 48 размер спрайта иконок, 10 - отступ снизу окна
	unsigned int timerXPos = wSize.x / 2 + 3 * sprSize;
	// устанавливаем позиции спрайтов
	scoreSprite.setPosition(sf::Vector2f(scoreXPos, scoreYPos));
	timerSprite.setPosition(sf::Vector2f(timerXPos, scoreYPos));
	// Вычисляем позицию текста для счета игры = позицияХ_спрайта + размер спрайта + отступ
	scoreTextPosition.x = scoreXPos + dashboardSprSize + 10;
	scoreTextPosition.y = scoreYPos - 15;
	// Вычисляем позицию текста для времени игры = позицияХ_спрайта + размер спрайта + отступ
	timeTextPosition.x = timerXPos + dashboardSprSize + 10;
	timeTextPosition.y = scoreYPos - 15;
}

// Функция загрузки шрифтов
void PrepareFonts(string headerFontName, string dashboardFontName)
{
	// настройки заголовка игры
	headerFont.loadFromFile(headerFontName); // загружаем шрифт
	headerText.setFont(headerFont); // устанавливаем шрифт для текста
	headerText.setCharacterSize(45); // устанавливаем размер символов
	headerText.setString("MAZE GAME"); // задаем строку текста для отображения
	headerText.setFillColor(sf::Color(255, 0, 0, 200)); // устанавливаем цвет Красный и прозрачность
	headerText.setStyle(sf::Text::Bold); // делаем шрифт жирным
	// настройки текста индикаторов игры
	dashboardFont.loadFromFile(dashboardFontName); // загружаем шрифт
	dashboardText.setFont(dashboardFont);// устанавливаем шрифт для текста
	dashboardText.setCharacterSize(45);// устанавливаем размер символов
	dashboardText.setString("0");// задаем строку текста для отображения
	dashboardText.setFillColor(sf::Color(255, 255, 35)); // Устанавливаем светло-желтый цвет текста
	dashboardText.setStyle(sf::Text::Bold | sf::Text::Italic);// делаем шрифт жирным и курсивом, используя битовое И
	// Центрирование заголовка. Получаем размеры окна игры
	sf::Vector2u wSize = window.getSize();
	// получаем границы текстовой строки заголовка. textBounds.width - ширина текста в пикселях
	sf::FloatRect textBounds = headerText.getLocalBounds();
	float x = wSize.x / 2 - textBounds.width / 2;
	headerText.setPosition(sf::Vector2f(x, 5)); // позиционируем заголовок
}

// функция, перерисовывающая образ лабиринта. Используется взамен PrintMaze
void RedrawMaze(int maze[LY][LX], int width, int height)
{
	// сначала "центрируем" лабиринт по размеру окна, для этого нужно вычислить координаты
	// верхнего левого угла лабиринта. Для этого сначала получаем размеры окна игры
	sf::Vector2u wSize = window.getSize();
	// как найти позицию х верхнего левого угла лабиринта внутри окна?
	// надо размер окна поделить на 2 и вычесть из полученного значения половину ширины лабиринта
	unsigned int xMaze = wSize.x / 2 - LX * sprSize / 2;
	// аналогично поступаем с у
	unsigned int yMaze = wSize.y / 2 - LY * sprSize / 2;
	// далее используем эти координаты в качестве базовой точки при выводе лабиринта

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			// получаем из матрицы лабиринта код отрисовываемого объекта
			int index = maze[j][i];
			// Задаем спрайту позицию в окне. Позиция равна по х - номеру текущего столбца
			// умноженного на размер спрайта по горизонтали. По у - номеру строки, умноженному
			// на размер спрайта по вертикали
			sprites[index].setPosition(sf::Vector2f(xMaze + i * sprSize, yMaze + j * sprSize));
			// отрисовываем спрайт
			if (index > 0) window.draw(sprites[index]);
		}
	}
}

// Загрузка текстур и инициализация спрайтов
void PrepareSpriteTexture(std::string names[spritesCount], int count = spritesCount)
{
	for (int i = 0; i < count; i++) // цикл загрузки текстур и инициализации спрайтов
	{
		textures[i].loadFromFile(names[i]); // читаем текстуру из файла
		textures[i].setSmooth(true); // разрешаем размывание текстуры для устранения эффекта пикселизации
		textures[i].setRepeated(false); // запрещаем циклическое заполнение спрайта текстурой
		sprites[i].setTexture(textures[i]); // натягиваем текстуру на спрайт
	}
}

// перемещает игрока из текущей позиции на dx клеток по горизонтали
// и dy клеток по вертикали
void Move(int dx, int dy)
{
	if (playerPos.y + dy >= 0 && playerPos.y + dy < LY && // проверяем на выход за пределы карты
		playerPos.x + dx >= 0 && playerPos.x + dx < LX)
	{
		if (maze[playerPos.y + dy][playerPos.x + dx] != 1) // проверяем, есть ли в позиции перемещения
		{ // стенка, и если нет - перемещаемся
			maze[playerPos.y][playerPos.x] = 0; // убираем игрока из лабиринта в текущей позиции
			playerPos.x += dx; // изменяем координаты
			playerPos.y += dy; // игрока
			switch (maze[playerPos.y][playerPos.x]) //проверяем содержимое клетки лабиринта, которой достиг игрок
			{
			case 2: // если выход - даем 500 бонусов и меняем статус игры на ВЫИГРЫШ
				score += 500; // увеличиваем бонусы
				gameState = 1;
				break;
			case 4: // если монета - добавляем 100 бонусов
				score += 100; // увеличиваем бонусы
				break;
			case 5: // если алмаз - добавляем 300 бонусов
				score += 300; // увеличиваем бонусы
				break;
			case 6: // если время - добавляем 10 секунд
				timeLimit += sf::seconds(10);
				break;
			case 7: // если мина - забираем 200 очков
				if (score - 200 >= 0)
					score -= 200;
				break;
			case 8: // если мина2 - забираем 8 секунд
				timeLimit -= sf::seconds(8);
				break;
			}
			maze[playerPos.y][playerPos.x] = 3; // записываем игрока в новую позицию лабиринта
		}
	}
}

void HandleKeyboardEvents() // Обрабатываем события клавиатуры
{
	// функция isKeyPressed используется для проверки - была ли нажата определенная клавиша.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) // нажата клавиша ВЛЕВО?
	{
		Move(-1, 0); // идем влево
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))// нажата клавиша ВПРАВО?
	{
		Move(1, 0);// идем вправо
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) // вверх
	{
		Move(0, -1);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) // вниз
	{
		Move(0, 1);
	}
}

void RenderScene()
{
	window.draw(backgroundSprite); // отрисовываем спрайт с фоновым изображением в буфере кадра
	RedrawMaze(maze, LX, LY); // отрисовка лабиринта
	window.draw(headerText); // Выводим заголовок игры
	window.draw(scoreSprite); // Выводим спрайт счета
	window.draw(timerSprite); // Выводим спрайт времени
	UpdateScore(score); // обновляем текст счета
	UpdateClock(gameClock.getElapsedTime()); // обновляем оставшееся время игры
}

// функия вывода сообщения о завершении игры
void ShowMessageWindow(const string& text, sf::Color textColor = sf::Color::White, sf::Color backgroundColor = sf::Color::Blue, sf::Color frameColor = sf::Color::White, sf::Vector2f windowSize = { 600,300 })
{
	sf::Vector2u wSize = window.getSize();
	float xTextCoords = wSize.x / 5;
	float yTextCoords = wSize.y / 2.75;

	float xRectangleCoords = wSize.x / 8;
	float yRectangleCoords = wSize.y / 3.75;

	sf::RectangleShape rectangle(windowSize);
	rectangle.setFillColor(backgroundColor);
	rectangle.setOutlineThickness(5);
	rectangle.setOutlineColor(frameColor);
	rectangle.setPosition({ xRectangleCoords,yRectangleCoords });
	dashboardText.setString(text);
	dashboardText.setFillColor(textColor);
	dashboardText.setPosition({ xTextCoords,yTextCoords });

	window.draw(rectangle);
	window.draw(dashboardText);
}

// функция поиска игрока и проверка валидации лабиринта
bool FindFirstPlayerPosition(int maze[LY][LX], int sizeX, int sizeY, PlayerPosition& playerPos)
{
	int OnePlayer = 0;
	int door = 0;
	for (int i = 0; i < sizeY; i++) // цикл пробегает массив по строкам
	{
		for (int j = 0; j < sizeX; j++) // цикл пробегает массив по столбцам
		{
			if (maze[i][j] == 3)
			{
				OnePlayer++;
				playerPos = { j,i };
			}

			if (maze[i][j] == 2)
				door++;
		}
	}
	if (OnePlayer == 1 && door >= 1)
		return true;
	else
		return false;
}


int main()
{
	PrepareSpriteTexture(textureNames); // подготавливаем спрайты лабиринта
	PrepareBackgroundImage(backgroundImage); // готовим спрайт фона
	PrepareDashboardImage(scoreImage, timerImage); // готовим спрайты индикаторов игры
	PrepareFonts(headerTextFile, dashboardTextFile); // готовим текст для вывода времени и счета
	gameClock.restart(); // запускаем таймер с нуля
	if (!FindFirstPlayerPosition(maze, LX, LY, playerPos))
		gameState = 3;
	while (window.isOpen()) // цикл обработки событий, пока окно программы не закрыто
	{
		sf::Event event; // описываем объект события
		while (window.pollEvent(event)) // получаем событие из очереди
		{
			if (event.type == sf::Event::Closed) // проверяем, если это закрытие окна, то закрываем окно
				window.close();
		}
		if (gameState != 0) // проверяем, не закончилась ли игра
		{
			if (gameState == 1) // если игрок прошел лабиринт
				ShowMessageWindow("\t\tCongratulations!\n You have passed the maze!\n\tPress escape to leave.");
			else if (gameState == 2) // если игрок не успел пройти
				ShowMessageWindow("\t\t Time is over!\n\t\t\t You lost!\n\tPress escape to leave.", sf::Color::Yellow, sf::Color(200, 0, 0)); // sf::Color(200,0,0) темно-красный цвет
			else if (gameState == 3)
				ShowMessageWindow("\n\tThe maze is not valid.", sf::Color::White, sf::Color::Black);
			window.display();
			while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)); // ожидаем нажатия клавиши Escape
			window.close(); // закрываем окно
		}
		window.clear(); // очищаем окно
		HandleKeyboardEvents(); // обработка событий клавиатуры
		RenderScene(); // формируем (рендерим) сцену
		window.display(); // отрисовка буфера экрана в окне
		Sleep(100); // пауза 100мс
	}
}