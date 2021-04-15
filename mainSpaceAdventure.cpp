#include <iostream>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <conio.h>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

short widthV = 130, heightV = 50; // visible
int ofset = 30, delay = 17, game = 0, debug = 0;
short width = widthV + ofset, height = heightV; // pre render, lmao no pre render used
int len = 0, hp = 3, customAsteroidSpawnRate = 0, moonSpawn = 1000, martianSpawn = 2000, marsSpawn = 3500, asteroidBeltStart = 4500, asteroidBeltEnd = 6000, jupiterSpawn = 8000, saturnSpawn = 10000, uranusSpawn = 12000, neptuneSpawn = 14500, plutoSpawn = 16000, voyagerSpawn = 17000;
// some game var 
int voyagerX = 0, voyagerY = 0, victoryEnd = 0, set2 = 0, set3 = 0, set4 = 0, victory = 30, planetX = 0, planetY = 0, invincible = 0, invincibleE1 = 0, invincibleE2 = 0, colisionNum = 0, showLength = 1, event = 0, frame = 0, martianLength = 0, wave = 0, cooldown = 0, entityColisionX1 = 0, entityColisionX2 = 0, entityColisionY1 = 0, entityColisionY2 = 0;
string str, planetType, button;
float crash = -1, crashE1 = -1, crashE2 = -1, colisionTime = -1, martianMadnessProgress = 0, entityX1 = widthV, entityX2 = widthV, entityY1 = height / 4, entityY2 = height / 4 * 3, ufo1Explosion = 10, ufo2Explosion = 10;


int shipCoordX[21] = { 0,1,2,3,1,2,3,4,0,1,2,3,4,5,6,1,2,3,4,0,1 }; // ship colision coords relative to x y
int shipCoordY[21] = { 0,0,0,0,1,1,1,1,2,2,2,2,2,2,2,3,3,3,3,4,4 };
int colisionX;
int colisionY;
int ufoColisionX[13] = { 3,3,4,5,2,3,4,0,1,2,3,4,5 };
int ufoColisionY[13] = { 0,0,0,0,1,1,1,2,2,2,3,3,3 };

float x = widthV / 8, y = height / 2 - 3;

vector <int> trailY = { 0,0,0,0 };

vector <int> dotX;
vector <int> dotY;
vector <int> dotSpeed;

vector <int> asteroidX;
vector <int> asteroidY;
vector <int> asteroidType;
vector <int> asteroidHp;
vector <float> asteroidDestroyed;

int hpe1 = 3, hpe2 = 3;

vector <int> bulletXe1;
vector <int> bulletYe1;

vector <int> bulletXe2;
vector <int> bulletYe2;

vector <int> bulletXp;
vector <int> bulletYp;

wchar_t* screen = new wchar_t[widthV * heightV * 200]; // Массив для записи в буфер
HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); // Буфер экрана

void mainMenu();
int main();

void text(int X, int Y, string text)
{
	for (int i = 0; i < text.size(); i += 1)
	{
		screen[Y * widthV + X + i] = text[i];
	}
}

string texture[50];

string getKey()
{
	string c;
	char b;
	int a;
	a = _getch();//получаем ascii код символа
	if (a == 224)// особые кнопки
	{
		a = _getch();
		if (a == 75)
		{
			return "leftArrow";
		}
		if (a == 72)
		{
			return "upArrow";
		}
		if (a == 80)
		{
			return "downArrow";
		}
		if (a == 77)
		{
			return "rightArrow";
		}
		if (a == 82)
		{
			return "insert";
		}
	}
	// больше особых кнопок
	else if (a == 32)
	{
		return "space";
	}
	if (a == 8)
	{
		return "backspace";
	}
	if (a == 9)
	{
		return "tab";
	}
	if (a == 13)
	{
		return "enter";
	}
	if (a == 27)
	{
		return "esc";
	}
	b = a;// превращяем int в чар
	c = b;// ревращаем char в string
	return c;// возвращаем значение функции
}

void reset()
{
	x = widthV / 8, y = height / 2 - 3;
	hpe1 = 3, hpe2 = 3;
	victoryEnd = 0, set2 = 0, set3 = 0, set4 = 0, victory = 30, planetX = 0, planetY = 0, invincible = 0, invincibleE1 = 0, invincibleE2 = 0, colisionNum = 0, showLength = 1, event = 0, frame = 0, martianLength = 0, wave = 0, cooldown = 0, entityColisionX1 = 0, entityColisionX2 = 0, entityColisionY1 = 0, entityColisionY2 = 0;
	crash = -1, crashE1 = -1, crashE2 = -1, colisionTime = -1, martianMadnessProgress = 0, entityX1 = widthV, entityX2 = widthV, entityY1 = height / 4, entityY2 = height / 4 * 3, ufo1Explosion = 10, ufo2Explosion = 10;
	len = 0, hp = 3;

	dotX.clear();
	dotY.clear();
	dotSpeed.clear();

	asteroidX.clear();
	asteroidY.clear();
	asteroidType.clear();

	bulletXe1.clear();
	bulletYe1.clear();

	bulletXe2.clear();
	bulletYe2.clear();

	bulletXp.clear();
	bulletYp.clear();
}

void drawTexture(int X, int Y, int n)
{
	for (int i = 0; i < n; i += 1)
	{
		for (int j = 0; j < texture[i].size(); j += 1)
		{
			if (texture[i][j] != 'q' && j + X < widthV && j + X >= 0)
			{
				screen[(i + Y) * widthV + j + X] = texture[i][j];
			}
		}
	}
}

void fileIcon(int X, int Y)
{
	texture[0] = "####";
	texture[1] = "%   #";
	texture[2] = "%    #";
	texture[3] = "%    #";
	texture[4] = "%    #";
	texture[5] = "%%%%%%";
	drawTexture(X, Y, 6);
}

void ship(int X, int Y)
{
	const int n = 5;
	texture[0] = "<\\__";
	texture[1] = "q\\  \\";
	texture[2] = "<= 0 |-";
	texture[3] = "q/__/";
	texture[4] = "</";

	drawTexture(X, Y, n);
}

void shipExplosion(int X, int Y, int pos)
{
	int n = 0;
	if (pos == 0)
	{
		texture[0] = "q";
		texture[1] = "#\\_#";
		texture[2] = "q\\  \\";
		texture[3] = "<= 0 |-";
		texture[4] = "q#__#";
		texture[5] = "</q#";
		n = 6;
	}
	if (pos == 1)
	{
		texture[0] = "q#qq#";
		texture[1] = "#q##q#";
		texture[2] = "q#q##\\";
		texture[3] = "q<# # |-";
		texture[4] = "q#q#q#";
		texture[5] = "q<#q#";
		n = 6;
	}
	if (pos == 2)
	{
		texture[0] = "qq#q##";
		texture[1] = "q#q#qq#";
		texture[2] = "#qqq#qq#";
		texture[3] = "q#qq%q#";
		texture[4] = "qq#qq#";
		texture[5] = "q#qqqqq#";
		texture[6] = "qq#q#q#";
		n = 7;
	}
	if (pos == 3)
	{
		texture[0] = "qq#q%";
		texture[1] = "q%q#qq#";
		texture[2] = "#qqq#qq#";
		texture[3] = "q%qq=q#";
		texture[4] = "qq#qq%";
		texture[5] = "q%qqqqq%";
		texture[6] = "qq%q#q#";
		n = 7;
	}
	if (pos == 4)
	{
		texture[0] = "qq=q=";
		texture[1] = "q%q%qq%";
		texture[2] = "#qqq#qq%";
		texture[3] = "q=qq+q%";
		texture[4] = "qq#qq%";
		texture[5] = "q%qqqqq=";
		texture[6] = "qq=q#q=";
		n = 7;
	}
	if (pos == 5)
	{
		texture[0] = "qq+q+";
		texture[1] = "q%q%qq%";
		texture[2] = "%qqq%qq=";
		texture[3] = "q+qqqq=";
		texture[4] = "qq%qq%";
		texture[5] = "q=qqqqq=";
		texture[6] = "qq+q%q+";
		n = 7;
	}
	if (pos == 6)
	{
		texture[0] = "q";
		texture[1] = "qqq+";
		texture[2] = "=qqq=qq+";
		texture[3] = "q";
		texture[4] = "qq=qq=";
		texture[5] = "q+qqqqq+";
		texture[6] = "qqqq=";
		n = 7;
	}
	if (pos == 7)
	{
		texture[0] = "q";
		texture[1] = "q";
		texture[2] = "+qqq+";
		texture[3] = "q";
		texture[4] = "q";
		texture[5] = "q";
		texture[6] = "qqqq+";
		n = 7;
	}
	if (pos == 8)
	{
		texture[0] = "q";
		texture[1] = "q";
		texture[2] = "q";
		texture[3] = "q";
		texture[4] = "q";
		texture[5] = "q";
		texture[6] = "q";
		n = 7;
	}

	drawTexture(X, Y, n);
}

