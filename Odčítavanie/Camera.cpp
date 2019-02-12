

#include "Camera.h"


Camera::Camera(int ID, int res, int jas, double kontrast, string prahovanie)
{
	this->ID = ID;
	this->res = res;
	this->jas = jas;
	this->kontrast = kontrast;
	this->prahovanie = prahovanie;


}

Camera::Camera(int ID, int res, int jas, double kontrast, string prahovanie, double prahovacia_hodnota)
{
	this->ID = ID;
	this->res = res;
	this->jas = jas;
	this->kontrast = kontrast;
	this->prahovanie = prahovanie;
	this->prahovacia_hodnota = prahovacia_hodnota;
}

Camera::Camera()
{
	this->ID = 0;
	this->res = 0;
	this->jas = 0;
	this->kontrast = 0;
	this->prahovanie = "auto";
	
}


Camera::~Camera()
{
}

int Camera::getJas() {
	return jas;
}
void Camera::setJas(int j)
{
	this->jas = j;
}
void Camera::setKontrast(double k)
{
	this->kontrast = k;
}
double Camera::getKontrast() {
	return kontrast;
}

int Camera::getID()
{
	return this->ID;
}

void Camera::setID(int id)
{
	this->ID = id;
}

int Camera::getRes()
{
	return this->res;
}

void Camera::setRes(int dr)
{
	this->res = dr;
}

void Camera::writeToFile(ostream &ofile)
{
	ofile << "[CAMERA]" << endl;
	ofile << "id: " + to_string(this->ID) << endl;
	ofile << "default_res:" + to_string(this->res) << endl;
	ofile << "jas: " + to_string(jas) << endl;
	ofile << "kontrast: " + to_string(kontrast) << endl;
	ofile << "prahovanie: " + prahovanie << endl;
	if (prahovanie == "rucne") {
		ofile << "prahovacia_hodnota: " + to_string(prahovacia_hodnota) << endl;
	}

	ofile << endl;

	
}
