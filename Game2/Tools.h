#pragma once

#include <Windows.h>
#include "Objects.h"
#include <vector>
#include <list>

using namespace std;

bool OnLineCheckX(int startx, int endx, int y, Drawer& player);
bool OnLineCheckY(int x, int starty, int endy, Drawer& player);
void BorderCheck(Line BorderLine[], Drawer& player, vector<int>& OnLines);
void CorrectOverPosition(Line line, Drawer& player, int beforex, int beforey);
void OnlyOnWindow(int MAPSIZE, Drawer& player, RECT& rectView);
void OnAreaCheck(POINT start, POINT end, Drawer& player, vector<int>& OnLines);
bool SelfLineCheck(vector<POINT> movepoints, Drawer& player, int BeforeX, int BeforeY);
void PlayerCorrectOnLine(int startx, int starty, int endx, int endy, Drawer& player);
void RedesignList(list<POINT> &origin, list<POINT> &result, POINT turnpoint, int readingdirection);

void RedesignList(list<POINT> &origin, list<POINT> &result, POINT turnpoint, int readingdirection)
{
	list<POINT>::iterator originiter = origin.begin();
	list<POINT>::iterator startiter = origin.begin();
	list<POINT>::iterator tempiter;
	list<POINT>::iterator turniter = origin.begin();
	vector<POINT> minpoints;//x값이 최소인 좌표들
	vector<POINT> maxpoints;//x값이 최대인 좌표들
	vector<int> minindex;
	vector<int> maxindex;
	POINT start, turn;
	int pointy;
	int min = (*originiter).x, max = (*originiter).x;
	for (POINT i : origin)//x의 최솟값, 최댓값을 구한다
	{
		if (i.x < min)
			min = i.x;
		if (i.x > max)
			max = i.x;
	}
	int temp = 0;
	for (POINT i : origin)//x값이 최대와 최소인 점을 저장
	{
		if (i.x == min)
		{
			minpoints.push_back(i);
			minindex.push_back(temp);
		}

		if (i.x == max)
		{
			maxpoints.push_back(i);
			maxindex.push_back(temp);
		}
		temp++;
	}


	max = minpoints[0].y;

	pointy = 0;
	for(int i = 0; i < minpoints.size(); i++)
	{
		if (minpoints[i].y > max)
		{
			max = minpoints[i].y;
			pointy = i;
		}
	}
	for (int i = 0; i < minindex[pointy]; i++)
		startiter++;
	start = *startiter;//가장 왼쪽에서 아래의 점

	min = maxpoints[0].y;

	pointy = 0;
	for (int i = 0; i < maxpoints.size(); i++)
	{
		if (maxpoints[i].y < min)
		{
			min = maxpoints[i].y;
			pointy = i;
		}
	}

	for (int i = 0; i < maxindex[pointy]; i++)
		turniter++;
	turn = *turniter;//가장 오른쪽에서 위쪽의 점

	if (turnpoint.x != -1)//우측 위의 경우
	{
		turn = turnpoint;
	}
	else
	{
		if (readingdirection == 1)
		{
			tempiter = startiter;
			for (; tempiter != origin.end(); tempiter++)
				result.push_back(*tempiter);

			tempiter = originiter;

			for (; tempiter != startiter; tempiter++)
				result.push_back(*tempiter);


			for (POINT i : result)
				cout << i.x << " " << i.y << endl;
		}
		else if(readingdirection == 2)
		{
			tempiter = startiter;
			for (; tempiter != origin.begin(); tempiter--)
				result.push_back(*tempiter);

			tempiter = origin.end();
			tempiter--;
			for (; tempiter != startiter; tempiter--)
				result.push_back(*tempiter);

			for (POINT i : result)
				cout << i.x << " " << i.y << endl;
		}
	}
}

void PlayerCorrectOnLine(int x, int y, int BeforeX, int BeforeY, Drawer& player)
{
	int min, max;

	int beforelength;

	if (x == BeforeX)//y선
	{
		beforelength = abs(BeforeY - y);
		if (x != player.getX())
			player.setX(x);

		if (beforelength < abs(player.getY() - y))
			player.setY(BeforeY);
	}
	else if (y == BeforeY)//x선
	{
		beforelength = abs(BeforeX - x);
		if (y != player.getY())
			player.setY(y);
		if (beforelength < abs(player.getX() - x))
			player.setX(BeforeX);

	}
}

bool SelfLineCheck(vector<POINT> movepoints, Drawer& player, int BeforeX, int BeforeY)
{
	int max, min;
	if (movepoints.size() >= 2)
	{
		for (int i = 0; i < movepoints.size() - 2; i++)
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

				if (BeforeX - movepoints[i].x < 0)//좌측
					beforesign = -1;
				else if (BeforeX - movepoints[i].x > 0)//우측
					beforesign = 1;
				else//같은 값
					beforesign = 0;

				if (player.getX() - movepoints[i].x < 0)//좌측
					aftersign = -1;
				else if (player.getX() - movepoints[i].x > 0)
					aftersign = 1;
				else
					aftersign = 0;
				if (player.getY() >= min && player.getY() <= max)
				{
					if (beforesign == -1 && (aftersign == 1 || aftersign == 0))
						return true;
					else if (beforesign == 1 && (aftersign == -1 || aftersign == 0))
						return true;
					else if (beforesign == 0 && aftersign == 0)
						return true;
				}
			}
			else if (movepoints[i].y == movepoints[i + 1].y)
			{
				int beforesign;
				int aftersign;
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

				if (BeforeY - movepoints[i].y < 0)
					beforesign = -1;
				else if (BeforeY - movepoints[i].y > 0)
					beforesign = 1;
				else//같은 값
					beforesign = 0;

				if (player.getY() - movepoints[i].y < 0)//좌측
					aftersign = -1;
				else if (player.getY() - movepoints[i].y > 0)
					aftersign = 1;
				else
					aftersign = 0;

				if (player.getX() >= min && player.getX() <= max)
				{
					if (beforesign == -1 && (aftersign == 1 || aftersign == 0))
						return true;
					else if (beforesign == 1 && (aftersign == -1 || aftersign == 0))
						return true;
					else if (beforesign == 0 && aftersign == 0)
						return true;
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