void asteroid(int X, int Y, int type)
{
	int n = 0;

	if (type == 0)
	{
		n = 6;
		texture[0] = "_/\\";
		texture[1] = "\\. |";
		texture[2] = "|  .\\";
		texture[3] = "[    >";
		texture[4] = "|  _/";
		texture[5] = "q\\/";
	}
	else if (type == 1)
	{
		n = 4;
		texture[0] = "qq_";
		texture[1] = "q/.\\";
		texture[2] = "|  .|";
		texture[3] = "q\\_/";
	}
	else if (type == 2)
	{
		n = 5;
		texture[0] = "q___";
		texture[1] = "/ . \\";
		texture[2] = "|   .\\";
		texture[3] = "q\\_  |";
		texture[4] = "qqq\\/";
	}
	else if (type == 3)
	{
		n = 6;
		texture[0] = "qqq_";
		texture[1] = "qq/.\\";
		texture[2] = "q/  .|";
		texture[3] = "|    |";
		texture[4] = "|   /";
		texture[5] = "q\\_/";
	}
	else if (type == 4)
	{
		n = 5;
		texture[0] = "qq_";
		texture[1] = "q/.\\";
		texture[2] = "|  .|";
		texture[3] = "|  /";
		texture[4] = "q\\/";
	}
	else if (type == 5)
	{
		n = 5;
		texture[0] = "q/\\";
		texture[1] = "| .|";
		texture[2] = "|  .\\";
		texture[3] = "|  _/";
		texture[4] = "q\\/";
	}

	drawTexture(X, Y, n);
}

void ufo(int X, int Y, int type)
{
	int n = 0;
	if (type == 0)
	{
		n = 3;
		texture[0] = "qq_=_";
		texture[1] = "-=___=-";
		texture[2] = " / | \\";
	}
	else if (type == 1)
	{
		n = 4;
		texture[0] = "qqq.-.";
		texture[1] = "qq_|_|_";
		texture[2] = "-=_   _=-";
		texture[3] = "qqq\\_/";
	}

	drawTexture(X, Y, n);
}

void ufoExplosion(int X, int Y, int num)
{
	int n = 0;
	if (num == 0)
	{
		texture[0] = "qqq.-.";
		texture[1] = "qq_#_|_";
		texture[2] = "-=_   #=-";
		texture[3] = "qqq#_/";
		n = 4;
	}
	if (num == 1)
	{
		texture[0] = "qqq#-.";
		texture[1] = "qq#%#|#";
		texture[2] = "-=_# #%#-";
		texture[3] = "qq#%#/#";
		texture[4] = "qqq#";
		n = 5;
	}
	if (num == 2)
	{
		texture[0] = "qqq%-%";
		texture[1] = "qq% %|%";
		texture[2] = "-%_  % %-";
		texture[3] = "qq% %/%";
		texture[4] = "qqq%";
		n = 5;
	}
	if (num == 3)
	{
		texture[0] = "qqq=-=";
		texture[1] = "  = =%=";
		texture[2] = "-=%  = =";
		texture[3] = "  = =%=";
		texture[4] = "   =";
		n = 5;
	}
	if (num == 4)
	{
		texture[0] = "  +-+";
		texture[1] = " + +=+";
		texture[2] = "+=  + +";
		texture[3] = " + +=+";
		texture[4] = "  +";
		n = 5;
	}
	if (num == 5)
	{
		texture[0] = "q+";
		texture[1] = "qqq+";
		texture[2] = "+qqqq+";
		texture[3] = "qqq+";
		int n = 4;
	}
	if (num == 6)
	{
		texture[0] = "q+";
		texture[1] = "q";
		texture[2] = "+";
		texture[3] = "qqq+";
		int n = 4;
	}

	drawTexture(X, Y, n);
}

void moon(int X, int Y)
{
	int n = 16;
	texture[0] = "qqqqqqqqq____";
	texture[1] = "qqqqq_---    ---_";
	texture[2] = "qqq_/    o      o-_";
	texture[3] = "qq/   _        /|  \\";
	texture[4] = "q|   / \\   /\\ .     |";
	texture[5] = "q|  |   |. \\/. .    |";
	texture[6] = "|   .\\_/  ()         |";
	texture[7] = "|         . () _  () |";
	texture[8] = "| |         . / \\    |";
	texture[9] = "| *-  |      |   |   |";
	texture[10] = "|      \\|    /\\_/    |";
	texture[11] = "q|     -*--  \\/     |";
	texture[12] = "q|     /|       *   |";
	texture[13] = "qq\\_           <> _/";
	texture[14] = "qqqq-__        __-";
	texture[15] = "qqqqqqq--____--";

	drawTexture(X, Y, n);
}

void mars(int X, int Y)
{
	int n = 21;
	texture[0] = "qqqqqqqqqq___---___";
	texture[1] = "qqqqqqq_--        _--_";
	texture[2] = "qqqq_--      ___--    --_";
	texture[3] = "qqq/---___---      _     \\";
	texture[4] = "qq/      _        / \\     \\";
	texture[5] = "q| |    / \\      |   | .   |";
	texture[6] = "q|     /_/      /\\\\_/      |";
	texture[7] = "q|     /        \\/         |";
	texture[8] = "|            .              |";
	texture[9] = "|           .               |";
	texture[10] = "|                           |";
	texture[11] = "|____                       |";
	texture[12] = "|    ----||__      ____     |";
	texture[13] = "q|           ----_/    \\__ |";
	texture[14] = "q|-_                      -|";
	texture[15] = "q| _-  _--_                |";
	texture[16] = "qq\\     --       ______   /";
	texture[17] = "qqq\\_       __---      -_/";
	texture[18] = "qqqqq--  _--          --";
	texture[19] = "qqqqqqq---___   ___---";
	texture[20] = "qqqqqqqqqqqqq---";

	drawTexture(X, Y, n);
}

void jupiter(int X, int Y)
{
	texture[0] = "q";
	texture[1] = "qqqqqqqqq__-------__";
	texture[2] = "qqqqqq__-            -_";
	texture[3] = "qqqq_-            -    -_";
	texture[4] = "qqq/      -*-  - .-      \\";
	texture[5] = "qq/                       \\";
	texture[6] = "q| - - -   -  ---    --  - |";
	texture[7] = "q|              .    .     |";
	texture[8] = "q|=--==-==-=----==--==-=---|";
	texture[9] = "|                           |";
	texture[10] = "|  -- -  -  -  -    -    -  |";
	texture[11] = "|               .           |";
	texture[12] = "|--===--==--=--===--=-=-=-==|";
	texture[13] = "|                           |";
	texture[14] = "q|-=__              _--_   |";
	texture[15] = "q|    --=---===--==-_ 0_-==|";
	texture[16] = "q|                   --    |";
	texture[17] = "qq\\   -   -  -   - -o o- -/";
	texture[18] = "qqq\\                     /";
	texture[19] = "qqqq--_               _--";
	texture[20] = "qqqqqqq--__       __--";
	texture[21] = "qqqqqqqqqqq-------";
	int n = 22;
	drawTexture(X, Y, n);
}

void uran(int X, int Y) // Uran
{
	texture[0] = "qqqqqqqqq__-------__";
	texture[1] = "qqqqqq__-           -_";
	texture[2] = "qqqq_-                 -_";
	texture[3] = "qqq/                     \\";
	texture[4] = "qq/                       \\";
	texture[5] = "q|                         |";
	texture[6] = "q|                         |";
	texture[7] = "q|                         |";
	texture[8] = "|                           |";
	texture[9] = "|                           |";
	texture[10] = "|                           |";
	texture[11] = "|                           |";
	texture[12] = "|                           |";
	texture[13] = "q|                         |";
	texture[14] = "q|                         |";
	texture[15] = "q|                         |";
	texture[16] = "qq\\                       /";
	texture[17] = "qqq\\                     /";
	texture[18] = "qqqq--_               _--";
	texture[19] = "qqqqqqq--__       __--";
	texture[20] = "qqqqqqqqqqq-------";
	int n = 21;
	drawTexture(X, Y, n);
}

