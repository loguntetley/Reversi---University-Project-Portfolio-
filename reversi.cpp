#include "pch.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <cctype>

using namespace std;

enum concol // enumeration type, gives names to the built-in colours
{
	black = 0, dark_blue = 1, dark_green = 2, dark_cyan = 3, dark_red = 4,
	dark_purple = 5, dark_yellow = 6, light_gray = 7, gray = 8, blue = 9,
	green = 10, cyan = 11, red = 12, purple = 13, yellow = 14, white = 15
};
void setColours(concol textcol, concol backcol) // set text/background colour
{
	// Combine the two 4-bit numbers into a single 8-bit number
	// If backcol is abcd in binary, and textcol is efgh, the result is abcdefgh
	unsigned short colours = (backcol << 4) | textcol;
	// set the attributes for the console
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colours);
}

void SetFont()
{
	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(fontInfo);
	fontInfo.nFont = 00;                         // Index of font in system console font table
	fontInfo.dwFontSize.Y = 30;                  // Size of font (also changes x size)
	fontInfo.dwFontSize.X = 40;                 // Width of font. Used to not work, now it works ¯\_(ツ)_/¯
	fontInfo.FontFamily = FF_DONTCARE;
	fontInfo.FontWeight = FW_NORMAL;
	wcscpy_s(fontInfo.FaceName, L"Consolas");    // Name of font (2nd argument)
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfo);
}

struct killGame : exception
{
	const char* what() const throw () override
	{
		return "its your falut im dead";
	}
};

void boardprint(char board[8][8])
{
	for (int i = 0; i < 8; i++)
	{
		cout << endl;
		for (int b = 0; b < 8; b++) {
			if (board[i][b] == 'w')
			{
				setColours(white, dark_red);
			}

			if (board[i][b] == 'b')
			{
				setColours(black, dark_red);
			}
			if (board[i][b] == '.')
			{
				setColours(gray, dark_red);
			}
			cout << board[i][b];
		}
		setColours(white, black);
	}
}

int ColMovmement()
{
	while (true)
	{
		char ans;
		cout << endl;
		cout << "press q to quite, or give me your location already" << endl;
		cin >> ans;
		ans = tolower(ans);
		if (ans > 96 && ans < 105)
		{
			return ans - 97;
		}
		if (ans == 'q')
			throw killGame();
	}
}

int RowMovmement()
{
	while (true)
	{
		char answer;
		cout << endl;
		cin >> answer;
		return answer - 49;
	}
}

char player(bool& currentPlayer)
{
	char chip = 'w';
	if (currentPlayer == false)
	{
		chip = 'b';
	}
	return chip;
}
void LocationPrint(char Alphabeticlocation[8])
{
	for (int i = 0; i < 8; i++)
	{

		setColours(black, dark_red);
		cout << Alphabeticlocation[i];
	}
}


void Flip(char board[8][8], const unsigned short x, const unsigned short y, const char color, bool& currentPlayer)
{
	bool HasFlipped = false;

	// Make directions
	for (short a = -1; a < 2; a++)
	{
		for (short b = -1; b < 2; b++)
		{
			if (b == 0 && a == 0)
			{
				continue;
			}

			bool ColourId = false;
			// Go in specified direction
			for (unsigned short c = 1; c < 8; c++)
			{
				const char current = board[y + c * a][x + c * b];
				if (ColourId == true && current == color)
				{
					// Flip pieces
					for (unsigned short l = 1; l <= c; l++)
					{
						board[y + l * a][x + l * b] = color;
					}

					board[y][x] = color;
					HasFlipped = true;
				}
				else if (color == 'w' && current == 'b')
				{
					ColourId = true;
				}
				else if (color == 'b' && current == 'w')
				{
					ColourId = true;
				}
				else
				{
					break;
				}
			}
		}
	}

	if (HasFlipped == true)
	{
		currentPlayer = !currentPlayer;
	}
}

int main()
{
	SetFont();

	char board[8][8] =
	{
		{'.','.','.','.','.','.','.','.'},
		{'.','.','.','.','.','.','.','.'},
		{'.','.','.','.','.','.','.','.'},
		{'.','.','.','w','b','.','.','.'},
		{'.','.','.','b','w','.','.','.'},
		{'.','.','.','.','.','.','.','.'},
		{'.','.','.','.','.','.','.','.'},
		{'.','.','.','.','.','.','.','.'}
	};

	char Alphabeticlocation[8] = { 'A','B','C','D','E','F','G','H' };

	LocationPrint(Alphabeticlocation);
	boardprint(board);
	bool currentPlayer = true;
	while (true)
	{
		int col;
		try
		{
			col = ColMovmement();
		}
		catch (killGame)
		{
			break;
		}
		int row = RowMovmement();
		char chip = player(currentPlayer);
		if (board[row][col] == 'w' || board[row][col] == 'b')
		{
			cout << "you cant do that";
		}
		if (board[row][col] == '.')
		{
			Flip(board, col, row, chip, currentPlayer);
			system("CLS");
			LocationPrint(Alphabeticlocation);
			boardprint(board);
		}
	}
	system("PAUSE");
}