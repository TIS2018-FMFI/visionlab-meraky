#pragma once

#if !defined( _ANALOGPOINT_H_ )
#define _ANALOGPOINT_H_

using namespace std;

#include <string>


/**
	 * Class AnalogPoint
	 * Drzi informacie o bode, vyuzivanom pri analogovych meracich pristrojoch
	 */

class AnalogPoint
{
	string name; 
	double x;
	double y;


public:
	AnalogPoint(double x, double y, string name);
	void setX(double x);
	void setY(double y);
	double getX();
	double getY();
	void setName(string name);
	string getName();

};

#endif;