void saturn(int X, int Y)
{
	texture[0] = "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq...";
	texture[1] = "qqqqqqqqqqqqqqqqqqqqqqqqqqqq.......";
	texture[2] = "qqqqqqqqqqqqqqq__-------__ ....qq..";
	texture[3] = "qqqqqqqqqqqq__-           -_.qqqq.";
	texture[4] = "qqqqqqqqqq_-       / /       -_..q.";
	texture[5] = "qqqqqqqqq/         /  /       .\\q.";
	texture[6] = "qqqqqqqq/      / /       /  ... \\";
	texture[7] = "qqqqqqq|          / /      . .   |";
	texture[8] = "qqqqqqq|   // /   /    .. .      |";
	texture[9] = "qqqqqqq|  /           .. ....    |";
	texture[10] = "qqqqqq|  //         . . .  .      |";
	texture[11] = "qqqqqq|  /        .... .          |";
	texture[12] = "qqqqqq|  /         .  .        /  |";
	texture[13] = "qqqqqq|         . . .       / /   |";
	texture[14] = "qqqqq.|       .  . .       /   /  |";
	texture[15] = "qqq..qq|    . .. ..      / //    |";
	texture[16] = "qqq..qq|   . ..          /       |";
	texture[17] = "qq..qqq| .. ..     /    /  /     |";
	texture[18] = "q.q.qqq.\\  .         /  /       /";
	texture[19] = ".qqq.q.qq\\.   /   /    /       /";
	texture[20] = "..qqq.....--_   / / /       _--";
	texture[21] = "qqq..q..qqqqq--__       __--";
	texture[22] = "qqqqqqqqqqqqqqqqq-------";
	int n = 23;
	drawTexture(X, Y, n);
}

void neptune(int X, int Y)
{
	texture[0] = "q";
	texture[1] = "qqqqqqqqq__-------__";
	texture[2] = "qqqqqq__-           -_";
	texture[3] = "qqqq_-     _ _-        -_";
	texture[4] = "qqq/   ---- -            \\";
	texture[5] = "qq/                       \\";
	texture[6] = "q|                         |";
	texture[7] = "q|          =-             |";
	texture[8] = "q|                         |";
	texture[9] = "|              __           |";
	texture[10] = "|             _\\_\\__        |";
	texture[11] = "|              -     --     |";
	texture[12] = "|                           |";
	texture[13] = "|            .              |";
	texture[14] = "q|  =                      |";
	texture[15] = "q|                         |";
	texture[16] = "q|                         |";
	texture[17] = "qq\\ o                     /";
	texture[18] = "qqq\\     __---------___  /";
	texture[19] = "qqqq--_--     __      _--";
	texture[20] = "qqqqqqq--__       __--";
	texture[21] = "qqqqqqqqqqq-------";
	drawTexture(X, Y, 22);
}

void pluto(int X, int Y)
{
	texture[0] = "qqq__---__";
	texture[1] = "qq/    .  \\";
	texture[2] = "q| . . . . |";
	texture[3] = "|  . .  :  :|";
	texture[4] = "| . . .__: _|";
	texture[5] = "|  .. /  \\/ |";
	texture[6] = "q|. . |    |";
	texture[7] = "qq\\__/  __/";
	texture[8] = "qqqqq---";
	drawTexture(X, Y, 9);
}

void voyager(int X, int Y)
{
	texture[0] = "qo_";
	texture[1] = "qq\\|";
	texture[2] = "qq | /|";
	texture[3] = "qq<|| |";
	texture[4] = "qq/| \\|";
	texture[5] = "q/q|";
	texture[6] = "/";

	drawTexture(X, Y, 7);
}

void borderText(int X, int Y, string text)
{
	for (int i = 0; i < text.size(); i += 1)
	{
		if (i + X < widthV)
			screen[Y * widthV + X + i] = text[i];
	}
}

void trail()
{
	borderText((int)x - 2, trailY[3], "%%");
	borderText((int)x - 2, trailY[3] + 2, "%%");
	borderText((int)x - 2, trailY[3] + 4, "%%");

	borderText((int)x - 4, trailY[2], "==");
	borderText((int)x - 4, trailY[2] + 2, "==");
	borderText((int)x - 4, trailY[2] + 4, "==");

	borderText((int)x - 5, trailY[1], "-");
	borderText((int)x - 5, trailY[1] + 2, "-");
	borderText((int)x - 5, trailY[1] + 4, "-");

	int temp1 = rand() % 3;
	int temp2 = rand() % 3;
	int temp3 = rand() % 3;

	if (temp1)
		borderText((int)x - 6, trailY[1] + 2, "-");
	else
		borderText((int)x - 4, trailY[2] + 2, "-");

	if (temp2)
		borderText((int)x - 6, trailY[1] + 4, "-");
	else
		borderText((int)x - 4, trailY[2] + 4, "-");

	if (temp3)
		borderText((int)x - 6, trailY[1], "-");
	else
		borderText((int)x - 4, trailY[2], "-");
}

void particleHit(int X, int Y, int num)
{
	if (num == 0)
	{
		screen[Y * widthV + X] = '#';
	}
	if (num == 1)
	{
		screen[Y * widthV + X + 1] = '%';
		screen[Y * widthV + X - 1] = '%';
		screen[(Y + 1) * widthV + X] = '%';
		screen[(Y - 1) * widthV + X] = '%';
	}
	if (num == 2)
	{
		screen[Y * widthV + X + 1] = '=';
		screen[Y * widthV + X - 1] = '=';
		screen[(Y + 1) * widthV + X] = '=';
		screen[(Y - 1) * widthV + X] = '=';
	}
	if (num == 3)
	{
		screen[Y * widthV + X + 1] = '+';
		screen[Y * widthV + X - 1] = '-';
		screen[(Y + 1) * widthV + X] = '+';
		screen[(Y - 1) * widthV + X] = '+';
	}
}

void emptyBuffer()
{
	for (int i = 0; i < heightV; i += 1)
	{
		for (int j = 0; j < widthV; j += 1)
		{
			screen[i * widthV + j] = ' ';
		}
	}
}

void render()
{
	DWORD dwBytesWritten = 0; // Для дебага

	screen[widthV * heightV - 1] = '\0'; // end

	WriteConsoleOutputCharacter(hConsole, screen, widthV * heightV, { 0, 0 }, &dwBytesWritten); // Запись в буфер

	emptyBuffer(); // empty
}

int isUfoColided(int X, int Y)
{
	for (int i = 0; i < 13; i += 1)
	{
		if (screen[X + ufoColisionX[i] + (Y + ufoColisionY[i]) * widthV] == '=')
		{
			return i;
		}
	}
	return 0;
}

int isColided()
{
	for (int i = 0; i < 21; i += 1)
	{
		if (screen[(int)(y + shipCoordY[i]) * widthV + (int)x + shipCoordX[i]] != ' ' && screen[(int)(y + shipCoordY[i]) * widthV + (int)x + shipCoordX[i]] != '-')
		{
			colisionX = shipCoordX[i];
			colisionY = shipCoordY[i];
			return i;
		}
	}
	return 0;
}

