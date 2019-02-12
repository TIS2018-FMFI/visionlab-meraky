#pragma once

using namespace std;

#include <string>

/**
	 * Class Camera
	 * Drzi informacie o pouzivanej kamere
	 */

class Camera
{

	/**
	 * id -> ID danej kamery
	 * res -> nastavene rozlisenie
	 * jas -> hodnota jasu
	 * kontrast -> hodnota kontrast
	 * prahovanie -> typ prahovania (auto)
	 * prahovacia_hodnota -> nepouzivane
	 */


	int ID;
	int res;
	int jas;
	double kontrast;
	string prahovanie;
	double prahovacia_hodnota;


public:
	Camera(int ID, int res, int jas, double kontrast, string prahovanie);
	Camera(int ID, int res, int jas, double kontrast, string prahovanie, double prahovacia_hodnota);
	Camera();
	~Camera();
	int getJas();
	void setJas(int j);
	void setKontrast(double k);
	double getKontrast();
	int getID();
	void setID(int id);
	int getRes();
	void setRes(int dr);


	/**
	 * zapise informacie o kamere do output suboru. ofile je stream do ktoreho sa zapisuje
	*/
	void writeToFile(ostream &ofile);
};

