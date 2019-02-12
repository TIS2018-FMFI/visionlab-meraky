#pragma once

#if !defined( _DATAWRITER_H_ )
#define _DATAWRITER_H_

using namespace std;

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

#include "Roi.h"

/**
	 * Class DataWriter
	 * Trieda, ktora zabezpecuje zapisovanie dat do vystupneho suboru
	 */


class DataWriter
{
	/**
	 * output_file -> cesta ku vystupnemu suboru
	 * interval -> interval odcitania/zapisovania
	 * intervalAvgMed -> interval na pocitanie medianu/priemeru
	 * rois -> vector vsetkych existujucich ROI
	 */

	string output_file;
	int interval;
	int intervalAvgMed;
	vector<Roi> rois;
	

public:
	DataWriter(string output_file, int interval, int intervalAvgMed, vector<Roi> r);
	DataWriter();
	~DataWriter();



	/**
	 * vytvori subor na zapisovanie ak este neexistuje
	 */
	void createFile();

	/**
	* vrati string obsahujuci aktualny cas v tvare HH:mm:ss
	 */
	string returnCurrentTimestampAsString();

	/**
	* zapise odcitanu hodnotu (value) z ROI roi_name a dalsie informacie
	 */
	void writeData(string roi_name, string timestamp, string elapsed_time, string value, string units);

	/**
	* zapise vypocitany priemer
	 */
	void writeAverage(string roi_name, string timestamp, string elapsed_time, string value, string units);

	/**
	* zapise vypocitany median
	 */
	void writeMedian(string roi_name, string timestamp, string elapsed_time, string value, string units);

	/**
	* zapise odcitanu hodnotu (value) z ROI roi_name a dalsie informacie
	 */
	void writeData(string roi_name, string elapsed_time, string value, string units);

	/**
	* zapise vypocitany priemer
	 */
	void writeAverage(string roi_name,string elapsed_time, string value, string units);

	/**
	* zapise vypocitany median
	 */
	void writeMedian(string roi_name, string elapsed_time, string value, string units);

};

#endif;