void asteroidExplosion(int X, int Y, int type, int pos)
{
	int n = 0;
	if (type == 0 || type == 4 || type == 5)
	{
		if (pos == 0)
		{
			texture[0] = "_/\\";
			texture[1] = "\\# |";
			texture[2] = "|   #";
			texture[3] = "[    >";
			texture[4] = "|# #/";
			texture[5] = " \\/";
			n = 6;
		}
		if (pos == 1)
		{
			texture[0] = "_#\\";
			texture[1] = "# #|#";
			texture[2] = "|# # #";
			texture[3] = "[# ##>";
			texture[4] = "# # #";
			texture[5] = " #/#";
			n = 6;
		}
		if (pos == 2)
		{
			texture[0] = "_=#";
			texture[1] = "= =#=";
			texture[2] = "|= = =";
			texture[3] = "#= ==>";
			texture[4] = "= = =";
			texture[5] = " =/=";
			n = 6;
		}
		if (pos == 3)
		{
			texture[0] = "_+=";
			texture[1] = "+ +=+";
			texture[2] = "-+ - +";
			texture[3] = "=+ ++>";
			texture[4] = "+ + +";
			texture[5] = " +/+";
			n = 6;
		}
		if (pos == 4)
		{
			texture[0] = "+q=";
			texture[1] = "q-q=";
			texture[2] = "-qq-";
			texture[3] = "+q-qq+";
			texture[4] = "q";
			texture[5] = "q-+";
			n = 6;
		}
		if (pos == 5)
		{
			texture[0] = "q";
			texture[1] = "qqq+";
			texture[2] = "q";
			texture[3] = "+qqqqq";
			texture[4] = "q";
			texture[5] = "qq+";
			n = 6;
		}

	}
	if (type == 1)
	{
		if (pos == 0)
		{
			texture[0] = "qq_";
			texture[1] = "q#.\\";
			texture[2] = "|  .#";
			texture[3] = "q\\_/";
			n = 4;
		}
		if (pos == 1)
		{
			texture[0] = " #_";
			texture[1] = "#%#\#";
			texture[2] = "|# #%#";
			texture[3] = " \%/#";
			n = 4;
		}
		if (pos == 2)
		{
			texture[0] = " %=";
			texture[1] = "%=%+%";
			texture[2] = "|% %=%";
			texture[3] = " -=-%";
			n = 4;
		}
		if (pos == 3)
		{
			texture[0] = " =+";
			texture[1] = "=+=-=";
			texture[2] = "-= =+=";
			texture[3] = "  - =";
			n = 4;
		}
		if (pos == 4)
		{
			texture[0] = "+";
			texture[1] = "q-q+";
			texture[2] = "+q+";
			texture[3] = "q+";
			n = 4;
		}
		if (pos == 5)
		{
			texture[0] = "q";
			texture[1] = "qqq+";
			texture[2] = "q";
			texture[3] = "q+";
			n = 4;
		}
	}
	if (type == 2 || type == 3)
	{
		if (pos == 0)
		{
			texture[0] = " ___";
			texture[1] = "/ .#\\";
			texture[2] = "|   .\\";
			texture[3] = " #_ #|";
			texture[4] = "   \\/";
			n = 5;
		}
		if (pos == 1)
		{
			texture[0] = " __#";
			texture[1] = "/ #%#";
			texture[2] = "|# ##\\";
			texture[3] = "#%##%#";
			texture[4] = " # \\#";
			n = 5;
		}
		if (pos == 2)
		{
			texture[0] = " __%";
			texture[1] = "= +=%";
			texture[2] = "|# %+=";
			texture[3] = "%=%+=%";
			texture[4] = " % =%";
			n = 5;
		}
		if (pos == 3)
		{
			texture[0] = "qq-=";
			texture[1] = "-  +=";
			texture[2] = "-= +-+";
			texture[3] = "=+= =";
			texture[4] = " = +=";
			n = 5;
		}
		if (pos == 4)
		{
			texture[0] = "qq+ =";
			texture[1] = "-=";
			texture[2] = "qq=";
			texture[3] = "q=qq-";
			n = 5;
		}
		if (pos == 5)
		{
			texture[0] = "qq+";
			texture[1] = "q";
			texture[2] = "q";
			texture[3] = "q+";
			n = 5;
		}
	}
	drawTexture(X, Y, n);
}

void isAsteroidColided()
{
	int asteroidHeight[6] = { 6,4,5,6,5,5 };
	for (int i = 0; i < bulletXp.size(); i += 1)
	{
		for (int j = 0; j < asteroidX.size(); j += 1)
		{
			for (int k = 0; k < asteroidHeight[asteroidType[j]]; k += 1)
			{
				if (debug)
					text(asteroidX[j], asteroidY[j] - 1, to_string(asteroidHp[j]) + " " + to_string(bulletXp[i]));

				if ((bulletXp[i] == asteroidX[j] || bulletXp[i] == asteroidX[j] + 1 || bulletXp[i] == asteroidX[j] + 2 || bulletXp[i] == asteroidX[j] + 3) && (bulletYp[i] == asteroidY[j] + k) && (asteroidHp[j] > 0))
				{
					asteroidHp[j] -= 1;
				}
			}
		}
	}


	for (int j = 0; j < asteroidX.size(); j += 1)
	{
		if (asteroidHp[j] <= 0)
		{
			asteroidHp[j] -= 1;
		}
		if (asteroidHp[j] <= 0)
			asteroidExplosion(asteroidX[j], asteroidY[j], asteroidType[j], abs((int)asteroidHp[j]) / 6);
	}
}

void demo()
{
	int k = 0;
	while (true)
	{
		for (float i = 0; i < 8; i += 0.13)
		{
			k += 1;
			text(k, 8, "-");
			if (k == widthV)
				k = 0;

			Sleep(delay);
			text(2, 2, "This is text with variable : delay = " + to_string(delay));
			string temp = "Demo";
			text(widthV / 2 - temp.size() / 2, 1, temp);
			ship(10, 10);
			asteroid(20, 10, 0);
			asteroid(30, 10, 1);
			asteroid(40, 10, 2);
			asteroid(50, 10, 3);
			asteroid(60, 10, 4);
			asteroid(70, 10, 5);
			moon(10, 20);
			ufo(40, 20, 0);
			ufo(40, 30, 1);
			shipExplosion(10, 40, 0);
			shipExplosion(20, 40, 1);
			shipExplosion(30, 40, 2);
			shipExplosion(40, 40, 3);
			shipExplosion(50, 40, 4);
			shipExplosion(60, 40, 5);
			shipExplosion(70, 40, 6);
			shipExplosion(80, 40, 7);
			shipExplosion(90, 40, 8);
			shipExplosion(100, 40, i);

			// border test
			moon(widthV - 10, 10);
			ship(-3, 40);

			uran(2, height - 26);

			render();
		}
	}
}

