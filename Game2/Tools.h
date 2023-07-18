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
void RedesignList(list<POINT>& origin, list<POINT>& result, POINT turnpoint, int readingdirection, POINT& inclturn);
void OnAreaLineCheck(list<list<POINT>>& Areas, Drawer& player, vector<int>& OnAreaLines, int& onarealinesize);
void SumAreas(list<list<POINT>>& Areas, list<list<POINT>>::iterator& hititer1, list<list<POINT>>::iterator& hititer2, int& hititeratorcount, list<POINT> NewArea, list<POINT> redesignlist
	, POINT inclturn);
void OnArea(list<list<POINT>>& Areas, list<list<POINT>>::iterator& hititer1, list<list<POINT>>::iterator& hititer2, int& hititeratorcount
	, int BeforeX, int BeforeY);
void FindTurnPoint(list<POINT>& area, POINT& turnpoint);
bool InclCompare(POINT start, POINT end, POINT check);

bool InclCompare(POINT start, POINT end, POINT check)
{
	if ((start.x - end.x) != 0 && (start.x - check.x != 0))
	{
		double basic = -((double)(start.y - end.y) / (double)(start.x - end.x));
		double incl = -((double)(start.y - check.y) / (double)(start.x - check.x));

		if (incl > basic)
			return true;
		else
			return false;
	}
	return false;
}

void FindTurnPoint(list<POINT>& area, POINT& turnpoint)
{
	int max = (*area.begin()).x;
	int min;
	vector<POINT> maxpoints;
	vector<int> maxindex;
	for (POINT i : area)//x의 최댓값을 구한다
	{
		if (i.x > max)
			max = i.x;
	}
	int temp = 0;
	for (POINT i : area)//x값이 최대와 최소인 점을 저장
	{
		if (i.x == max)
		{
			maxpoints.push_back(i);
			maxindex.push_back(temp);
		}
		temp++;
	}
	min = maxpoints[0].y;
	temp = 0;
	for (int i = 0; i < maxpoints.size(); i++)
	{

		if (maxpoints[i].y < min)
		{
			min = maxpoints[i].y;
			temp = i;
		}
	}
	list<POINT>::iterator areaiter = area.begin();

	for (int i = 0; i < maxindex[temp]; i++)
		areaiter++;

	turnpoint = *areaiter;
}

void SumAreas(list<list<POINT>>& Areas, list<list<POINT>>::iterator& hititer1, list<list<POINT>>::iterator& hititer2,
	int& hititeratorcount, list<POINT> NewArea, list<POINT> redesignlist, POINT inclturn)
{
	if (hititeratorcount == 0)
		return;
	else if (hititeratorcount == 1)//1개의 도형과 부딪쳤을 때 
	{
		POINT turn;
		list<POINT>::iterator tempiter = redesignlist.begin();
		list<POINT>::iterator temphititer = (*hititer1).begin();

		int check = 0;
		for (; tempiter != redesignlist.end(); tempiter++)//중복되는 좌표 제거
		{
			check = 0;
			temphititer = (*hititer1).begin();
			for (; temphititer != (*hititer1).end(); temphititer++)
			{
				if (((*tempiter).x == (*temphititer).x) &&
					((*tempiter).y == (*temphititer).y))
				{
					(*hititer1).erase(temphititer);
					redesignlist.erase(tempiter);
					check = 1;
					break;
				}
			}

			if (check == 1)
				tempiter = redesignlist.begin();
		}
		//turn : 원래 도형의 전환점 inclturn : 새로 생성된 도형의 전환점
		//시작 도형의 원래 리스트 값을 저장
		list<POINT> temp = *hititer1;
		(*hititer1).clear();//리스트 초기화

		tempiter = redesignlist.begin();//생성된 도형의 이터레이터
		temphititer = temp.begin();//시작 도형의 이터레이터
		double incl;
		if ((*tempiter).x > (*temphititer).x)//원래 도형이 좌측 생성된 도형은 우측
		{
			list<POINT>::iterator nextiter = temp.begin();
			nextiter++;

			for (POINT i : temp)
				cout << i.x << " " << i.y << endl;
			cout << endl;

			for (; temphititer != temp.end(); nextiter++, temphititer++)
			{
				if (nextiter == temp.end())
					nextiter = temp.begin();

				if ((*temphititer).x != (*nextiter).x && (*temphititer).y != (*nextiter).y)
				{
					(*hititer1).push_back(*temphititer);
					nextiter++;
					temphititer++;
					break;
				}

				if ((*temphititer).x == (*nextiter).x)//y선
				{
					Drawer tempplayer;
					tempplayer.setX((*tempiter).x);
					tempplayer.setY((*tempiter).y);
					if (OnLineCheckY((*temphititer).x, (*temphititer).y, (*nextiter).y, tempplayer) == true)
					{
						(*hititer1).push_back(*temphititer);
						nextiter++;
						temphititer++;
						break;
					}
				}

				(*hititer1).push_back(*temphititer);
			}

			for (; tempiter != redesignlist.end(); tempiter++)
			{
				(*hititer1).push_back(*tempiter);
			}
			for (; temphititer != temp.end(); temphititer++)
			{
				(*hititer1).push_back(*temphititer);
			}

			for (POINT i : *hititer1)
				cout << i.x << " " << i.y << endl;
		}
	}
}


