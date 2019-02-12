
#include "DataWriter.h"


DataWriter::DataWriter(string output_file, int interval,int intervalAvgMed, vector<Roi> r)
{
	this->output_file = output_file;
	this->interval = interval;
	this->intervalAvgMed = intervalAvgMed;
	this->rois = r;
}

DataWriter::DataWriter()
{
}


DataWriter::~DataWriter()
{
}

void DataWriter::createFile()
{
	time_t rawtime;
	struct tm timeinfo;
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	ofstream out_file;
	out_file.open(output_file);
	string date = to_string(timeinfo.tm_mday) + "-" + to_string(timeinfo.tm_mon + 1) + "-" + to_string(timeinfo.tm_year + 1900);
	out_file << "* Dátum: " + date + " *" << endl;
	out_file << "* Konfigurácia: *" << endl << "Interval merania: " + to_string(interval) + " s;Interval pre priemer a medián: " + to_string(intervalAvgMed) + " s" << endl;
	out_file << endl;

	out_file << "ubehnutý èas:;reálny èas:;typ_hodnoty:;nazov_oblasti:;jednotky:;hodnota:;" << endl;


	out_file.close();

}

string DataWriter::returnCurrentTimestampAsString()
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

void DataWriter::writeData(string roi_name, string timestamp, string elapsed_time, string value, string units)
{
	ofstream out_file;
	out_file.open(output_file, ios_base::app);
	out_file << elapsed_time + ";" + timestamp + ";" + "odèítaná;" + roi_name + ";" + units + ";" + value << endl;
	out_file.close();
}

void DataWriter::writeAverage(string roi_name, string timestamp, string elapsed_time, string value, string units)
{
	ofstream out_file;
	out_file.open(output_file, ios_base::app);
	out_file << elapsed_time + ";" + timestamp + ";" + "priemer;" + roi_name + ";" + units + ";" + value << endl;
	out_file.close();
}

void DataWriter::writeMedian(string roi_name, string timestamp, string elapsed_time, string value, string units)
{
	ofstream out_file;
	out_file.open(output_file, ios_base::app);
	out_file << elapsed_time + ";" + timestamp + ";" + "medián;" + roi_name + ";" + units + ";" + value << endl;
	out_file.close();
}

void DataWriter::writeData(string roi_name, string elapsed_time, string value, string units)
{
	ofstream out_file;
	out_file.open(output_file, ios_base::app);
	out_file << elapsed_time + ";" + returnCurrentTimestampAsString() + ";" + "odèítaná;" + roi_name + ";" + units + ";" + value << endl;
	out_file.close();
}

void DataWriter::writeAverage(string roi_name, string elapsed_time, string value, string units)
{
	ofstream out_file;
	out_file.open(output_file, ios_base::app);
	out_file << elapsed_time + ";" + returnCurrentTimestampAsString() + ";" + "priemer;" + roi_name + ";" + units + ";" + value << endl;
	out_file.close();
}

void DataWriter::writeMedian(string roi_name, string elapsed_time, string value, string units)
{
	ofstream out_file;
	out_file.open(output_file, ios_base::app);
	out_file << elapsed_time + ";" + returnCurrentTimestampAsString() + ";" + "medián;" + roi_name + ";" + units + ";" + value << endl;
	out_file.close();
}