void play()
{
	float totalTime=0, totalTimes=0, shootCooldown = 5;
	game += 1;
	while (true)
	{
		auto begin = chrono::steady_clock::now(); // timer

		int startHp = hp;
		if (event == 0)
			len += 1;

		frame += 1;
		if (frame == 101)
			frame = 1;

		invincible -= 1;

		// controls
		if (GetAsyncKeyState((unsigned short)'W') && y > 1)
			y -= 0.5;
		if (GetAsyncKeyState((unsigned short)'S') && y < height - 6)
			y += 0.5;
		if (GetAsyncKeyState((unsigned short)'A') && x > 0 /*&& !GetAsyncKeyState((unsigned short)'W') && !GetAsyncKeyState((unsigned short)'S')*/)
			x -= 0.4;
		if (GetAsyncKeyState((unsigned short)'D') && x < width - 8 /*&& !GetAsyncKeyState((unsigned short)'W') && !GetAsyncKeyState((unsigned short)'S')*/)
			x += 0.4;
		if (GetAsyncKeyState((unsigned short)32) && shootCooldown < 0)
		{
			bulletXp.push_back(x + 7);
			bulletYp.push_back(y + 2);
			shootCooldown = 10;
		}
		shootCooldown -= 1;
		if (_kbhit())
			button = getKey();
		else
			button = "no button lmao";

		// trailY
		trailY.push_back(y);
		trailY.erase(trailY.begin());
		// generating dots
		int temp;
		if (event)
			temp = 40;
		else
			temp = 15;
		if (frame % temp == 0)
		{
			dotX.push_back(widthV);
			dotSpeed.push_back(0);
			dotY.push_back(rand() % (heightV - 2) + 1);
			dotX.push_back(widthV);
			dotSpeed.push_back(1);
			dotY.push_back(rand() % (heightV - 2) + 1);
		}
		// generating asteroids
		if (len < asteroidBeltEnd && len > asteroidBeltStart)
			temp = 15;
		else
			temp = 60;
		if (customAsteroidSpawnRate > 0)
		{
			temp = customAsteroidSpawnRate;
		}
		if (len % temp == 0 && len != moonSpawn)
		{
			if (len < asteroidBeltEnd && len > asteroidBeltStart && rand() % 2 == 0)
			{
				asteroidY.push_back(rand() % (height - 8) + 1);
				asteroidX.push_back(width + rand() % 10 + 8);
				temp = rand() % 6;
				asteroidType.push_back(temp);
				asteroidHp.push_back(1);
			}
			asteroidY.push_back(rand() % (height - 8) + 1);
			asteroidX.push_back(width);
			temp = rand() % 6;
			asteroidType.push_back(temp);
			asteroidHp.push_back(1);
		}
		// putting Moon
		if (len == moonSpawn)
		{
			planetX = widthV;
			planetY = heightV / 4;
			planetType = "moon";
		}
		// putting Mars
		if (len == marsSpawn)
		{
			planetX = widthV;
			planetY = heightV / 4;
			planetType = "mars";
		}
		// putting Jupiter
		if (len == jupiterSpawn)
		{
			planetX = widthV;
			planetY = heightV / 4;
			planetType = "jupiter";
		}
		// putting Saturn
		if (len == saturnSpawn)
		{
			planetX = widthV;
			planetY = heightV / 3;
			planetType = "saturn";
		}
		// putting Uranus
		if (len == uranusSpawn)
		{
			planetX = widthV;
			planetY = heightV - 25;
			planetType = "uranus";
		}
		// putting Uranus
		if (len == neptuneSpawn)
		{
			planetX = widthV;
			planetY = heightV / 4;
			planetType = "neptune";
		}
		// putting Uranus
		if (len == plutoSpawn)
		{
			planetX = widthV;
			planetY = heightV / 4;
			planetType = "pluto";
		}
		// putting voyager 2
		if (len == voyagerSpawn)
		{
			voyagerX = width;
			voyagerY = height * 0.75;
		}

		// moving player bullets
		for (int i = 0; i < bulletXp.size(); i += 1)
		{
			if (bulletXp[i] > widthV - 2)
			{
				bulletXp.erase(bulletXp.begin() + i);
				bulletYp.erase(bulletYp.begin() + i);
			}
			else
			{
				bulletXp[i] += 2;
			}
		}
		// moving dots
		for (int i = 0; i < dotX.size(); i += 1)
		{
			if (dotX[i] <= 0)
			{
				dotX.erase(dotX.begin() + i);
				dotY.erase(dotY.begin() + i);
				dotSpeed.erase(dotSpeed.begin() + i);
			}
			else if (dotSpeed[i] == 1)
			{
				dotX[i] -= 1;
			}
			else if (dotSpeed[i] == 0 && len % 2 == 0)
			{
				dotX[i] -= 1;
			}
		}
		// moving asteroids (tag)
		for (int i = 0; i < asteroidX.size(); i += 1)
		{
			if (asteroidX[i] > -10 && asteroidHp[i] > -50)
				asteroidX[i] -= 1;
			else
			{
				asteroidX.erase(asteroidX.begin() + i);
				asteroidY.erase(asteroidY.begin() + i);
				asteroidType.erase(asteroidType.begin() + i);
				asteroidHp.erase(asteroidHp.begin() + i);
			}
		}
		// moving planet
		if (planetX > -70 && len % 2 == 0)
		{
			planetX -= 1;
		}
		// moving voyager
		if (voyagerX > -10 && len % 2 == 0)
		{
			voyagerX -= 1;
		}

		// putting planet on screen, zLayer = -1, bg
		if (planetType == "moon")
			moon(planetX, planetY);
		if (planetType == "mars")
			mars(planetX, planetY);
		if (planetType == "uranus")
			uran(planetX, planetY);
		if (planetType == "saturn")
			saturn(planetX, planetY);
		if (planetType == "jupiter")
			jupiter(planetX, planetY);
		if (planetType == "neptune")
			neptune(planetX, planetY);
		if (planetType == "pluto")
			pluto(planetX, planetY);
		if (len > voyagerSpawn && len - widthV - 10 > voyagerX)
			voyager(voyagerX, voyagerY);
		// putting dots on screen, bg layer
		for (int i = 0; i < dotX.size(); i += 1)
			screen[dotY[i] * widthV + dotX[i]] = '-';
		// putting asteroids on screen, top layer
		for (int i = 0; i < asteroidX.size(); i += 1)
		{
			if (asteroidHp[i] > 0)
				asteroid(asteroidX[i], asteroidY[i], asteroidType[i]);
		}
		// putting player bullets on screen
		for (int i = 0; i < bulletXp.size(); i += 1)
			text(bulletXp[i], bulletYp[i], "=");
		// putting martian madness hud and main logic
		if (len >= martianSpawn && len <= martianSpawn + 100 && martianMadnessProgress <= 100)
		{
			if (hp == 0)
			{
				str = "You died!";
				text(widthV / 2 - str.size() / 2, height / 2, str);
				str = "Press f to restart";
				text(widthV / 2 - str.size() / 2, height / 2 + 2, str);
				button = "nobutton";
				if (GetAsyncKeyState((unsigned short)'F'))
				{
					hp = 3;
					x = widthV / 4;
					y = height / 2;
					wave = 0;
					set2 = 0;
					set3 = 0;
					set4 = 0;
					martianMadnessProgress = 0;
					crash = -1;
				}
			}
			if (wave == 0)
			{
				hpe1 = 3;
				victory = 30;
				invincibleE1 = 0;
				cooldown = 0;
				entityColisionX1 = 0;
				entityColisionY1 = 0;
				crashE1 = -1;
				colisionTime = -1;
				entityX1 = widthV;
				entityY1 = height / 4;
				ufo1Explosion = 10;
				ufo2Explosion = 10;
				wave = 1;
			}
			if (wave == 1)
			{
				if (entityX1 > widthV / 5 * 4) // entering cutscene
				{
					entityX1 -= 0.2;
				}
				else // atack and move 
				{
					cooldown -= 1;
					if (!(frame % 20) && cooldown < 0)
					{
						if (hpe1 > 0)
						{
							bulletXe1.push_back(entityX1);
							bulletYe1.push_back(entityY1 + 2);
						}
					}
					if (frame % 80 == 0)
					{
						cooldown = 60;
					}
					// bullet logic
					for (int i = 0; i < bulletXe1.size(); i += 1)
					{
						if (bulletXe1[i] < 0)
						{
							bulletXe1.erase(bulletXe1.begin() + i);
							bulletYe1.erase(bulletYe1.begin() + i);
						}
						else
						{
							bulletXe1[i] -= 1.5;
						}
					}
					// checking ufo1 colision
					int temp = 0;
					if (hpe1 > 0)
						temp = isUfoColided(entityX1, entityY1);
					if (temp != 0 && invincibleE1 < 0)
					{
						entityColisionX1 = ufoColisionX[temp] + entityX1;
						entityColisionY1 = ufoColisionY[temp] + entityY1;
						invincibleE1 = 5;
						crashE1 = 0;
						hpe1 -= 1;
						if (hpe1 == 0)
							ufo1Explosion = 0;
					}
					invincibleE1 -= 1;
					if (crashE1 <= 3 && crashE1 != -1)
						crashE1 += 0.2;
					else
						crashE1 = -1;
					// putting ufo bullets
					for (int i = 0; i < bulletXe1.size(); i += 1)
						text(bulletXe1[i], bulletYe1[i], "=");
				}
				ufo1Explosion += 0.2;
				// putting ufo
				if (hpe1 > 0)
				{
					ufo(entityX1, entityY1, 1);
				}
				else
				{
					ufoExplosion(entityX1, entityY1, ufo1Explosion);
				}
				if (hpe1 <= 0)
				{
					martianMadnessProgress = 10;
					victory -= 1;
				}
				if (victory == 0)
					wave = 2;

				// putting hit marker
				if (hpe1 > 0)
					particleHit(entityColisionX1, entityColisionY1, (int)crashE1);
			}
			if (wave == 2 && set2 == 0)
			{
				hpe1 = 3;
				victory = 30;
				invincibleE1 = 0;
				invincibleE2 = 0;
				cooldown = 0;
				entityColisionX1 = 0;
				entityColisionX2 = 0;
				entityColisionY1 = 0;
				entityColisionY2 = 0;
				crashE1 = -1;
				crashE2 = -1;
				colisionTime = -1;
				entityX1 = widthV;
				entityX2 = widthV;
				entityY1 = height / 4;
				entityY2 = height / 4 * 3;
				ufo1Explosion = 10;
				ufo2Explosion = 10;
				set2 = 1;
			}
			if (wave == 2)
			{
				if (entityX1 > widthV / 5 * 4) // entering cutscene
				{
					entityX1 -= 0.2;
					entityX2 -= 0.2;
				}
				else // atack and move 
				{
					cooldown -= 1;
					if (!(frame % 20) && cooldown < 0)
					{
						if (hpe1 > 0)
						{
							bulletXe1.push_back(entityX1);
							bulletYe1.push_back(entityY1 + 2);
						}
						if (hpe2 > 0)
						{
							bulletXe2.push_back(entityX2);
							bulletYe2.push_back(entityY2 + 2);
						}
					}
					if (frame % 80 == 0)
					{
						cooldown = 60;
					}
					// bullet logic
					for (int i = 0; i < bulletXe1.size(); i += 1)
					{
						if (bulletXe1[i] < 0)
						{
							bulletXe1.erase(bulletXe1.begin() + i);
							bulletYe1.erase(bulletYe1.begin() + i);
						}
						else
						{
							bulletXe1[i] -= 1.5;
						}
					}
					for (int i = 0; i < bulletXe2.size(); i += 1)
					{
						if (bulletXe2[i] < 0)
						{
							bulletXe2.erase(bulletXe2.begin() + i);
							bulletYe2.erase(bulletYe2.begin() + i);
						}
						else
						{
							bulletXe2[i] -= 1.5;
						}
					}
					// checking ufo1 colision
					int temp = 0;
					if (hpe1 > 0)
						temp = isUfoColided(entityX1, entityY1);
					if (temp != 0 && invincibleE1 < 0)
					{
						entityColisionX1 = ufoColisionX[temp] + entityX1;
						entityColisionY1 = ufoColisionY[temp] + entityY1;
						invincibleE1 = 5;
						crashE1 = 0;
						hpe1 -= 1;
						if (hpe1 == 0)
							ufo1Explosion = 0;
					}
					invincibleE1 -= 1;
					if (crashE1 <= 3 && crashE1 != -1)
						crashE1 += 0.2;
					else
						crashE1 = -1;
					// checking ufo2 colision
					if (hpe2 > 0)
						temp = isUfoColided(entityX2, entityY2);
					if (temp != 0 && invincibleE2 < 0)
					{
						entityColisionX2 = ufoColisionX[temp] + entityX2;
						entityColisionY2 = ufoColisionY[temp] + entityY2;
						invincibleE2 = 5;
						crashE2 = 0;
						hpe2 -= 1;
						if (hpe2 == 0)
							ufo2Explosion = 0;
					}
					invincibleE2 -= 1;
					if (crashE2 <= 3 && crashE2 != -1)
						crashE2 += 0.2;
					else
						crashE2 = -1;
					// putting ufo bullets
					for (int i = 0; i < bulletXe1.size(); i += 1)
						text(bulletXe1[i], bulletYe1[i], "=");
					for (int i = 0; i < bulletXe2.size(); i += 1)
						text(bulletXe2[i], bulletYe2[i], "=");
				}
				ufo1Explosion += 0.2;
				ufo2Explosion += 0.2;
				// putting ufo
				if (hpe1 > 0)
				{
					ufo(entityX1, entityY1, 1);
				}
				else
				{
					ufoExplosion(entityX1, entityY1, ufo1Explosion);
				}
				if (hpe2 > 0)
				{
					ufo(entityX2, entityY2, 1);
				}
				else
				{
					ufoExplosion(entityX2, entityY2, ufo2Explosion);
				}
				if (hpe1 <= 0 || hpe2 <= 0)
					martianMadnessProgress = 20;
				if (hpe1 <= 0 && hpe2 <= 0)
				{
					martianMadnessProgress = 30;
					victory -= 1;
				}
				if (victory == 0)
					wave = 3;

				// putting hit marker
				if (hpe1 > 0)
					particleHit(entityColisionX1, entityColisionY1, (int)crashE1);
				if (hpe2 > 0)
					particleHit(entityColisionX2, entityColisionY2, (int)crashE2);
			}
			if (wave == 3 && set3 == 0)
			{
				hpe1 = 3;
				hpe2 = 3;
				victory = 30;
				invincibleE1 = 0;
				invincibleE2 = 0;
				cooldown = 0;
				entityColisionX1 = 0;
				entityColisionX2 = 0;
				entityColisionY1 = 0;
				entityColisionY2 = 0;
				crashE1 = -1;
				crashE2 = -1;
				colisionTime = -1;
				entityX1 = widthV;
				entityX2 = widthV;
				entityY1 = height / 4;
				entityY2 = height / 4 * 3;
				ufo1Explosion = 10;
				ufo2Explosion = 10;
				set3 = 1;
			}
			if (wave == 3)
			{
				if (entityX1 > widthV / 5 * 4) // entering cutscene
				{
					entityX1 -= 0.2;
				}
				else // atack and move 
				{
					if (entityY1 < y)
						entityY1 += 0.1;
					if (entityY1 > y)
						entityY1 -= 0.1;
					cooldown -= 1;
					if (!(frame % 20) && cooldown < 0)
					{
						if (hpe1 > 0)
						{
							bulletXe1.push_back(entityX1);
							bulletYe1.push_back(entityY1 + 2);
						}
					}
					if (frame % 80 == 0)
					{
						cooldown = 60;
					}
					// bullet logic
					for (int i = 0; i < bulletXe1.size(); i += 1)
					{
						if (bulletXe1[i] < 0)
						{
							bulletXe1.erase(bulletXe1.begin() + i);
							bulletYe1.erase(bulletYe1.begin() + i);
						}
						else
						{
							bulletXe1[i] -= 1.5;
						}
					}
					// checking ufo1 colision
					int temp = 0;
					if (hpe1 > 0)
						temp = isUfoColided(entityX1, entityY1);
					if (temp != 0 && invincibleE1 < 0)
					{
						entityColisionX1 = ufoColisionX[temp] + entityX1;
						entityColisionY1 = ufoColisionY[temp] + entityY1;
						invincibleE1 = 30;
						crashE1 = 0;
						hpe1 -= 1;
						if (hpe1 == 0)
							ufo1Explosion = 0;
					}
					invincibleE1 -= 1;
					if (crashE1 <= 3 && crashE1 != -1)
						crashE1 += 0.2;
					else
						crashE1 = -1;
					// putting ufo bullets
					for (int i = 0; i < bulletXe1.size(); i += 1)
						text(bulletXe1[i], bulletYe1[i], "=");
				}
				ufo1Explosion += 0.2;
				// putting ufo
				if (hpe1 > 0)
				{
					ufo(entityX1, entityY1, 1);
				}
				else
				{
					ufoExplosion(entityX1, entityY1, ufo1Explosion);
				}
				if (hpe1 <= 0)
				{
					martianMadnessProgress = 75;
					victory -= 1;
				}
				if (victory == 0)
					wave = 4;

				// putting hit marker
				if (hpe1 > 0)
					particleHit(entityColisionX1, entityColisionY1, (int)crashE1);
			}
			if (wave == 4 && set4 == 0)
			{
				hpe1 = 10;
				victory = 30;
				invincibleE1 = 0;
				cooldown = 0;
				entityColisionX1 = 0;
				entityColisionY1 = 0;
				crashE1 = -1;
				colisionTime = -1;
				entityX1 = widthV;
				entityY1 = height / 4;
				ufo1Explosion = 10;
				set4 = 1;
			}
			if (wave == 4)
			{
				str = "Martian boss " + to_string(hpe1);
				text(widthV / 2 - str.size() / 2, 2, str);
				str = "<";
				for (int i = 0; i < 10; i += 1)
				{
					if (hpe1 > i)
						str += "=";
					else
						str += "-";
				}
				str += ">";
				text(widthV / 2 - str.size() / 2, 3, str);
				if (entityX1 > widthV / 5 * 4) // entering cutscene
				{
					entityX1 -= 0.25;
				}
				else // atack and move 
				{
					if (entityY1 < y)
						entityY1 += 0.1;
					if (entityY1 > y)
						entityY1 -= 0.1;
					cooldown -= 1;
					if (!(frame % 20) && cooldown < 0)
					{
						if (hpe1 > 0)
						{
							bulletXe1.push_back(entityX1);
							bulletYe1.push_back(entityY1 + 2);
						}
					}
					if (frame % 80 == 0)
					{
						cooldown = 50;
					}
					// bullet logic
					for (int i = 0; i < bulletXe1.size(); i += 1)
					{
						if (bulletXe1[i] < 0)
						{
							bulletXe1.erase(bulletXe1.begin() + i);
							bulletYe1.erase(bulletYe1.begin() + i);
						}
						else
						{
							bulletXe1[i] -= 1.5;
						}
					}
					// checking ufo1 colision
					int temp = 0;
					if (hpe1 > 0)
						temp = isUfoColided(entityX1, entityY1);
					if (temp != 0 && invincibleE1 < 0)
					{
						entityColisionX1 = ufoColisionX[temp] + entityX1;
						entityColisionY1 = ufoColisionY[temp] + entityY1;
						invincibleE1 = 30;
						crashE1 = 0;
						hpe1 -= 1;
						if (hpe1 == 0)
							ufo1Explosion = 0;
					}
					invincibleE1 -= 1;
					if (crashE1 <= 3 && crashE1 != -1)
						crashE1 += 0.2;
					else
						crashE1 = -1;
					// putting ufo bullets
					for (int i = 0; i < bulletXe1.size(); i += 1)
						text(bulletXe1[i], bulletYe1[i], "=");
				}
				ufo1Explosion += 0.2;
				// putting ufo
				if (hpe1 > 0)
				{
					ufo(entityX1, entityY1, 1);
				}
				else
				{
					ufoExplosion(entityX1, entityY1, ufo1Explosion);
				}
				if (hpe1 <= 0)
				{
					martianMadnessProgress = 100;
					victory -= 1;
				}
				if (victory == 0)
					wave = 5;

				// putting hit marker
				if (hpe1 > 0)
					particleHit(entityColisionX1, entityColisionY1, (int)crashE1);
			}
			martianLength += 1;
			showLength = 0;
			event = 1;
			if (wave != 5)
				str = "Martian madness! - " + to_string((int)martianMadnessProgress) + "%, wave - " + to_string(wave);
			else
			{
				if (victoryEnd < -10)
					victoryEnd = 200;
				str = "Ship hp restored!";
				text(widthV / 2 - str.size() / 2, height - 6, str);
				str = "Martian madness! - completed";
				hp = 3;
			}
			victoryEnd -= 1;
			if (victoryEnd == 0)
			{
				martianMadnessProgress = 101;
				showLength = 1;
				event = 0;
				len = martianSpawn + 1000;
			}
			text(widthV / 2 - str.size() / 2, height - 4, str);
			str = "<";
			for (int i = 0; i < 25; i += 1)
			{
				if (i < martianMadnessProgress / 4)
					str += "=";
				else
					str += "-";
			}
			str += ">";
			text(widthV / 2 - str.size() / 2, height - 3, str);
		}
		if (len < asteroidBeltEnd && len > asteroidBeltStart)
		{
			str = "Asteroid belt";
			text(widthV / 2 - str.length() / 2, height - 3, str);
		}
		// colision detection, before ship on screen
		if (invincible < 0)
		{
			colisionNum = isColided();
		}
		if (colisionNum != 0)
		{
			colisionTime += 0.2;
		}
		else
		{
			colisionTime = -1;
		}

		// bullet and asteroid colision
		isAsteroidColided();

		if (colisionNum != 0 && invincible < 0 && hp > 0)
		{
			hp -= 1;
			invincible = 30;
		}
		//putting ship, top layer
		if (hp > 0)
		{
			trail();
			ship(x, y);
		}
		else // crash
		{
			crash += 0.2;
			if (crash < 8 && crash > -1)
				shipExplosion(x - 2, y - 2, (int)crash);
			x -= 0.25;
		}

		particleHit(x + shipCoordX[colisionNum], y + shipCoordY[colisionNum], (int)colisionTime); // hit particle


		/// info ///
		if (showLength)
		{
			str = "Distance : " + to_string(len) + ", ship health : " + to_string(hp);
			text(widthV / 2 - str.length() / 2, height - 1, str);
		}
		else
		{
			str = "Ship health : " + to_string(hp);
			text(widthV / 2 - str.length() / 2, height - 1, str);
		}

		/// Sleep(rand()%(delay-1)); // lag creator for test

		/// final

		auto end = chrono::steady_clock::now();// timer end
		auto elapsed_ms = chrono::duration_cast<chrono::milliseconds>(end - begin);
		int frameRenderTime = elapsed_ms.count(); // anti lag
		if (frameRenderTime > 0)
			text(0, 0, "!");
		text(0, 0, " Frame render time: " + to_string(frameRenderTime) + " ms, 60 frames delay : " + to_string(totalTime));

		totalTime += frameRenderTime;
		totalTimes += 1;
		if (totalTimes == 60)
		{
			totalTime = 0;
		}

		if (delay - frameRenderTime > 1)
			Sleep(delay - frameRenderTime);
		else
			Sleep(1);

		// Sleep(delay);

		// planet names
		if (planetX > -30)
		{
			if (planetType == "moon")
				str = "Moon";
			else if (planetType == "mars")
				str = "Mars";
			else if (planetType == "uranus")
				str = "Uranus(Yes it is circle)";
			else if (planetType == "saturn")
				str = "Saturn";
			else if (planetType == "jupiter")
				str = "Jupiter";
			else if (planetType == "neptune")
				str = "Neptune";
			else if (planetType == "pluto")
				str = "Pluto";
			else
				str = "";
			text(widthV / 2 - str.length() / 2, height - 4, str);
		}

		if (GetAsyncKeyState((unsigned short)27))
		{
			str = "Paused";
			text(widthV / 2 - str.length() / 2, height / 2, str);
			str = "Press enter to exit";
			text(widthV / 2 - str.length() / 2, height / 2 + 2, str);
			render();
			button = getKey();
			if (button == "enter")
				mainMenu();
		}

		/// death ///
		if (hp <= 0)
		{
			str = "You died!";
			text(widthV / 2 - str.length() / 2, height / 2, str);
			str = "Enter - try again, esc - main menu";
			text(widthV / 2 - str.length() / 2, height / 2 + 2, str);
			if (_kbhit())
				button = getKey();
			if (button == "enter")
			{
				reset();
				play();
			}
			if (button == "esc")
			{
				game = 0;
				reset();
				mainMenu();
			}
		}

		render();
	}
}

