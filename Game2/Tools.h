#pragma once

#include <Windows.h>
#include "Objects.h"

using namespace std;

bool OnLineCheckX(int startx, int endx, int y, Drawer& player);
bool OnLineCheckY(int x, int starty, int endy, Drawer& player);
void BorderCheck(Line BorderLine[], Drawer& player, vector<int>& OnLines);
void CorrectOverPosition(Line line, Drawer& player, int beforex, int beforey);
void OnlyOnWindow(int MAPSIZE, Drawer& player, RECT& rectView);
void OnAreaCheck(POINT start, POINT end, Drawer& player, vector<int>& OnLines);
bool SelfLineCheck(vector<POINT> movepoints, Drawer& player, int BeforeX, int BeforeY);

bool SelfLineCheck(vector<POINT> movepoints, Drawer& player, int BeforeX, int BeforeY)
{
	int max, min;
	if (movepoints.size() >= 2)
	{
		for (int i = 0; i < movepoints.size() - 1; i++)
		{
			if (movepoints[i].x == movepoints[i + 1].x)
			{
				int beforesign;
				int aftersign;
				if (movepoints[i].y < movepoints[i + 1].y)
				{
					min = movepoints[i].y;
					max = movepoints[i + 1].y;
				}
				else
				{
					min = movepoints[i + 1].y;
					max = movepoints[i].y;
				}
				if (BeforeX - movepoints[i].x > 0)
					beforesign = 1;
				else if (BeforeX - movepoints[i].x < 0)
					beforesign = -1;
				else
					beforesign = 0;

				if (player.getX() - movepoints[i].x > 0)
					aftersign = 1;
				else if (player.getX() - movepoints[i].x < 0)
					aftersign = -1;
				else
					aftersign = 0;

				cout << BeforeX - movepoints[i].x << " "
					<< player.getX() - movepoints[i].x << " " << i <<  endl;
				if (player.getY() <= max && player.getY() >= min &&
					aftersign != 0 && beforesign != 0 &&
					aftersign != beforesign)
				{
					return true;
				}
			}
			else if (movepoints[i].y == movepoints[i + 1].y)
			{
				if (movepoints[i].x < movepoints[i + 1].x)
				{
					min = movepoints[i].x;
					max = movepoints[i + 1].x;
				}
				else
				{
					min = movepoints[i + 1].x;
					max = movepoints[i].x;
				}
			}
		}
	}

	return false;
};

void OnAreaCheck(POINT start, POINT end, Drawer& player, vector<int>& OnLines)
{
	int linedirection;
	if (start.x == end.x)//x선
		linedirection = 1;
	else//y선
		linedirection = 2;
}


void OnlyOnWindow(int MAPSIZE, Drawer& player, RECT &rectView)
{
	if (player.getX() < rectView.left + MAPSIZE)
		player.setX(rectView.left + MAPSIZE);

	else if (player.getX() > rectView.right - MAPSIZE)
		player.setX(rectView.right - MAPSIZE);

	if (player.getY() < rectView.top + MAPSIZE)
		player.setY(rectView.top + MAPSIZE);
	else if (player.getY() > rectView.bottom - MAPSIZE)
		player.setY(rectView.bottom - MAPSIZE);
}

void CorrectOverPosition(Line line, Drawer& player, int beforex, int beforey)
{
	int min, max;
	if (line.getDirection() == 1)
	{
		if (line.getStartX() < line.getEndX())
		{
			min = line.getStartX();
			max = line.getEndX();
		}
		else
		{
			min = line.getEndX();
			max = line.getStartX();
		}

		if (min > player.getX())
		{
			player.setX(min);
			player.setY(beforey);
		}
		
		else if (max < player.getX())
		{
			player.setX(max);
			player.setY(beforey);
		}

		if (player.getY() != beforey)
			player.setY(beforey);
	}
	else if (line.getDirection() == 2)
	{
		if (line.getStartY() < line.getEndY())
		{
			min = line.getStartY();
			max = line.getEndY();
		}
		else
		{
			min = line.getEndY();
			max = line.getStartY();
		}

		if (min > player.getY())
		{
			player.setY(min);
			player.setX(beforex);
		}

		if (max < player.getY())
		{
			player.setY(max);
			player.setX(beforex);
		}

		if (player.getX() != beforex)
			player.setX(beforex);
	}
}

void BorderCheck(Line BorderLine[], Drawer& player, vector<int>& OnLines)
{
	for (int i = 0; i < 4; i++)
	{
		if (BorderLine[i].getDirection() == 1)//x선이면
		{
			if (OnLineCheckX(BorderLine[i].getStartX(), BorderLine[i].getEndX(), BorderLine[i].getStartY(), player))
			{
				OnLines.push_back(i);
			}
		}
		if (BorderLine[i].getDirection() == 2)//y선이면
		{
			if (OnLineCheckY(BorderLine[i].getStartX(), BorderLine[i].getStartY(), BorderLine[i].getEndY(), player))
			{
				OnLines.push_back(i);
			}
		}
	}
}

bool OnLineCheckX(int startx, int endx, int y, Drawer& player)//x방향 선 위에 있는지 확인
{
	int min;
	int max;
	if (startx < endx)
	{
		min = startx;
		max = endx;
	}
	else
	{
		min = endx;
		max = startx;
	}

	if (player.getX() <= max && player.getX() >= min && player.getY() == y)
		return true;
	return false;
}

bool OnLineCheckY(int x, int starty, int endy, Drawer& player)//y방향 선 위에 있는지 확인
{
	int min;
	int max;
	if (starty < endy)
	{
		min = starty;
		max = endy;
	}
	else
	{
		min = endy;
		max = starty;
	}

	if (player.getY() <= max && player.getY() >= min && player.getX() == x)
		return true;
	return false;
}