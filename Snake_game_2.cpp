#include <iostream>
#include <windows.h>
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
using namespace std;

class position
{
	public:
		int x,y;
};

void gotoxy (int x, int y)
{
	COORD co_ord;     // coordinates is declared as COORD
	co_ord.X = x;     // defining x-axis
	co_ord.Y = y;     //defining  y-axis
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),co_ord);
}

class Field
{
		char ** field;
		Field(const Field &);
	public:
		static int field_height;
		static int field_width;
		Field()
		{
			field = new char*[Field::field_height];
			for(int c = 0; c < Field::field_height; ++c)
			{
				field[c] = new char[Field::field_width];
			}
		}
		~Field()
		{
			for(int c = 0; c < Field::field_height; ++c)
			{
				delete[] field[c];
			}
			delete[] field;
		}
		void display()
		{
			gotoxy(0,0);
			for(int y = 0; y < field_height; ++y)
			{
				for(int x = 0; x < field_width; ++x)
				{
					if(x==0||x==field_width-1||y==0||y==field_height-1)
						cout<<"~";
					else
						cout << field[y][x];
				}
				cout << endl;
			}
		}
		void clear()
		{
			for(int y = 0; y < field_height; ++y)
			{
				for(int x = 0; x < field_width; ++x)
				{
					field[y][x] = ' ';
				}
			}
		}
		int get_width() const
		{
			return field_width;
		}
		int get_height() const
		{
			return field_height;
		}
		void draw(int y, int x, char what)
		{
			field[y][x] = what;
		}
} field;

class Food
{
		position pos;
		char symbol;
	public:
		Food(): symbol('X'), pos()
		{
			pos.x = pos.y = -1;
		}
		void set_pos(int x, int y)
		{
			pos.x = x;
			pos.y = y;
		}
		void relocate(const Field & field)
		{
			pos.x = 1+(rand() % (field.get_width()-2));
			pos.y = 1+(rand() % (field.get_height()-2));
		}
		int get_x() const
		{
			return pos.x;
		}
		int get_y() const
		{
			return pos.y;
		}
		char get_symbol() const
		{
			return symbol;
		}
} food;

class Snake
{
		int dir;
		char symbol, head_symbol;
		position pos[100];
		position & head;
		int speed;
		int size;
		bool can_turn;
	public:
		Snake(int x, int y):
			symbol('*'), head_symbol('O'), pos(),
			speed(1), size(1), dir(RIGHT),
			head(pos[0]), can_turn(true)
		{
			pos[0].x = x;
			pos[0].y = y;
		}
		bool check_food(const Food & food)
		{
			if(food.get_x() == head.x && food.get_y() == head.y)
			{
				size += 1;
				return true;
			}
			return false;
		}
		void get_input()
		{
			if(GetAsyncKeyState(VK_UP) && dir != DOWN)
			{
				dir = UP;
			}
			if(GetAsyncKeyState(VK_DOWN) && dir != UP)
			{
				dir = DOWN;
			}
			if(GetAsyncKeyState(VK_LEFT) && dir != RIGHT)
			{
				dir = LEFT;
			}
			if(GetAsyncKeyState(VK_RIGHT) && dir != LEFT)
			{
				dir = RIGHT;
			}
		}
		void move(const Field & field)
		{
			position next = {0, 0};
			switch(dir)
			{
				case UP:
					next.y = -speed;
					break;
				case DOWN:
					next.y = speed;
					break;
				case LEFT:
					next.x = -speed;
					break;
				case RIGHT:
					next.x = speed;
			}
			for(int c = size - 1; c > 0; --c)
			{
				pos[c] = pos[c-1];
			}
			head.x += next.x;
			head.y += next.y;
			if(head.x <= 0 || head.y <= 0 || head.x >= field.get_width()-1 || head.y >= field.get_height()-1)
			{
				string s="Your score is ";
				s.append(to_string(10*(size-1)));
				throw s;
			}
		}
		void draw(Field & field)
		{
			for(int c = 0; c < size; ++c)
			{
				if(c == 0)
				{
					field.draw(pos[c].y, pos[c].x, head_symbol);
				}
				else
				{
					field.draw(pos[c].y, pos[c].x, symbol);
				}
			}
		}
		int get_x() const
		{
			return head.x;
		}
		int get_y() const
		{
			return head.y;
		}
		char get_symbol() const
		{
			return symbol;
		}
} snake(1, 1);

int Field::field_height = 24;
int Field::field_width = 79;

int main()
{
	field.clear();
	cout<<"Enter Level \n1. Hard\n2. Medium\n3. Easy\n";
	int x;
	cin>>x;
	if(x==1)
	{
		Field:: field_height=12;
		Field:: field_width=25;
	}
	else if(x==2)
	{
		Field:: field_height=19;
		Field:: field_width=50;
	}
	else
	{
		Field:: field_height=24;
		Field:: field_width=79;
	}
	system("cls");
	food.set_pos(4, 4);
	while(1)
	{
		field.clear();
		snake.get_input();
		try
		{
			snake.move(field);
		}
		catch (string s)
		{
			field.clear();
			cerr << s << endl;
			system("pause");
			return -1;
		}
		snake.draw(field);
		field.draw(food.get_y(), food.get_x(), food.get_symbol());
		if(snake.check_food(food))
		{
			food.relocate(field);
		}
		field.display();
		Sleep(100/x);
	}
	return 0;
}