#pragma once

#include <Windows.h>
#include "Objects.h"
#include <vector>
#include <list>

using namespace std;

bool OnLineCheckX(int startx, int endx, int y, Drawer& player);
bool OnLineCheckY(int x, int starty, int endy, Drawer& player);
void OnlyOnWindow(int MAPSIZE, Drawer& player, RECT& rectView);
bool SelfLineCheck(vector<POINT> movepoints, Drawer& player, int BeforeX, int BeforeY);
void PlayerCorrectOnLine(int startx, int starty, int endx, int endy, Drawer& player);
void RedesignList(list<POINT>& Area, list<POINT>& origin, list<POINT>& result, POINT a, POINT b);
void OnAreaLineCheck(list<POINT>& Area, Drawer& player, vector<int>& OnAreaLines);
void SumAreas(list<POINT>& Area, list<POINT> redesignlist);


void SumAreas(list<POINT>& Area, list<POINT> redesignlist)
{

	list<POINT>::iterator checkiter = Area.begin();
	list<POINT>::iterator nextiter = Area.begin();
	nextiter++;
	list<POINT>::iterator recheckiter = redesignlist.begin();

	Drawer startpoint, endpoint;
	POINT start = { 0, 0 }, end = { 0, 0 };

	vector<POINT> startNend;
	vector<POINT> startNendBeginPoint;


	for (; checkiter != Area.end(); checkiter++, nextiter++)//시작점과 끝점 추출
	{
		recheckiter = redesignlist.begin();
		if (nextiter == Area.end())
			nextiter = Area.begin();

		if ((*checkiter).x == (*nextiter).x)//y선
		{
			for (; recheckiter != redesignlist.end(); recheckiter++)
			{
				startpoint.setX((*recheckiter).x);
				startpoint.setY((*recheckiter).y);
				if (OnLineCheckY((*checkiter).x, (*checkiter).y, (*nextiter).y, startpoint) == true)
				{
					startNend.push_back(*recheckiter);
					startNendBeginPoint.push_back(*checkiter);
				}
			}
		}
		else if ((*checkiter).y == (*nextiter).y)
		{
			for (; recheckiter != redesignlist.end(); recheckiter++)
			{
				startpoint.setX((*recheckiter).x);
				startpoint.setY((*recheckiter).y);
				if (OnLineCheckX((*checkiter).x, (*nextiter).x, (*checkiter).y, startpoint) == true)
				{
					startNend.push_back(*recheckiter);
					startNendBeginPoint.push_back(*checkiter);
				}
			}
		}
	}


	if (startNend[0].x == startNend[1].x)//y선
	{
		int alen = abs(startNend[0].y - startNendBeginPoint[0].y);
		int blen = abs(startNend[1].y - startNendBeginPoint[1].y);

		if (alen < blen)
		{
			start = startNend[0];
			end = startNend[1];
		}
		else
		{
			start = startNend[1];
			end = startNend[0];
		}
	}
	else if (startNend[0].y == startNend[1].y)
	{
		int alen = abs(startNend[0].x - startNendBeginPoint[0].x);
		int blen = abs(startNend[1].x - startNendBeginPoint[1].x);

		if (alen < blen)
		{
			start = startNend[0];
			end = startNend[1];
		}
		else
		{
			start = startNend[1];
			end = startNend[0];
		}
	}
	else
	{
		start = startNend[0];
		end = startNend[1];
	}


	startpoint.setX(start.x);
	startpoint.setY(start.y);
	endpoint.setX(end.x);
	endpoint.setY(end.y);
	list<POINT> temp = Area;

	checkiter = temp.begin();
	nextiter = temp.begin();
	nextiter++;
	recheckiter = redesignlist.begin();

	Area.clear();

	int startpointLine = 0;
	int endpointLine = 0;
	//도형을 읽음

	while ((*recheckiter).x != start.x || (*recheckiter).y != start.y)
	{
		recheckiter++;
	}

	list<POINT>::iterator nextrecheckiter = recheckiter;
	nextrecheckiter++;



	while (checkiter != temp.end())
	{
		if (nextiter == temp.end())
		{
			nextiter = temp.begin();
		}

		if ((*checkiter).y == (*nextiter).y)//x선
		{
			if (OnLineCheckX((*checkiter).x, (*nextiter).x, (*checkiter).y, startpoint) == false)
			{
				Area.push_back(*checkiter);
				nextiter++;
				checkiter++;
			}
			else
			{
				if ((*nextiter).x == startpoint.getX() &&
					(*nextiter).y == startpoint.getY())
				{
					if ((*recheckiter).y == (*nextrecheckiter).y)
					{

					}
					Area.push_back(*checkiter);
					nextiter++;
					checkiter++;
					continue;
				}
				Area.push_back(*checkiter);
				startpointLine = 1;
				break;
			}
		}
		else if ((*checkiter).x == (*nextiter).x)//y선
		{
			if (OnLineCheckY((*checkiter).x, (*checkiter).y, (*nextiter).y, startpoint) == false)
			{
				Area.push_back(*checkiter);
				nextiter++;
				checkiter++;
			}
			else
			{
				if ((*nextiter).x == startpoint.getX() &&
					(*nextiter).y == startpoint.getY())
				{
					Area.push_back(*checkiter);
					nextiter++;
					checkiter++;
					continue;
				}
				startpointLine = 1;
				break;
			}
		}
	}
	list<POINT>::iterator tempiter = recheckiter;

	if (startpointLine == 1)
		recheckiter++;

	//새로 생성된 도형의 값을 순서대로 전부 넣는다

	for (; recheckiter != redesignlist.end(); recheckiter++)
	{
		Area.push_back(*recheckiter);
	}

	recheckiter = redesignlist.begin();
	for (; recheckiter != tempiter; recheckiter++)
	{
		Area.push_back(*recheckiter);
	}
	while (checkiter != temp.end())
	{
		if (nextiter == temp.end())
		{
			nextiter = temp.begin();
		}
		if ((*checkiter).y == (*nextiter).y)//x선
		{
			if (OnLineCheckX((*checkiter).x, (*nextiter).x, (*checkiter).y, endpoint) == false)
			{
				if (endpointLine == 1)
				{
					if (!(endpoint.getX() == (*nextiter).x &&
						endpoint.getY() == (*nextiter).y))
						Area.push_back(*checkiter);
				}
				nextiter++;
				checkiter++;
			}
			else
			{
				endpointLine = 1;
				nextiter++;
				checkiter++;
			}
		}
		else if ((*checkiter).x == (*nextiter).x)//y선
		{
			if (OnLineCheckY((*checkiter).x, (*checkiter).y, (*nextiter).y, endpoint) == false)
			{
				if (endpointLine == 1)
				{
					if (!(endpoint.getX() == (*nextiter).x &&
						endpoint.getY() == (*nextiter).y))
						Area.push_back(*checkiter);
				}
				nextiter++;
				checkiter++;
			}
			else
			{
				endpointLine = 1;
				nextiter++;
				checkiter++;
			}
		}
	}
	for(POINT i : redesignlist)
		cout << i.x << " " << i.y << endl;
	cout << endl;

	for (POINT i : Area)
		cout << i.x << " " << i.y << endl;
	cout << endl;
}

