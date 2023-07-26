#pragma once

#include <list>
#include <Windows.h>
#include "Objects.h"
#include <iostream>
#include <vector>

using namespace std;

bool OnArea(list<POINT>& Area, ObjectCircle& circle)
{
	list<POINT>::iterator areaiter = Area.begin();
	list<POINT>::iterator nextiter = Area.begin();
	nextiter++;

	int max, min;
	int count = 0;
	for (; areaiter != Area.end(); areaiter++, nextiter++)
	{
		if (nextiter == Area.end())
			nextiter = Area.begin();

		if ((*areaiter).x == (*nextiter).x)//y선일때만
		{
			if ((*areaiter).y < (*nextiter).y)
			{
				min = (*areaiter).y;
				max = (*nextiter).y;
			}
			else
			{
				max = (*areaiter).y;
				min = (*nextiter).y;
			}

			if (circle.getX() < (*areaiter).x && circle.getY() <= max && circle.getY() > min)
			{
				count++;
			}
		}
	}

	if (count % 2 == 0)
		return false;
	else
		return true;
}

bool OutOfArea(list<POINT>& Area, ObjectCircle& circle)
{
	circle.setX(circle.getX() + (double)circle.getSpeed() * cos(circle.getAngle() * M_PI / 180));
	circle.setY(circle.getY() - (double)circle.getSpeed() * sin(circle.getAngle() * M_PI / 180));
	list<POINT>::iterator areaiter = Area.begin();
	list<POINT>::iterator nextiter = Area.begin();
	nextiter++;
	int max, min;
	int collide = 0;
	for (; areaiter != Area.end(); areaiter++, nextiter++)
	{
		if (nextiter == Area.end())
			nextiter = Area.begin();

		if ((*areaiter).x == (*nextiter).x)//y선
		{
			if ((*areaiter).y < (*nextiter).y)
			{
				min = (*areaiter).y;
				max = (*nextiter).y;
			}
			else
			{
				max = (*areaiter).y;
				min = (*nextiter).y;
			}
			if (abs((*areaiter).x - circle.getX()) < circle.getRadius()
				&& circle.getY() < max && circle.getY() > min)
			{
				collide = 2;
				return true;
			}
		}
		else if ((*areaiter).y == (*nextiter).y)//x선
		{
			if ((*areaiter).x < (*nextiter).x)
			{
				min = (*areaiter).x;
				max = (*nextiter).x;
			}
			else
			{
				max = (*areaiter).x;
				min = (*nextiter).x;
			}
			if (abs((*areaiter).y - circle.getY()) < circle.getRadius()
				&& circle.getX() < max && circle.getX() > min)
			{
				return true;
			}
		}
		if ((pow((*areaiter).x - circle.getX(), 2) + pow((*areaiter).y - circle.getY(), 2))
			< pow(circle.getRadius(), 2))
		{
			return true;
		}
	}

	return false;
}

bool HitPlayer(vector<POINT>& movements, ObjectCircle& circle, Drawer& player)
{
	if (pow(player.getX() - circle.getX(), 2) + pow(player.getY() - circle.getY(), 2) < pow(circle.getRadius(), 2))
		return true;
	int nexti = 1;
	int min, max;
	for (int i = 0; i < movements.size(); i++, nexti++)
	{
		POINT pointi = movements[i];
		POINT pointnexti;

		if (nexti >= movements.size())
		{
			pointnexti.x = player.getX();
			pointnexti.y = player.getY();
		}
		else
		{
			pointnexti = movements[nexti];
		}

		if (pointi.x == pointnexti.x)//y선
		{
			if (pointi.y < pointnexti.y)
			{
				min = pointi.y;
				max = pointnexti.y;
			}
			else
			{
				min = pointnexti.y;
				max = pointi.y;
			}

			if (abs(pointi.x - circle.getX()) < circle.getRadius()
				&& circle.getY() > min && circle.getY() < max)
				return true;
		}
		else if (pointi.y == pointnexti.y)//x선
		{
			if (pointi.x < pointnexti.x)
			{
				min = pointi.x;
				max = pointnexti.x;
			}
			else
			{
				min = pointnexti.x;
				max = pointi.x;
			}

			if (abs(pointi.y - circle.getY()) < circle.getRadius()
				&& circle.getX() > min && circle.getX() < max)
				return true;
		}


		if ((pow(pointi.x - circle.getX(), 2) + pow(pointi.y - circle.getY(), 2))
			< pow(circle.getRadius(), 2))
			return true;
	}
	return false;
}