void logo(int X, int Y)
{
	texture[0] = "   _____                       ___       __                 __                       ";
	texture[1] = "  / ___/____  ____ _________  /   | ____/ /   _____  ____  / /___  __________        ";
	texture[2] = "  \\__ \\/ __ \\/ __ `/ ___/ _ \\/ /| |/ __  / | / / _ \\/ __ \\/ __/ / / / ___/ _ \\";
	texture[3] = " ___/ / /_/ / /_/ / /__/  __/ ___ / /_/ /| |/ /  __/ / / / /_/ /_/ / /  /  __/       ";
	texture[4] = "/____/ .___/\\__,_/\\___/\\___/_/  |_\\__,_/ |___/\\___/_/ /_/\\__/\\__,_/_/   \\___/";
	texture[5] = "    /_/                                                                              ";
	drawTexture(X, Y, 6);
}

void settings()
{
	COORD bufferSize = { widthV, heightV };
	SMALL_RECT windowSize = { 0, 0, widthV - 1, heightV - 1 };
	emptyBuffer();
	int menu = 0;
	button = "gfdajskg";
	while (true)
	{
		Sleep(100);
		if (_kbhit())
			button = getKey();
		str = "Options";
		text(widthV / 2 - str.size() / 2, 2, str);

		if (menu == 0)
		{
			str = "> Ship health : " + to_string(hp);
			ship(widthV * 0.6, 8);
		}
		else
			str = "  Ship health : " + to_string(hp);
		text(3, 4, str);
		if (menu == 1)
		{
			fileIcon(widthV * 0.6, 7);
			str = "> Travelled distance : " + to_string(len);
		}
		else
			str = "  Travelled distance : " + to_string(len);
		text(3, 6, str);
		if (menu == 2)
		{
			str = "> Asteroid spawn rate : " + to_string(customAsteroidSpawnRate) + " (60 - normal, 30 - asteroid belt, 0 - turn off custom rate)";
		}
		else
			str = "  Asteroid spawn rate : " + to_string(customAsteroidSpawnRate);
		text(3, 8, str);
		if (menu == 3)
		{
			str = "> Moon spawn : " + to_string(moonSpawn);
			moon(widthV * 0.6, 7);
		}
		else
			str = "  Moon spawn : " + to_string(moonSpawn);
		text(3, 10, str);

		if (menu == 4)
		{
			str = "> Martian madness spawn : " + to_string(martianSpawn);
			ufo(widthV * 0.6, 3, 1);
			ufo(widthV * 0.6 + 20, 8, 1);
			ufo(widthV * 0.6 + 5, 17, 1);
			ufo(widthV * 0.6 + 8, 10, 0);
		}
		else
			str = "  Martian madness spawn : " + to_string(martianSpawn);
		text(3, 12, str);

		if (menu == 5)
		{
			str = "> Mars spawn : " + to_string(marsSpawn);
			mars(widthV * 0.6, 4);
		}
		else
			str = "  Mars spawn : " + to_string(marsSpawn);
		text(3, 14, str);

		if (menu == 6)
		{
			str = "> Asteroid belt spawn : " + to_string(asteroidBeltStart);
			asteroid(widthV * 0.6, 3, 2);
			asteroid(widthV * 0.6 + 20, 6, 3);
			asteroid(widthV * 0.6 + 10, 11, 4);
			asteroid(widthV * 0.6 + 6, 20, 1);
		}
		else
			str = "  Asteroid belt spawn : " + to_string(asteroidBeltStart);
		text(3, 16, str);

		if (menu == 7)
		{
			str = "> Jupiter spawn : " + to_string(jupiterSpawn);
			jupiter(widthV * 0.6, 3);
		}
		else
			str = "  Jupiter spawn : " + to_string(jupiterSpawn);
		text(3, 18, str);

		if (menu == 8)
		{
			str = "> Saturn spawn : " + to_string(saturnSpawn);
			saturn(widthV * 0.6 - 6, 2);
		}
		else
			str = "  Saturn spawn : " + to_string(saturnSpawn);
		text(3, 20, str);

		if (menu == 9)
		{
			str = "> Uranus spawn : " + to_string(uranusSpawn);
			uran(widthV * 0.6, 4);
		}
		else
			str = "  Uranus spawn : " + to_string(uranusSpawn);
		text(3, 22, str);

		if (menu == 10)
		{
			str = "> Neptune spawn : " + to_string(neptuneSpawn);
			neptune(widthV * 0.6, 3);
		}
		else
			str = "  Neptune spawn : " + to_string(neptuneSpawn);
		text(3, 24, str);

		if (menu == 11)
		{
			str = "> Frame delay(fps:" + to_string(1000 / delay) + ") : " + to_string(delay);
			fileIcon(widthV * 0.6, 7);
		}
		else
			str = "  Frame delay(fps:" + to_string(1000 / delay) + ") : " + to_string(delay);
		text(3, 26, str);

		str = "  Screen size(use arrows to adjust) : x=" + to_string(widthV) + " y=" + to_string(heightV);
		text(3, 28, str);

		asteroidBeltEnd = asteroidBeltStart + 1500;

		if (GetAsyncKeyState((unsigned short)'D') && hp < 1000 && menu == 0)
			hp += 1;
		if (GetAsyncKeyState((unsigned short)'A') && hp > 1 && menu == 0)
			hp -= 1;
		if (GetAsyncKeyState((unsigned short)'D') && len < 100000 && menu == 1)
			len += 100;
		if (GetAsyncKeyState((unsigned short)'A') && len > 0 && menu == 1)
			len -= 100;
		if (GetAsyncKeyState((unsigned short)'D') && customAsteroidSpawnRate < 1000 && menu == 2)
			customAsteroidSpawnRate += 1;
		if (GetAsyncKeyState((unsigned short)'A') && customAsteroidSpawnRate > 0 && menu == 2)
			customAsteroidSpawnRate -= 1;
		if (GetAsyncKeyState((unsigned short)'D') && menu == 3)
			moonSpawn += 100;
		if (GetAsyncKeyState((unsigned short)'A') && moonSpawn - 100 > 0 && menu == 3)
			moonSpawn -= 100;
		if (GetAsyncKeyState((unsigned short)'D') && menu == 4)
			martianSpawn += 100;
		if (GetAsyncKeyState((unsigned short)'A') && martianSpawn - 100 > 0 && menu == 4)
			martianSpawn -= 100;
		if (GetAsyncKeyState((unsigned short)'D') && menu == 5)
			marsSpawn += 100;
		if (GetAsyncKeyState((unsigned short)'A') && marsSpawn - 100 > 0 && menu == 5)
			marsSpawn -= 100;
		if (GetAsyncKeyState((unsigned short)'D') && menu == 6)
			asteroidBeltStart += 100;
		if (GetAsyncKeyState((unsigned short)'A') && asteroidBeltStart - 100 > 0 && menu == 6)
			asteroidBeltStart -= 100;
		if (GetAsyncKeyState((unsigned short)'D') && menu == 7)
			jupiterSpawn += 100;
		if (GetAsyncKeyState((unsigned short)'A') && jupiterSpawn - 100 > 0 && menu == 7)
			jupiterSpawn -= 100;
		if (GetAsyncKeyState((unsigned short)'D') && menu == 8)
			saturnSpawn += 100;
		if (GetAsyncKeyState((unsigned short)'A') && saturnSpawn - 100 > 0 && menu == 8)
			saturnSpawn -= 100;
		if (GetAsyncKeyState((unsigned short)'D') && menu == 9)
			uranusSpawn += 100;
		if (GetAsyncKeyState((unsigned short)'A') && uranusSpawn - 100 > 0 && menu == 9)
			uranusSpawn -= 100;
		if (GetAsyncKeyState((unsigned short)'D') && menu == 10)
			neptuneSpawn += 100;
		if (GetAsyncKeyState((unsigned short)'A') && neptuneSpawn - 100 > 0 && menu == 10)
			neptuneSpawn -= 100;
		if (GetAsyncKeyState((unsigned short)'D') && menu == 11)
			delay += 1;
		if (GetAsyncKeyState((unsigned short)'A') && delay - 1 > 0 && menu == 11)
			delay -= 1;
		if (button == "rightArrow")
		{
			widthV += 1;
		}
		if (button == "leftArrow" && widthV > 75)
		{
			widthV -= 1;
		}
		if (button == "upArrow" && heightV > 30)
		{
			heightV -= 1;
		}
		if (button == "downArrow")
		{
			heightV += 1;
		}

		width = widthV;
		height = heightV;
		bufferSize = { widthV, heightV };
		windowSize = { 0, 0, widthV - 1, heightV - 1 };
		SetConsoleScreenBufferSize(hConsole, bufferSize);
		SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

		if (button == "w" && menu > 0)
			menu -= 1;
		if (button == "s" && menu < 11)
			menu += 1;
		if (button == "esc")
			mainMenu();
		button = "gfdajskg";

		render();
	}
}