void OnArea(list<list<POINT>>& Areas, list<list<POINT>>::iterator& hititer1, list<list<POINT>>::iterator& hititer2, int& hititeratorcount,
	int BeforeX, int BeforeY)
{
	if (Areas.size() > 0)
	{
		list<list<POINT>>::iterator tempiter = Areas.begin();
		list<POINT>::iterator tempareaiter;
		list<POINT>::iterator tempnextareaiter;
		int max, min;
		Drawer beforeplayer;
		beforeplayer.setX(BeforeX);
		beforeplayer.setY(BeforeY);
		for (; tempiter != Areas.end(); tempiter++)
		{
			tempareaiter = (*tempiter).begin();
			tempnextareaiter = (*tempiter).begin();
			tempnextareaiter++;
			for (; tempnextareaiter != (*tempiter).end(); tempareaiter++, tempnextareaiter++)
			{
				if ((*tempareaiter).x == (*tempnextareaiter).x)//Y선
				{
					if (OnLineCheckY((*tempareaiter).x, (*tempareaiter).y, (*tempnextareaiter).y, beforeplayer))
					{
						if (hititeratorcount == 0)
						{
							hititer1 = tempiter;
							hititeratorcount = 1;
						}
						else if (hititeratorcount == 1 && hititer1 != tempiter)
						{
							hititer2 = tempiter;
							hititeratorcount = 2;
						}
						break;
					}
				}

				else if ((*tempareaiter).y == (*tempnextareaiter).y)
				{
					if (OnLineCheckX((*tempareaiter).x, (*tempnextareaiter).x, (*tempareaiter).y, beforeplayer))
					{
						if (hititeratorcount == 0)
						{
							hititer1 = tempiter;
							hititeratorcount = 1;
						}
						else if (hititeratorcount == 1 && hititer1 != tempiter)
						{
							hititer2 = tempiter;
							hititeratorcount = 2;
						}
						break;
					}
				}
			}
			
			if (tempnextareaiter == (*tempiter).end())
			{
				tempnextareaiter = (*tempiter).begin();
				if ((*tempareaiter).x == (*tempnextareaiter).x)//Y선
				{
					if (OnLineCheckY((*tempareaiter).x, (*tempareaiter).y, (*tempnextareaiter).y, beforeplayer))
					{
						if (hititeratorcount == 0)
						{
							hititer1 = tempiter;
							hititeratorcount = 1;
						}
						else if (hititeratorcount == 1 && hititer1 != tempiter)
						{
							hititer2 = tempiter;
							hititeratorcount = 2;
						}
					}
				}
				else if ((*tempareaiter).y == (*tempnextareaiter).y)//x선
				{
					if (OnLineCheckX((*tempareaiter).x, (*tempnextareaiter).x, (*tempareaiter).y, beforeplayer))
					{
						if (hititeratorcount == 0)
						{
							hititer1 = tempiter;
							hititeratorcount = 1;
						}
						else if (hititeratorcount == 1 && hititer1 != tempiter)
						{
							hititer2 = tempiter;
							hititeratorcount = 2;
						}
					}
				}
			}
			
		}

	}
}

void OnAreaLineCheck(list<list<POINT>> &Areas,  Drawer& player, vector<vector<int>>& OnAreaLines, int & onarealinesize
)
{
	list<POINT>::iterator temp;
	list<POINT>::iterator tempnext;
	for (list<POINT> i : Areas)//도형의 선 위에 있는지 확인
	{

		vector<int> tempvec;
		temp = i.begin();
		tempnext = i.begin();
		tempnext++;
		int index = 0;

		for (; tempnext != i.end(); tempnext++, temp++)
		{
			if ((*temp).x == (*tempnext).x)//y선
			{
				if (OnLineCheckY((*temp).x, (*temp).y, (*tempnext).y, player))
				{
					tempvec.push_back(index);
					onarealinesize += 1;
				}
			}
			else if ((*temp).y == (*tempnext).y)//x선
			{
				if (OnLineCheckX((*temp).x, (*tempnext).x, (*temp).y, player))
				{
					tempvec.push_back(index);
					onarealinesize += 1;
				}
			}
			index++;
		}

		if (tempnext == i.end())
		{
			if ((*temp).x == i.front().x)//y선
			{
				if (OnLineCheckY((*temp).x, (*temp).y, i.front().y, player))
				{
					tempvec.push_back(index);
					onarealinesize += 1;
				}
			}
			else if ((*temp).y == i.front().y)//x선
			{
				if (OnLineCheckX((*temp).x, i.front().x, (*temp).y, player))
				{
					tempvec.push_back(index);
					onarealinesize += 1;
				}
			}
		}
		OnAreaLines.push_back(tempvec);
	}
}

void RedesignList(list<POINT> &origin, list<POINT> &result, POINT turnpoint, int readingdirection, POINT &inclturn)
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
		origin.push_back(turn);
	}

	if (readingdirection == 1)
	{
		tempiter = startiter;
		for (; tempiter != origin.end(); tempiter++)
			result.push_back(*tempiter);

		tempiter = originiter;

		for (; tempiter != startiter; tempiter++)
			result.push_back(*tempiter);

	}
	else if(readingdirection == 2)
	{
		tempiter = startiter;
		for (; tempiter != origin.begin(); tempiter--)
			result.push_back(*tempiter);
		result.push_back(*tempiter);

		tempiter = origin.end();
		tempiter--;
		for (; tempiter != startiter; tempiter--)
			result.push_back(*tempiter);

	}
	inclturn = turn;
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