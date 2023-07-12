#pragma once

#include <Windows.h>
#include "Objects.h"

using namespace std;

void DrawRectangle(HDC& mem1dc, Drawer& player);
void DrawLineOnSpace(HDC& mem1dc, POINT start, POINT end);

void DrawRectangle(HDC& mem1dc, Drawer& player)
{
	Rectangle(mem1dc, player.getX() - player.getWidth() / 2,
		player.getY() - player.getWidth() / 2,
		player.getX() + player.getWidth() / 2,
		player.getY() + player.getWidth() / 2);
}

void DrawLineOnSpace(HDC& mem1dc, POINT start, POINT end)
{
	MoveToEx(mem1dc, start.x, start.y, NULL);
	LineTo(mem1dc, end.x, end.y);
}