void mainMenu()
{
	string str, button;
	int menu = 0, firstTime = 1;
	emptyBuffer();
	while (true)
	{
		logo(widthV / 2 - 40, heightV / 4 - 4);

		if (!firstTime)
			button = getKey();
		firstTime = 0;

		if ((button == "w" || button == "upArrow") && menu > 0)
			menu -= 1;
		if ((button == "s" || button == "downArrow") && menu < 2)
			menu += 1;
		if (button == "enter" && menu == 0)
		{
			if (menu == 0)
			{
				bool temp = 1;
				if (game > 0)
					while (temp)
					{
						emptyBuffer();
						fileIcon(widthV / 2 - 3, height / 4);
						str = "Do you want to continue old game?";
						text(widthV / 2 - str.length() / 2, height / 2, str);
						str = "Enter - yes, esc - no";
						text(widthV / 2 - str.length() / 2, height / 2 + 2, str);
						render();
						button = getKey();

						if (button == "enter")
							play();
						else
						{
							reset();
							play();
						}
					}
				else
					play();
			}
		}
		if (button == "enter" && menu == 1)
		{
			settings();
		}
		if (button == "enter" && menu == 2)
		{
			exit(0);
		}

		if (menu == 0)
			str = "> Play <";
		else
			str = "Play";
		text(widthV / 2 - str.size() / 2, heightV / 2, str);

		if (menu == 1)
			str = "> Options <";
		else
			str = "Options";
		text(widthV / 2 - str.size() / 2, heightV / 2 + 2, str);

		if (menu == 2)
			str = "> Exit <";
		else
			str = "Exit";
		text(widthV / 2 - str.size() / 2, heightV / 2 + 4, str);

		render();
	}
}

int main()
{
	srand(time(NULL));

	reset();

	//Сначала задаем минимальные значения
	SMALL_RECT zeroWindow = { 0, 0, 20, 20 };
	COORD zBuffer = { 20, 20 };
	SetConsoleWindowInfo(hConsole, TRUE, &zeroWindow);
	SetConsoleScreenBufferSize(hConsole, zBuffer);

	//А уже потом изменяем значения на нужные нам
	COORD bufferSize = { widthV, heightV };
	SMALL_RECT windowSize = { 0, 0, widthV - 1, heightV - 1 };
	SetConsoleScreenBufferSize(hConsole, bufferSize);
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

	SetConsoleActiveScreenBuffer(hConsole); // Настройка консоли
	emptyBuffer();

	//demo();

	mainMenu();

	system("pause");
}