void OnAreaLineCheck(list<POINT> &Area,  Drawer& player, vector<int>& OnAreaLines)
{
	list<POINT>::iterator temp;
	list<POINT>::iterator tempnext;
	temp = Area.begin();
	tempnext = Area.begin();
	tempnext++;
	int index = 0;

	for (; temp != Area.end(); tempnext++, temp++)
	{
		if (tempnext == Area.end())
			tempnext = Area.begin();

		if ((*temp).x == (*tempnext).x)//y선
		{
			if (OnLineCheckY((*temp).x, (*temp).y, (*tempnext).y, player))
			{
				OnAreaLines.push_back(index);
			}
		}
		else if ((*temp).y == (*tempnext).y)//x선
		{
			if (OnLineCheckX((*temp).x, (*tempnext).x, (*temp).y, player))
			{
				OnAreaLines.push_back(index);
			}
		}
		index++;
	}
}

void RedesignList(list<POINT> &Area, list<POINT> &origin, list<POINT> &result, POINT a, POINT b)
{
	list<POINT>::iterator tempiter, nextiter;
	Drawer tempPoint;

	vector<POINT> startNend;
	startNend.push_back(a);
	startNend.push_back(b);

	vector<POINT> startindexPoint;
	vector<int> startNendindex;


	int readingdirection = 0;

	int index;
	for (int i = 0; i < startNend.size(); i++)
	{
		index = 0;
		tempPoint.setX(startNend[i].x);
		tempPoint.setY(startNend[i].y);

		tempiter = Area.begin();
		nextiter = Area.begin();
		nextiter++;

		for (; tempiter != Area.end(); tempiter++, nextiter++)
		{
			if (nextiter == Area.end())
				nextiter = Area.begin();
			if ((*tempiter).x == (*nextiter).x)//y선
			{
				if (OnLineCheckY((*tempiter).x, (*tempiter).y, (*nextiter).y, tempPoint) == true)
				{
					startindexPoint.push_back(*tempiter);
					startNendindex.push_back(index);
					break;
				}
			}
			else if ((*tempiter).y == (*nextiter).y)
			{
				if(OnLineCheckX((*tempiter).x, (*nextiter).x, (*tempiter).y, tempPoint) == true)
				{
					startindexPoint.push_back(*tempiter);
					startNendindex.push_back(index);

					break;
				}
			}
			index++;
		}
	}

	if (startNendindex[0] < startNendindex[1])
		readingdirection = 1;
	else if (startNendindex[0] > startNendindex[1])
		readingdirection = 2;
	else
	{
		int alen, blen;
		if (startNend[0].x == startindexPoint[0].x)//y선
		{
			alen = abs(startNend[0].y - startindexPoint[0].y);
			blen = abs(startNend[1].y - startindexPoint[1].y);
		}
		else
		{
			alen = abs(startNend[0].x - startindexPoint[0].x);
			blen = abs(startNend[1].x - startindexPoint[1].x);
		}

		if (alen < blen)
			readingdirection = 1;
		else
			readingdirection = 2;
	}
	if(readingdirection == 1)//정방향
		result = origin;
	else if (readingdirection == 2)//역방향
	{
		tempiter = origin.end();
		tempiter--;
		for (; tempiter != origin.begin(); tempiter--)
		{
			result.push_back((*tempiter));
		}
		result.push_back((*tempiter));
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