#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>	
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>


#define ROWS 25
#define COLS 75
#define FLT_MAX 100
int board[ROWS][COLS];


struct node
{
	int x;
	int y;
	char Dir;
	struct node* next;
};
typedef struct node NODE;
NODE* snakehead;
NODE* snaketail;
NODE* fruit;
char dir = 'd';
int prevX, prevY, count = 0, count1 = 0;
float moveSpeed = 0.015;
char* dirs;

// Turn escape codes ON.
void init(void);

// Moves the cursor to position (x,y) on screen.
void gotoxy(int x, int y);

// Delays the execution of the program.
void sleep(float secs);

void playerinput(char ch);
void printBoard();
void drawSnake(NODE* head, char ch);
void freeSnake(NODE* tail);
NODE* newNode(int x, int y);
NODE* addTohead(NODE* head, int x, int y);
NODE* deleteTail(NODE* tail);
bool move(int steps);
bool Collision(NODE* tail, NODE* head);
char think(int x, int y);
// A Function to find the shortest path between 
// a given source cell to a destination cell according 
// to A* Search Algorithm 


	// When the destination cell is not found and the open 
	// list is empty, then we conclude that we failed to 
	// reach the destiantion cell. This may happen when the 
	// there is no way to destination cell (due to blockages) 
//the function will create a snake that grows a tail and speeds up every five steps
void main() {
	bool gameOver = false;
	int steps = 0;
	init();
	while (!gameOver)
	{
		steps++;
		if (_kbhit()) playerinput(_getch());
		gameOver = move(steps);
		sleep(moveSpeed);
	}
	freeSnake(snaketail);
	gotoxy(0, 40);
	printf("Goddammittt!!");
}
// Turn escape codes ON.
void init(void) {
	system("cls");
	printBoard();
	srand(time(0));
	fruit = newNode((rand() % (COLS - 2)) + 2, (rand() % (ROWS - 2)) + 2);
	snaketail = newNode(COLS / 2, ROWS / 2);
	snakehead = newNode(snaketail->x, snaketail->y);
	snakehead->next = NULL;
	snaketail->next = snakehead;
}
bool move(int steps) {
	prevX = snakehead->x, prevY = snakehead->y;
	dir = think(prevX, prevY);
	switch (dir) {
	case 'w':prevY--; break;
	case 'a':prevX--; break;
	case 's':prevY++; break;
	case 'd':prevX++; break;
	}
	//dir = think(prevX, prevY);
	snakehead = addTohead(snakehead, prevX, prevY);
	if (Collision(snaketail, snakehead))
		return true;
	snakehead->Dir = dir;
	switch (dir) {
	case 'w':drawSnake(snakehead, '^'); break;
	case 'a':drawSnake(snakehead, '<'); break;
	case 's':drawSnake(snakehead, 'v'); break;
	case 'd':drawSnake(snakehead, '>'); break;
	}
	drawSnake(snaketail, ' ');
	//if (steps % 5 != 0)
	if (snakehead->x != fruit->x || snakehead->y != fruit->y)
	{
		drawSnake(fruit, '*');
		snaketail = deleteTail(snaketail);
		gotoxy(200, 200);
	}
	else {
		NODE* temp = snaketail;
		fruit->x = (rand() % (COLS - 2)) + 2;
		fruit->y = (rand() % (ROWS - 2)) + 2;
		while (temp->next != NULL)
		{
			while (fruit->x == temp->x && fruit->y == temp->y)
			{
				fruit->x = (rand() % (COLS - 2)) + 2;
				fruit->y = (rand() % (ROWS - 2)) + 2;
			}
			temp = temp->next;
		}
		if (moveSpeed > 1) moveSpeed = 1;
	}
	drawSnake(fruit, '*');
	return false;
}
/// <summary>
/// /Decides the next move of the snake according to its last position and snakes body location
/// </summary>
char think(int x, int y)
{
	char Dir = dir;
	NODE* temp = snaketail;
	int nextX = x, nextY = y;
	bool boom = false;
	if (x != fruit->x)
	{
		if (x < fruit->x)
		{
			if (dir != 'a')
				Dir = 'd';
			else
			{
				if (fruit->y < y)
					Dir = 'w';
				else
					if (fruit->y > y)
						Dir = 's';
					else
					{
						if (ROWS - y > y)
							Dir = 's';
						else
							Dir = 'w';
					}
			}
		}
		else
		{
			if (dir != 'd')
				Dir = 'a';
			else
			{
				if (fruit->y < y)
					Dir = 'w';
				else
					if (fruit->y > y)
						Dir = 's';
					else
					{
						if (ROWS - y > y)
							Dir = 's';
						else
							Dir = 'w';
					}
			}
		}

	}
	else
	{
		if (y < fruit->y)
		{
			if (dir != 'w')
				Dir = 's';
			else
			{
				if (fruit->x < x)
					Dir = 'a';
				else
					if (fruit->x > x)
						Dir = 'd';
					else
					{
						if (COLS - x > x)
							Dir = 'd';
						else
							Dir = 'a';
					}
			}
		}
		else
		{
			if (dir != 's')
				Dir = 'w';
			else
			{
				if (fruit->x < x)
					Dir = 'a';
				else
					if (fruit->x > x)
						Dir = 'd';
					else
					{
						if (COLS - x > x)
							Dir = 'd';
						else
							Dir = 'a';
					}
			}
		}
	}
	switch (Dir) {
	case 'w':nextY--; break;
	case 'a':nextX--; break;
	case 's':nextY++; break;
	case 'd':nextX++; break;
	}
	gotoxy(COLS, count);
	if (dir != Dir)
	{
		gotoxy(0, ROWS + count1);
		count1++;
	}
	while (temp->next->next != NULL || (temp->x == nextX && temp->y == nextY))
	{
		if (boom)
			temp = snaketail;
		if (temp->x == nextX && temp->y == nextY)
		{
			count++;
			gotoxy(COLS, count);
			switch (Dir) {
			case 'w':
				if (temp->x <= temp->next->x && nextX >= 2)
					Dir = 'a';
				else
					if (nextX <= COLS - 2)
						Dir = 'd';
				break;
			case 'a':
				if (temp->y >= temp->next->y && nextY <= ROWS - 2)
					Dir = 's';
				else
					if (nextY >= 2)
						Dir = 'w';
				break;
			case 's':
				if (temp->x >= temp->next->x && nextX <= COLS - 2)
					Dir = 'd';
				else
					if (nextX >= 2)
						Dir = 'a';
				break;
			case 'd':
				if (temp->y <= temp->next->y && nextY >= 2)
					Dir = 'w';
				else
					if (nextY <= ROWS - 2)
						Dir = 's';
				break;
			}
			nextX = x; nextY = y;
			switch (Dir) {
			case 'w':nextY--; break;
			case 'a':nextX--; break;
			case 's':nextY++; break;
			case 'd':nextX++; break;
			}
			boom = true;
		}
		else
			boom = false;
		temp = temp->next;
	}
	return Dir;
}
bool Collision(NODE* tail, NODE* head) {
	NODE* temp = tail;
	if (head->x >= COLS || head->y >= ROWS || head->x <= 1 || head->y <= 1)
		return true;
	while (temp->next->next != NULL)
	{
		if (temp->x == head->x && temp->y == head->y)
			return true;
		temp = temp->next;
	}
	return false;
}
void playerinput(char ch)
{
	switch (ch) {
	case 'w':dir = 'w';	break;
	case 'a':dir = 'a';	break;
	case 's':dir = 's';	break;
	case 'd':dir = 'd';	break;
	}
}
NODE* addTohead(NODE* head, int x, int y)
{
	head->next = newNode(x, y);
	head->x = head->next->x;
	head->y = head->next->y;
	head = head->next;
	return head;
}
NODE* newNode(int x, int y)
{
	NODE* newN = malloc(sizeof(NODE));
	newN->x = x; newN->y = y;
	newN->next = NULL;
	return newN;
}
NODE* deleteTail(NODE* tail)
{
	if (tail != NULL)
	{
		tail->x = tail->next->x;
		tail->y = tail->next->y;
		tail->Dir = tail->next->Dir;
		NODE* temp = tail->next;
		tail->next = tail->next->next;

		free(temp);
	}
	return tail;
}
void drawSnake(NODE* head, char ch)
{
	gotoxy(head->x, head->y);
	printf("%c", ch);
	gotoxy(200, 200);
}
void printBoard()
{
	int i, j;
	for (i = 0; i < ROWS; i++)
	{
		for (j = 0; j < COLS; j++)
		{
			if (i == 0 || j == 0 || i == ROWS - 1 || j == COLS - 1)
				printf("%c", '#');
			else
				printf("%c", ' ');
		}
		printf("\n");
	}
}
void freeSnake(NODE* tail)
{
	NODE* temp;
	while (tail != NULL)
	{
		temp = tail;
		tail = tail->next;
		free(temp);
	}
}
// Moves the cursor to position (x,y) on screen.
void gotoxy(int x, int y) {
	printf("\x1b[%d;%df", y, x);
}
// Delays the execution of the program.
// Parameters:
//     secs: the length of the delay in seconds. 
void sleep(float secs) {
	clock_t clocks_start = clock();
	while (clock() - clocks_start < secs * CLOCKS_PER_SEC);
}