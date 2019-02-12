


#include "Config.h"
#include "Roi.h"
#include "Camera.h"
#include "DataWriter.h"


using namespace std;


		Config::Config(string file_name) {
			//precita config file, vytvori roi, vytvori cameru
			//ulozi potrebne veci

			string line = "";
			
			ifstream input_file(file_name);
			if (input_file.is_open()) {

				//prva cast config file
				//obsahuje filename (nazov alebo path k config file)
				//interval - cas v sekundach, za ktory bude bezat meranie
				//avg_med_interval - cas v sekundach za ktory sa bude pocitat median a priemer
				//output_file - subor do ktoreho sa zapisuje vystup,
				//ak nie je urceny, vytvori sa novy pod nazvom datumu

				getline(input_file, line);
				if (line.find("file_name") != std::string::npos) { 
					size_t pos = line.find(":");
					config_file = trim(line.substr(pos+1));
				}
				else {
					//zly format suboru, chybajuci atribut filename
					throw invalid_argument("Missing attribute file_name");
				}

				getline(input_file, line);
				if (line.find("interval") != std::string::npos) {
					size_t pos = line.find(":");
					setInterval(stoi(line.substr(pos + 1)));
				}
				else {
					//zly format suboru, chybajuci atribut interval
					throw invalid_argument("Missing attribute interval");
				}

				getline(input_file, line);
				if (line.find("avg_med_interval") != std::string::npos) {
					size_t pos = line.find(":");
					setAvgMedInterval(stoi(line.substr(pos + 1)));

					getline(input_file, line);
					if (line.find("avg_or_med") != std::string::npos) {
						size_t pos = line.find(":");
						string pom = trim(line.substr(pos + 1));

						if (pom == "both") {
							setDoAverage(true);
							setDoMedian(true);
						}

						else if (pom == "avg") {
							setDoAverage(true);
							setDoMedian(false);
						}

						else if (pom == "med") {
							setDoAverage(false);
							setDoMedian(true);
						}

						else if (pom == "none") {
							setDoAverage(false);
							setDoMedian(false);
						}

					}
					else {
						//zly format suboru, chybajuci atribut interval
						throw invalid_argument("Missing information about whether to count median or average");
					}

				}
				else {
					//atribut avg_med_interval nie je povinny,
					setAvgMedInterval(0);
					setDoAverage(false);
					setDoMedian(false);
				}

				getline(input_file, line);
				if (line.find("cislice") != std::string::npos) {
					size_t pos = line.find(":");
					string c = trim(line.substr(pos + 1));
					if (c == "dark" || c == "bright") {
						setCislice(c);
					}
					else {
						throw invalid_argument("Wrong attribute value cislice");
					}
				}
				else {
					//zly format suboru, chybajuci atribut interval
					throw invalid_argument("Missing attribute cislice");
				}



				getline(input_file, line);
				if (line.find("output_file") != std::string::npos) {
					size_t pos = line.find(":");
					output_file = trim(line.substr(pos + 1));
				}
				else {
					//nie je zadany, vytvor novy podla aktualneho datumu
					time_t rawtime;
					struct tm timeinfo;
					time(&rawtime);
					localtime_s(&timeinfo, &rawtime);
					output_file = to_string(timeinfo.tm_mday) + '-' + to_string(timeinfo.tm_mon + 1) + '-' + to_string(timeinfo.tm_year+1900) + ".csv";

				}

				//koniec zakladnych atributov

				//citaj file az do konca
				//precitas CAMERA zavolaj funkciu na nastavenie kamery
				//precitas ROI, vytvor ROI podla config file, pridaj do vectora
				
				while (!input_file.eof()) {
					
					getline(input_file, line);
					//cout << line << endl;

					if (line.find("[ROI]") != string::npos) {
						//funkcia na spracovanie ROI
						addRoi(setROIfromInput(input_file));
						
					}

					else if (line.find("[CAMERA]") != string::npos && !camera_set) {
						
						cam = setCameraFromInput(input_file);
						camera_set = true;
						
					}

				}

				input_file.close();

				//cout << config_file << endl;
				//cout << output_file << endl;
				//cout << interval << endl;
				//cout << int_avg_med << endl;


			}
			else {
				//subor sa nepodarilo otvorit
				throw invalid_argument("Config file could not be opened");
			}
			


		};
		
		Config::Config() {
			//pripad, kedy nie je nastaveny config file
			//vytvori sa prazdna trieda a postupne uzivatel doplni ROI, nastavi kameru, atd :) 
			
			time_t rawtime;
			struct tm timeinfo;
			time(&rawtime);
			localtime_s(&timeinfo, &rawtime);
			string file_name = to_string(timeinfo.tm_mday) + '-' + to_string(timeinfo.tm_mon + 1) + '-' + to_string(timeinfo.tm_year + 1900);
			setConfigFile("config " + file_name + ".txt");
			setOutputFile("output " + file_name + ".csv");


	
		}
		Config::~Config()
		{
		}

		bool Config::fileExists(string filepath)
		{
			
			ifstream infile(filepath);
			return infile.good();
			
		}

		string Config::returnCurrentTimestampAsString()
		{
			time_t tt;
			struct tm ti;

			// Applying time() 
			time(&tt);

			// Using localtime() 
			localtime_s(&ti, &tt);

			string output = to_string(ti.tm_hour) + ":" + to_string(ti.tm_min) + ":" + to_string(ti.tm_sec);
			return output;
			
		}

		

		Roi Config::getRoiByName(string name)
		{
			for (Roi r : getRoiList()) {
				if (name == r.getName())
					return r;
			}

			return Roi();
		}

		bool Config::RoiWithNameExists(string name)
		{
			for (Roi r : getRoiList()) {
				if (r.getName() == name) {
					return true;
				}
			}

			return false;
		}

		void Config::setDataWriter()
		{
			//pripravy subor na vypis, toto sa zrejme spusti, az ked 
			//uzivatel prejde do rezimu behu a bude mat vsetky ROIs uz nastavene

			dWriter = DataWriter(output_file, getInterval(), getIntAvgMed(), getRoiList());
			if (!fileExists(output_file)) dWriter.createFile(); //ak subor neexistuje, vytvori ho :) 
		}

		DataWriter Config::getDataWriter()
		{
			return dWriter;
		}




		;
		vector<Roi>&Config::getRoiList() {
			return roi_list;
		};

		void Config::deleteRoi(int index) {
			roi_list.erase(roi_list.begin()+index);
		}

		void Config::deleteRoi(Roi r)
		{
			string nameToFind = r.getName();
			int pos = 0;
			for (Roi roi : getRoiList()) {
				if (roi.getName() == nameToFind) {
					deleteRoi(pos);
				}
				pos++;
			}
			

		}

		void Config::addRoi(Roi& roi) {
			roi_list.push_back(roi);
		}

		Camera Config::getCam() {
			return cam;
		}
		void Config::setCam(Camera &came) {
			cam = came;
			camera_set = true;
		}


		string Config::getOutputFile() {
			return output_file;
		}

		void Config::setOutputFile(string path)
		{
			output_file = path;
		}

		void Config::setConfigFile(string path)
		{
			config_file = path;
		}

		string Config::getConfigFile()
		{
			return config_file;
		}

		int Config::getInterval() { return interval; }
		void Config::setInterval(int i) { interval = i; }
		int Config::getIntAvgMed() { return int_avg_med; }

		void Config::setDoAverage(bool val)
		{
			do_average = val;
		}

		void Config::setDoMedian(bool val)
		{
			do_median = val;
		}

		bool Config::doAverage()
		{
			return do_average;
		}

		bool Config::doMedian()
		{
			return do_median;
		}
		void Config::setRmin(int i) { rmin = i; }
		void Config::setRmax(int i) { rmax = i; }
		void Config::setGmin(int i) { gmin = i; }
		void Config::setGmax(int i) { gmax = i; }
		void Config::setBmin(int i) { bmin = i; }
		void Config::setBmax(int i) { bmax = i; }
		int Config::getRmin() { return rmin; };
		int Config::getRmax() { return rmax; }
		int Config::getGmin() { return gmin; }
		int Config::getGmax() { return gmax; }
		int Config::getBmin() { return bmin; }
		int Config::getBmax() { return bmax; }

		void Config::setAvgMedInterval(int i) { int_avg_med = i; }

		string Config::getCislice() {
			return cislice;
		}
		void Config::setCislice(string s) {
			cislice = s;
		}

		bool Config::writeToFile()
		{
			ofstream ofile(config_file);
			
			if (ofile.is_open()) {
				//cout << "Writing to file: " + config_file << endl;
				ofile << "file_name: " + config_file << endl;
				ofile << "interval: " + to_string(getInterval()) << endl;
				ofile << "avg_med_interval: " + to_string(getIntAvgMed()) << endl;

				if (doAverage() && doMedian()) {
					ofile << "avg_or_med: both" << endl;
				}

				else if (doAverage() && !doMedian()) {
					ofile << "avg_or_med: avg" << endl;
				}

				else if (!doAverage() && doMedian()) {
					ofile << "avg_or_med: med" << endl;
				}

				else {
					ofile << "avg_or_med: none" << endl;
				}


				ofile << "cislice: " + getCislice() << endl;
				ofile << "output_file: " + output_file << endl;
				ofile << endl;
				if (camera_set) {
					cam.writeToFile(ofile);
				}

				for (Roi r : getRoiList()) {
					r.writeToFile(ofile);
				}

				

				ofile.close();
				return true;
			}

			return false;



		}



		Roi Config::setROIfromInput(istream &input_file) {
			//ROI obsahuje atributy v tomto poradi:
			//roi_name,x, y, width, height, pocet_desatinnych_miest (PDM), jednotky, typ
			//ak je typ analog, musi obsahovat aj pole troch bodov
			string line = "a";
			string roi_name = "";
			double x;
			double y;
			double width;
			double height;
			int PDM;
			string units;
			string type;
			string body;
			vector<double> points = {};

			getline(input_file, line);
			if (line.find("roi_name") != std::string::npos) {
				size_t pos = line.find(":");
				roi_name = trim(line.substr(pos + 1));
				
			}
			else {
				//zly format suboru, chybajuci atribut filename
				throw invalid_argument("Missing attribute roi_name");
			}

			getline(input_file, line);
			if (line.find("x") != std::string::npos) {
				size_t pos = line.find(":");
				x = stod(line.substr(pos + 1));
				
			}
			else {
				//zly format suboru, chybajuci atribut filename
				throw invalid_argument("Missing attribute coordinate x");
			}

			getline(input_file, line);
			if (line.find("y") != std::string::npos) {
				size_t pos = line.find(":");
				y = stod(line.substr(pos + 1));
				
			}
			else {
				//zly format suboru, chybajuci atribut filename
				throw invalid_argument("Missing attribute coordinate y");
			}

			getline(input_file, line);
			if (line.find("width") != std::string::npos) {
				size_t pos = line.find(":");
				width = stod(line.substr(pos + 1));
				
			}
			else {
				//zly format suboru, chybajuci atribut filename
				throw invalid_argument("Missing attribute width");
			}

			getline(input_file, line);
			if (line.find("height") != std::string::npos) {
				size_t pos = line.find(":");
				height = stod(line.substr(pos + 1));
				
			}
			else {
				//zly format suboru, chybajuci atribut filename
				throw invalid_argument("Missing attribute height");
			}

			getline(input_file, line);
			if (line.find("pocet_desatinnych_miest") != std::string::npos) {
				size_t pos = line.find(":");
				PDM = stoi(line.substr(pos + 1));
				
				
			}
			else {
				//zly format suboru, chybajuci atribut filename
				throw invalid_argument("Missing attribute pocet desatinnych miest");
			}
			
			getline(input_file, line);
			if (line.find("jednotky") != std::string::npos) {
				size_t pos = line.find(":");
				units = trim(line.substr(pos + 1));
				
			}
			else {
				//zly format suboru, chybajuci atribut filename
				throw invalid_argument("Missing attribute jednotky");
			}

			getline(input_file, line);
			if (line.find("typ") != std::string::npos) {
				size_t pos = line.find(":");
				type = trim(line.substr(pos + 1));
				
			}
			else {
				//zly format suboru, chybajuci atribut filename
				throw invalid_argument("Missing attribute typ");
			}

			if (type == "analog") {
				getline(input_file, line);
				
				if (line.find("body") != std::string::npos) {
					size_t pos = line.find(":");
					string pom = trim(line.substr(pos + 1));
					string body;
				
					body.reserve(pom.size());
					string numb = "";
					for (size_t i = 0; i < pom.size(); i++) {
						if (pom[i] != '[' && pom[i] != ']' && pom[i] != ',') {
							numb += pom[i];
						}
						else if (pom[i] == ',' || pom[i] == ']') {
							points.push_back(stod(trim(numb)));
							numb = "";
						}
					}

					if (points.size() != 6) {
						throw invalid_argument("Missing points coordinates");
					}
				}
				else {
					//zly format suboru, chybajuci atribut filename
					throw invalid_argument("Missing attribute body for analog type");
				}
			}


			if (type == "digital") {
				return Roi(roi_name, x, y, width, height, PDM, units, type);
			}

			else {
				return Roi(roi_name, x, y, width, height, PDM, units, type, points);
			}
		}

		Camera Config::setCameraFromInput(istream &input_file) {

			string line;
			int id;
			int res;
			double jas;
			double kontrast;
			string prahovanie;
			double prahovacia_hodnota;

			getline(input_file, line);
			if (line.find("id:") != std::string::npos) {
				size_t pos = line.find(":");
				id = stoi(trim(line.substr(pos + 1)));
				
			}
			else {
				//zly format suboru, chybajuci atribut filename
				throw invalid_argument("Missing attribute ID");
			}

			getline(input_file, line);
			if (line.find("default_res:") != std::string::npos) {
				size_t pos = line.find(":");
				res = stoi(trim(line.substr(pos + 1)));

			}
			else {
				//zly format suboru, chybajuci atribut filename
				throw invalid_argument("Missing attribute default resolution");
			}

			getline(input_file, line);
			if (line.find("jas:") != std::string::npos) {
				size_t pos = line.find(":");
				jas = stod(trim(line.substr(pos + 1)));

			}
			else {
				//zly format suboru, chybajuci atribut filename
				throw invalid_argument("Missing attribute jas");
			}

			getline(input_file, line);
			if (line.find("kontrast:") != std::string::npos) {
				size_t pos = line.find(":");
				kontrast = stod(trim(line.substr(pos + 1)));

			}
			else {
				//zly format suboru, chybajuci atribut filename
				throw invalid_argument("Missing attribute kontrast");
			}

			getline(input_file, line);
			if (line.find("prahovanie:") != std::string::npos) {
				size_t pos = line.find(":");
				prahovanie = trim(line.substr(pos + 1));

			}
			else {
				//zly format suboru, chybajuci atribut filename
				throw invalid_argument("Missing attribute prahovanie");
			}

			if (prahovanie == "rucne") {
				getline(input_file, line);
				if (line.find("prahovacia_hodnota:") != std::string::npos) {
					size_t pos = line.find(":");
					prahovacia_hodnota = stod(trim(line.substr(pos + 1)));

				}
				else {
					//zly format suboru, chybajuci atribut filename
					throw invalid_argument("Missing attribute prahovacia hodnota");
				}
			}

			if (prahovanie == "auto") {
				return Camera(id, res, jas, kontrast, prahovanie);
			}

			else {
				return Camera(id, res, jas, kontrast, prahovanie, prahovacia_hodnota);
			}
		}

		std::string Config::trim(const std::string &s)
		{
			std::string::const_iterator it = s.begin();
			while (it != s.end() && isspace(*it))
				it++;

			std::string::const_reverse_iterator rit = s.rbegin();
			while (rit.base() != it && isspace(*rit))
				rit++;

			return std::string(it, rit.base());
		}