void CollideCheck(list<POINT>& Area, ObjectCircle& circle)
{
	circle.setX(circle.getX() + (double)circle.getSpeed() * cos(circle.getAngle() * M_PI / 180));
	circle.setY(circle.getY() - (double)circle.getSpeed() * sin(circle.getAngle() * M_PI / 180));
	list<POINT>::iterator areaiter = Area.begin();
	list<POINT>::iterator nextiter = Area.begin();
	nextiter++;
	int max, min;
	int collide = 0;
	for (; areaiter != Area.end(); areaiter++, nextiter++)
	{
		if (nextiter == Area.end())
			nextiter = Area.begin();

		if ((*areaiter).x == (*nextiter).x)//y선
		{
			if ((*areaiter).y < (*nextiter).y)
			{
				min = (*areaiter).y;
				max = (*nextiter).y;
			}
			else
			{
				max = (*areaiter).y;
				min = (*nextiter).y;
			}
			if (abs((*areaiter).x - circle.getX()) < circle.getRadius()
				&& circle.getY() < max && circle.getY() > min)
			{
				collide = 2;
				break;
			}
		}
		else if ((*areaiter).y == (*nextiter).y)//x선
		{
			if ((*areaiter).x < (*nextiter).x)
			{
				min = (*areaiter).x;
				max = (*nextiter).x;
			}
			else
			{
				max = (*areaiter).x;
				min = (*nextiter).x;
			}
			if (abs((*areaiter).y - circle.getY()) < circle.getRadius()
				&& circle.getX() < max && circle.getX() > min)
			{
				collide = 1;
				break;
			}
		}
		if ((pow((*areaiter).x - circle.getX(), 2) + pow((*areaiter).y - circle.getY(), 2))
			< pow(circle.getRadius(), 2))
		{
			collide = 3;
			break;
		}
	}

	int speed = circle.getSpeed();
	if (collide == 1)
	{
		circle.setX(circle.getX() - circle.getSpeed() * cos(circle.getAngle() * M_PI / 180));
		circle.setY(circle.getY() + circle.getSpeed() * sin(circle.getAngle() * M_PI / 180));
		while (!(abs((*areaiter).y - circle.getY()) < circle.getRadius()
			&& circle.getX() < max && circle.getX() > min))
		{
			circle.setX(circle.getX() + cos(circle.getAngle() * M_PI / 180));
			circle.setY(circle.getY() - sin(circle.getAngle() * M_PI / 180));
			speed--;
		}
		circle.setAngle(360 - circle.getAngle());

		circle.setX(circle.getX() + (double)(speed + 1) * cos(circle.getAngle() * M_PI / 180));
		circle.setY(circle.getY() - (double)(speed + 1) * sin(circle.getAngle() * M_PI / 180));
	}

	else if (collide == 2)
	{
		circle.setX(circle.getX() - circle.getSpeed() * cos(circle.getAngle() * M_PI / 180));
		circle.setY(circle.getY() + circle.getSpeed() * sin(circle.getAngle() * M_PI / 180));

		while (!(abs((*areaiter).x - circle.getX()) <= circle.getRadius()
			&& circle.getY() < max && circle.getY() > min))
		{
			circle.setX(circle.getX() + cos(circle.getAngle() * M_PI / 180));
			circle.setY(circle.getY() - sin(circle.getAngle() * M_PI / 180));
			speed--;
		}
		circle.setAngle(180 - circle.getAngle());

		circle.setX(circle.getX() + (double)(speed + 1) * cos(circle.getAngle() * M_PI / 180));
		circle.setY(circle.getY() - (double)(speed + 1) * sin(circle.getAngle() * M_PI / 180));
	}

	else if (collide == 3)
	{
		circle.setX(circle.getX() - circle.getSpeed() * cos(circle.getAngle() * M_PI / 180));
		circle.setY(circle.getY() + circle.getSpeed() * sin(circle.getAngle() * M_PI / 180));

		while (!(pow((*areaiter).x - circle.getX(), 2) + pow((*areaiter).y - circle.getY(), 2)
			< pow(circle.getRadius(), 2)))
		{
			circle.setX(circle.getX() + cos(circle.getAngle() * M_PI / 180));
			circle.setY(circle.getY() - sin(circle.getAngle() * M_PI / 180));
			speed--;
		}

		if (abs((*areaiter).y - circle.getY()) >= abs((*areaiter).x - circle.getX()))
		{
			circle.setAngle(360 - circle.getAngle());
		}
		else
		{
			circle.setAngle(180 - circle.getAngle());
		}
		circle.setX(circle.getX() + (double)(speed + 1) * cos(circle.getAngle() * M_PI / 180));
		circle.setY(circle.getY() - (double)(speed + 1) * sin(circle.getAngle() * M_PI / 180));
	}
}