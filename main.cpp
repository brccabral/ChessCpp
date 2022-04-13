#include <SFML/Graphics.hpp>
#include <time.h>
#include "Connector.cpp"
using namespace sf;

int size = 56;
Vector2f offset(28, 28);

Sprite f[32]; //figures
ref struct Globals {

	static System::String^ position = gcnew System::String("");

};

int board[8][8] =
{ -1,-2,-3,-4,-5,-3,-2,-1,
 -6,-6,-6,-6,-6,-6,-6,-6,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  6, 6, 6, 6, 6, 6, 6, 6,
  1, 2, 3, 4, 5, 3, 2, 1 };

System::String^ toChessNote(Vector2f p)
{
	System::String^ s = "";
	s += System::Char(p.x / size + 97).ToString();
	s += System::Char(7 - p.y / size + 49).ToString();
	return s;
}

Vector2f toCoord(char a, char b)
{
	int x = int(a) - 97;
	int y = 7 - int(b) + 49;
	return Vector2f(x * size, y * size);
}

void move(System::String^ str)
{
	Vector2f oldPos = toCoord(str[0], str[1]);
	Vector2f newPos = toCoord(str[2], str[3]);

	for (int i = 0; i < 32; i++)
		if (f[i].getPosition() == newPos) f[i].setPosition(-100, -100);

	for (int i = 0; i < 32; i++)
		if (f[i].getPosition() == oldPos) f[i].setPosition(newPos);

	//castling       //if the king didn't move
	if (str->Equals("e1g1")) if (Globals::position->IndexOf("e1") == -1) move("h1f1");
	if (str->Equals("e8g8")) if (Globals::position->IndexOf("e8") == -1) move("h8f8");
	if (str->Equals("e1c1")) if (Globals::position->IndexOf("e1") == -1) move("a1d1");
	if (str->Equals("e8c8")) if (Globals::position->IndexOf("e8") == -1) move("a8d8");
}

void loadPosition()
{
	int k = 0;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			int n = board[i][j];
			if (!n) continue;
			int x = abs(n) - 1;
			int y = n > 0 ? 1 : 0;
			f[k].setTextureRect(IntRect(size * x, size * y, size, size));
			f[k].setPosition(size * j, size * i);
			k++;
		}

	for (int i = 0; i < Globals::position->Length; i += 5)
		move(Globals::position->Substring(i, 4));
}

Connector* connector = new Connector();

int main()
{
	RenderWindow window(VideoMode(504, 504), "The Chess! (press SPACE)");

	connector->connectorStart();

	Texture t1;
	t1.loadFromFile("images/figures.png");
	Texture t2;
	t2.loadFromFile("images/board.png");

	for (int i = 0; i < 32; i++) f[i].setTexture(t1);
	Sprite sBoard(t2);

	loadPosition();

	bool isMove = false;
	float dx = 0, dy = 0;
	Vector2f oldPos, newPos;
	System::String^ str = "";
	int n = 0;

	while (window.isOpen())
	{
		Vector2i pos = Mouse::getPosition(window) - Vector2i(offset);

		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();

			////move back//////
			if (e.type == Event::KeyPressed)
				if (e.key.code == Keyboard::BackSpace)
				{
					if (Globals::position->Length > 6) Globals::position->Remove(Globals::position->Length - 6, 5); loadPosition();
				}

			/////drag and drop///////
			if (e.type == Event::MouseButtonPressed)
				if (e.key.code == Mouse::Left)
					for (int i = 0; i < 32; i++)
						if (f[i].getGlobalBounds().contains(pos.x, pos.y))
						{
							isMove = true; n = i;
							dx = pos.x - f[i].getPosition().x;
							dy = pos.y - f[i].getPosition().y;
							oldPos = f[i].getPosition();
						}

			if (e.type == Event::MouseButtonReleased)
				if (e.key.code == Mouse::Left)
				{
					isMove = false;
					Vector2f p = f[n].getPosition() + Vector2f(size / 2, size / 2);
					newPos = Vector2f(size * int(p.x / size), size * int(p.y / size));
					str = toChessNote(oldPos) + toChessNote(newPos);
					move(str);
					if (oldPos != newPos) {
						Globals::position += str;
						Globals::position += " ";
					}
					f[n].setPosition(newPos);
				}
		}

		//comp move
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			str = connector->getNextMove(Globals::position);

			oldPos = toCoord(str[0], str[1]);
			newPos = toCoord(str[2], str[3]);

			for (int i = 0; i < 32; i++) if (f[i].getPosition() == oldPos) n = i;

			/////animation///////
			for (int k = 0; k < 50; k++)
			{
				Vector2f p = newPos - oldPos;
				f[n].move(p.x / 50, p.y / 50);
				window.draw(sBoard);
				for (int i = 0; i < 32; i++) f[i].move(offset);
				for (int i = 0; i < 32; i++) window.draw(f[i]); window.draw(f[n]);
				for (int i = 0; i < 32; i++) f[i].move(-offset);
				window.display();
			}

			move(str);
			Globals::position += str;
			Globals::position += " ";
			f[n].setPosition(newPos);
		}

		if (isMove) f[n].setPosition(pos.x - dx, pos.y - dy);

		////// draw  ///////
		window.clear();
		window.draw(sBoard);
		for (int i = 0; i < 32; i++) f[i].move(offset);
		for (int i = 0; i < 32; i++) window.draw(f[i]); window.draw(f[n]);
		for (int i = 0; i < 32; i++) f[i].move(-offset);
		window.display();
	}

	connector->connectorClose();

	return 0;
}
