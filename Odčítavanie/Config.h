#pragma once

using namespace std;

#include <vector>
#include <fstream>
#include <istream>
#include <iostream>
#include <string>
#include <ctime>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#if !defined( _CONFIG_H_ )
#define _CONFIG_H_

#include "Roi.h"
#include "Camera.h"
#include "DataWriter.h"


/**
	 * Class Config
	 * Drzi informacie o vytvorenej a pouzivanej konfiguracii
	 */


class Config
{

	/**
	 * roi_list -> vector, ktory obsahuje vsetky vytvorene a pouzivane ROI
	 * cam -> referencia na Kameru
	 * camera_set -> oznacuje, ci je kamera uz nastavena
	 * interval -> interval odcitania hodnot
	 * int_avg_med -> interval na pocitanie priemeru a medianu
	 * do_average, do_median -> oznacuje, ci sa priemer/median maju pocitat
	 * output_file -> cesta k vystupnemu suboru
	 * config_file -> cesta ku konfiguracnemu suboru
	 * dWriter -> referencia na triedu zapisovac
	 * cislice -> ci su cislice svetle na tmavom podklade alebo tmave na svetlom (unused)
	 */


	//uchovava ROIs, 
	vector<Roi> roi_list = {};
	//pamata si referenciu na kameru, ktoru pri nacitani config file vytvori
	Camera cam;
	bool camera_set = false;
	//interval merania, interval na priemer a median
	int interval;
	int int_avg_med;
	bool do_average, do_median;
	string output_file = "";
	string config_file = "";
	DataWriter dWriter;
	string cislice = "";

	int rmin = 0;
	int rmax = 255;
	int gmin = 0;
	int gmax = 255;
	int bmin = 0;
	int bmax = 255;


public:
	Config();
	Config(string file_name);
	vector<Roi>& getRoiList();
	void deleteRoi(int index);
	void deleteRoi(Roi r);
	void addRoi(Roi& roi);
	Camera getCam();
	void setCam(Camera &came);
	void setInterval(int interval);
	int getInterval();
	void setAvgMedInterval(int interval);
	int getIntAvgMed();
	void setDoAverage(bool val);
	void setDoMedian(bool val);
	bool doAverage();
	bool doMedian();
	void setRmin(int i);
	void setRmax(int i);
	void setGmin(int i);
	void setGmax(int i);
	void setBmin(int i);
	void setBmax(int i);
	int getRmin();
	int getRmax();
	int getGmin();
	int getGmax();
	int getBmin();
	int getBmax();
	string getOutputFile();
	void setOutputFile(string path);
	void setConfigFile(string path);
	string getConfigFile();
	string getCislice();
	void setCislice(string s);
	void setDataWriter();
	DataWriter getDataWriter();
	virtual ~Config();


	/**
	 * zapise konfiguraciu do suboru, zavola aj zapis Kamery a vsetkych ROI
	*/
	bool writeToFile();


	/**
	 * zisti, ci subor s cestou filepath existuje
	*/
	bool fileExists(string filepath);

	/**
	 * vrati string obsahujuci aktualny cas HH:MM:SS
	*/
	string returnCurrentTimestampAsString();

	/**
	 * vrati ROI podla jej mena
	*/
	Roi getRoiByName(string name);

	/**
	 * zisti, ci ROI s menom name uz existuje
	*/
	bool RoiWithNameExists(string name);





private:

	/**
	 * vytvori ROI zo vstupneho streamu
	*/
	Roi setROIfromInput(istream &input_file);

	/**
	 * vytvori Cameru zo vstupneho streamu
	*/
	Camera setCameraFromInput(istream &input_file);

	/**
	 * pomocna funkcia na odstranenie medzier na zaciatku a konci stringu
	*/
	std::string trim(const std::string &s);
};

#endif

