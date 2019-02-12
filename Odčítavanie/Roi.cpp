
#include "Roi.h"



using namespace std;

Roi::Roi()
{
}

Roi::Roi(string roi_name, double& pos_x, double& pos_y, double& width, double& height, int PDM, string units, string typ)
{
	name = roi_name;
	x = pos_x;
	y = pos_y;
	this->width = width;
	this->height = height;
	this->PDM = PDM;
	this->units = units;
	this->typ = typ;
	//values.reserve(100);


}


Roi::Roi(string roi_name, double& pos_x, double& pos_y, double& width, double& height, int PDM, string units, string typ, vector<double> points)
{
	name = roi_name;
	x = pos_x;
	y = pos_y;
	this->width = width;
	this->height = height;
	this->PDM = PDM;
	this->units = units;
	this->typ = typ;
	int pom_poc = 0;
	for (int i = 0; i < 6; i += 2) {
		double pom_x = points.at(i);
		double pom_y = points.at(i + 1);
		addPoint(pom_x, pom_y, "bod"+ to_string(pom_poc));
		pom_poc++;
	}
	//values.reserve(100);

	cout << to_string(this->points.size()) + " body" << endl;

	for (AnalogPoint p : this->points) {
		cout << "X: " + to_string(p.getX()) + " Y:" + to_string(p.getY()) + " name: " + p.getName() << endl;
	}

}



string Roi::getName() {
	return name;
}

void Roi::setName(string new_name) {
	this->name = new_name;
}

double Roi::getX()
{
	return x;
}

double Roi::getY()
{
	return y;
}

double Roi::getWidth()
{
	return width;
}

double Roi::getHeight()
{
	return height;
}

void Roi::setX(double x)
{
	this->x = x;
}

void Roi::setY(double y)
{
	this->y = y;
}

void Roi::setWidth(double width)
{
	this->width = width;
}

void Roi::setHeight(double height)
{
	this->height = height;
}

int Roi::getPDM()
{
	return PDM;
}

void Roi::setPDM(int new_pdm)
{
	this->PDM = new_pdm;
}

string Roi::getUnits()
{
	return units;
}

void Roi::setUnits(string units)
{
	this->units = units;
}

string Roi::getType()
{
	return typ;
}

void Roi::setType(string type)
{
	this->typ = type;
}

vector<AnalogPoint> Roi::getPoints()
{
	return points;
}

void Roi::addPoint(double x, double y, string which)
{
	points.push_back(AnalogPoint(x, y, which));
}

void Roi::removePoint(string ktory)
{	
	//po dohode ako sa urcity Point bude oznacovat
	
}

void Roi::removePoint(AnalogPoint bod)
{
	//po dohode ako sa urcity Point bude oznacovat
}

void Roi::addValue(double& val)
{
	this->values.push_back(val);
	
}

void Roi::writeToFile(ostream &ofile)
{
	ofile << "[ROI]" << endl;
	ofile << "roi_name: " + name << endl;
	ofile << "x: " + to_string(x) << endl;
	ofile << "y: " + to_string(y) << endl;
	ofile << "width: " + to_string(width) << endl;
	ofile << "height: " + to_string(height) << endl;
	ofile << "pocet_desatinnych_miest: " + to_string(PDM) << endl;
	ofile << "jednotky: " + units << endl;
	ofile << "typ: " + typ << endl;
	if (typ == "analog") {
		ofile << "body: ";
		ofile << "[";
		int pom = 0;
		for (AnalogPoint p : points) {
			ofile << to_string(p.getX());
			ofile << ",";
			ofile << to_string(p.getY());
			if (pom < 2) {
				ofile << ",";
			}
			pom++;
		}


		ofile << "]" << endl;
	}

	ofile << endl;
}

double Roi::getMedian()
{
	if (values.size() > 0) {
		if (values.size() % 2 == 0) {
			const auto median_it1 = values.begin() + values.size() / 2 - 1;
			const auto median_it2 = values.begin() + values.size() / 2;

			std::nth_element(values.begin(), median_it1, values.end());
			const auto e1 = *median_it1;

			std::nth_element(values.begin(), median_it2, values.end());
			const auto e2 = *median_it2;

			return (e1 + e2) / 2;

		}
		else {
			const auto median_it = values.begin() + values.size() / 2;
			std::nth_element(values.begin(), median_it, values.end());
			return *median_it;
		}
	}
	else return 0;
}

double Roi::getAverage()
{
	double sum = 0;
	
	for (double val : this->values) {
		
		sum += val;
	}

	if (this->values.size() > 0) return sum / this->values.size();
	else return 0;

}

void Roi::eraseValues()
{
	values.clear();
}






