
#include "AnalogPoint.h"

using namespace std;

AnalogPoint::AnalogPoint(double x, double y, string name)
{
	this->name = name;
	this->x = x;
	this->y = y;
}



void AnalogPoint::setX(double x)
{
	this->x = x;

}

void AnalogPoint::setY(double y)
{
	this->y = y;
}

double AnalogPoint::getX()
{
	return x;
}

double AnalogPoint::getY()
{
	return y;
}

void AnalogPoint::setName(string name)
{
	this->name = name;
}

string AnalogPoint::getName()
{
	return this->name;
}
