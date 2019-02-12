
#pragma once

#if !defined( _ROI_H_ )
#define _ROI_H_

using namespace std;

#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <istream>
#include <iostream>

#include "AnalogPoint.h"


/**
	 * Class Roi
	 * Drzi informacie o vytvorenej oblasti zaujmu
	 */



class Roi
{

	/**
	 * name -> uchovava meno oblasti zaujmu
	 * x, y -> suradnice laveho horneho rohu vytvorenej ROI
	 * width, height -> sirka, vyska ROI
	 * PDM -> pocet desatinnych miest hodnoty, ktora sa nachadza v ROI
	 * units -> jednotky
	 * typ -> analog/digital , typ meracieho pristroja
	 * points -> pole AnalogPointov, ktore urcuju tri body pre vypocet analogovych pristrojov (neimplementovane)
	 * value -> uchovava namerane hodnoty v danej roi
	 */


	string name = "";
	double x;
	double y;
	double width;
	double height;
	int PDM;
	string units;
	string typ;
	vector<AnalogPoint> points = {};
	vector<double> values = {};
	



public:
	Roi();
	Roi(string roi_name, double& pos_x, double& pos_y, double& width, double& height, int PDM, string units, string typ);
	Roi(string roi_name, double& pos_x, double& pos_y, double& width, double& height, int PDM, string units, string typ, vector<double> points);
	//~Roi();


	//getters, setters
	string getName();
	void setName(string n);
	double getX();
	double getY();
	double getWidth();
	double getHeight();
	void setX(double x);
	void setY(double y);
	void setWidth(double width);
	void setHeight(double height);
	int getPDM();
	void setPDM(int new_pdm);
	string getUnits();
	void setUnits(string units);
	string getType();
	void setType(string type);

	//functions
	vector<AnalogPoint> getPoints();

	/**
	 * prida AnalogPoint do vectora AnalogPointov/x,y su suradnice, ktory je pomenovanie
	*/
	void addPoint(double x, double y, string ktory);

	/**
	 * odstrani AnalogPoint z vectora, ktory je jeho oznacenie
	*/
	void removePoint(string ktory);

	/**
	 * odstrani AnalogPoint bod z vectora AnalogPointov
	*/
	void removePoint(AnalogPoint bod);

	/**
	 * prida odmeranu hodnotu do vectora values
	*/
	void addValue(double& val);

	/**
	 * zapise informacie o ROI do suboru, ofile je output file do ktoreho sa zapisuje
	*/
	void writeToFile(ostream &ofile);

	/**
	 * vrati vypocitany median z hodnot vo vectore values
	*/
	double getMedian();

	/**
	 * vrati vypocitany priemer z hodnot vo vectore values
	*/
	double getAverage();

	/**
	 * premaze vector values
	*/
	void eraseValues();

	
};

#endif;