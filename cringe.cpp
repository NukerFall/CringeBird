#include<iostream>
#include<conio.h>
#include<dos.h>
#include<stdlib.h>
#include<string.h>
#include <windows.h>
#include <time.h> 
// рандомные импорты
#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70
#define MENU_WIDTH 20
#define GAP_SIZE 7
#define PIPE_DIF 45
// всякие константы
using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // буду менять цвета
COORD CursorPosition; // Буду обрабатывать откуда cout

boolean goesUp = false; // Буду рисовать птичку вверх и птичку вниз

int pipePos[3];
int gapPos[3];
int pipeFlag[3];
char bird[3][6] = { ' ','_','_','_',' ',' ',
					'/',' ','v','o','\\','>',
					'|', '_', '_', '_', '/' ,' ' };
char birdd[3][6] = { ' ','_','_','_',' ',' ',
					'/',' ','^','o','\\','>',
					'|', '_', '_', '_', '/' ,' ' };
int birdPos = 6;
int score = 0;

void gotoxy(int x, int y) {
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size) {
	if (size == 0) size = 20;
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}

void drawBorder() {
	SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	for (int i = 0; i <= SCREEN_WIDTH; i++) {
		gotoxy(i, 0); cout << "#";
		gotoxy(i, SCREEN_HEIGHT); cout << "#";
	}

	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		gotoxy(0, i); cout << "#";
		gotoxy(SCREEN_WIDTH, i); cout << "#";
	}
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		gotoxy(WIN_WIDTH, i); cout << "#";
	}
} // заливка по краям

void generatePipe(int ind) {
	gapPos[ind] = 3 + rand() % 14;
} // трубу рандомим для того чтобы вставить

void drawPipe(int ind) {
	SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	if (pipeFlag[ind] == true) {
		for (int i = 0; i < gapPos[ind]; i++) {
			gotoxy(WIN_WIDTH - pipePos[ind], i + 1); cout << "|*|";
		}
		for (int i = gapPos[ind] + GAP_SIZE; i < SCREEN_HEIGHT - 1; i++) {
			gotoxy(WIN_WIDTH - pipePos[ind], i + 1); cout << "|*|";
		}
	}
} // рисуем сгенерированные трубы из списка

void erasePipe(int ind) {
	if (pipeFlag[ind] == true) {
		for (int i = 0; i < gapPos[ind]; i++) {
			gotoxy(WIN_WIDTH - pipePos[ind], i + 1); cout << "   ";
		}
		for (int i = gapPos[ind] + GAP_SIZE; i < SCREEN_HEIGHT - 1; i++) {
			gotoxy(WIN_WIDTH - pipePos[ind], i + 1); cout << "   ";
		}
	}
} // стираем старую чтобы ткнуть новую по кд

void drawBird() {
	SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 6; j++) {
			if (goesUp) {
				gotoxy(j + 2, i + birdPos); cout << bird[i][j];
			} else {
				gotoxy(j + 2, i + birdPos); cout << birdd[i][j];
			}
		}
	}
} // Самый лучший рисовальщик птиц

void eraseBird() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 6; j++) {
			gotoxy(j + 2, i + birdPos); cout << " ";
		}
	}
} // Какая досадная потеря

int collision() {
	if (pipePos[0] >= 61) {
		if (birdPos < gapPos[0] - 1 || birdPos > gapPos[0] - 2 + GAP_SIZE) {
			return 1;
		}
	}
	return 0;
} // Проверка на столкновение(true/false но интами) 

void death() {
	system("cls");
	cout << endl;
	SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "\t\t--------------------------" << endl;
	cout << "\t\t--- Напился и разбился ---" << endl;
	cout << "\t\t--------------------------" << endl << endl;
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\t\tНажми любую кнопку чтобы вернуться в меню.";
	_getch();
} // Очистить всё и вывести текст с цветом
void updateScore() {
	gotoxy(WIN_WIDTH + 7, 5); cout << "Очков: " << score << endl;
} // Выводить очки

void help() {
	system("cls");
	SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "Как играть:" << endl;
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "Кликай на пробел чтобы птичка подлетала.";
	SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "\n\nНажми любую кнопку чтобы вернуться в меню.";
	_getch();
} // Окошко с помощью
// Навалил кринжа
void play();

int main() {
	setlocale(LC_ALL, "Russian");
	setcursor(0, 0);
	srand((unsigned)time(NULL));
	while(1) {
		system("cls");
		SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		gotoxy(10, 5); cout << " -------------------------- ";
		gotoxy(10, 6); cout << " |     Пивная птичка      | ";
		gotoxy(10, 7); cout << " --------------------------";
		SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		gotoxy(10, 9); cout << "1. Словить тильт";
		gotoxy(10, 10); cout << "2. Как играть?";
		gotoxy(10, 11); cout << "3. Выйти из консольки";
		SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		gotoxy(10, 13); cout << "Введи цифру от 1 до 3: ";
		char op = _getche();
		if (op == '1') play();
		else if (op == '2') help();
		else if (op == '3') exit(0);
	}
	return 0;
}

void play() {
	birdPos = 6;
	score = 0;
	pipeFlag[0] = 1;
	pipeFlag[1] = 0;
	pipePos[0] = pipePos[1] = 4;
	system("cls");
	drawBorder();
	generatePipe(0);
	updateScore();
	SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	gotoxy(WIN_WIDTH + 5, 2); cout << "ПИВНАЯ ПТИЧКА";
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	gotoxy(10, 5); cout << "Нажми любую кнопку чтобы начать.";
	_getch();
	gotoxy(10, 5); cout << "                                ";
	while (1) {
		goesUp = false;
		if (_kbhit()) {
			char ch = _getch();
			if (ch == 32) {
				if (birdPos > 3) {
					birdPos -= 3;
					goesUp = true;
				}
			}
			if (ch == 27) {
				break;
			}
		}
		drawBird();
		drawPipe(0);
		drawPipe(1);
		if (collision() == 1) {
			death();
			return;
		}
		Sleep(100);
		eraseBird();
		erasePipe(0);
		erasePipe(1);
		birdPos += 1;
		if (birdPos > SCREEN_HEIGHT - 3) {
			death();
			return;
		}
		if (pipeFlag[0] == 1) pipePos[0] += 2;
		if (pipeFlag[1] == 1) pipePos[1] += 2;
		if (pipePos[0] >= 40 && pipePos[0] < 42) {
			pipeFlag[1] = 1;
			pipePos[1] = 4;
			generatePipe(1);
		}
		if (pipePos[0] > 68) {
			score++;
			updateScore();
			pipeFlag[1] = 0;
			pipePos[0] = pipePos[1];
			gapPos[0] = gapPos[1];
		}
	}
}
