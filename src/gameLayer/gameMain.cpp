#include <raylib.h>
#include "gameMain.h"

bool initGame()
{
	return true;
}

bool updateGame()
{
	DrawRectangle(75, 75, 100, 100, { 0,255,0,127 }); // green
	DrawRectangle(50, 50, 100, 100, { 255,0,0,127 }); // red

	DrawText("Congrats! You created your first window", 190, 200, 20, RED);
	//DrawText("Congrats! You created your first window", 190, 200, 20, {255, 0, 200, 255});

	return true;
}

void closeGame()
{
}
