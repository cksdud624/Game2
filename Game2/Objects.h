#pragma once

#include <list>
class Line
{
private:
	int startx;
	int starty;
	int endx;
	int endy;
	int direction;//1 : x방향 2 : y방향
public:
	void setStartX(int startx) { this->startx = startx; }
	void setStartY(int starty) { this->starty = starty; }
	void setEndX(int endx) { this->endx = endx; }
	void setEndY(int endy) { this->endy = endy; }
	void setDirection(int direction) { this->direction = direction; }

	int getStartX() { return startx; }
	int getStartY() { return starty; }
	int getEndX() { return endx; }
	int getEndY() { return endy; }
	int getDirection() { return direction; }

};


class Drawer
{
private:
	int x;
	int y;
	int width;
	int height;
	bool drawing;
	int speed;
	int direction;
	int returning; //0 : 해당 없음 1 : 원래 위치로 돌아가는 중
	int life;
public:
	void setX(int x) { this->x = x; }
	void setY(int y) { this->y = y; }
	void setDrawing(bool drawing) { this->drawing = drawing; }
	void setWidth(int width) { this->width = width; }
	void setHeight(int height) { this->height = height; }
	void setSpeed(int speed) { this->speed = speed; }
	void setDirection(int direction) { this->direction = direction; }
	void setReturning(int returning) { this->returning = returning; }
	void setLife(int life) { this->life = life; }

	int getX() { return x; }
	int getY() { return y; }
	bool getDrawing() { return drawing; }
	int getWidth() { return width; }
	int getHeight() { return height; }
	int getSpeed() { return speed; }
	int getDirection() { return direction; }
	int getReturning() { return returning; }
	int getLife() { return life; }
};

class ObjectCircle
{
private:
	double x;
	double y;
	int radius;
	int angle;
	int speed;
public:
	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	void setRadius(int radius) { this->radius = radius; }
	void setAngle(int angle) { this->angle = angle; }
	void setSpeed(int speed) { this->speed = speed; }

	double getX() { return x; }
	double getY() { return y; }
	int getRadius() { return radius; }
	int getAngle() { return angle; }
	int getSpeed() { return speed; }
};