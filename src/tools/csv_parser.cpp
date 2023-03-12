#include <iostream>
#include <fstream>
#include <string>
#include <allegro5/allegro.h>
#include "tools/csv_parser.h"


CSVParser::CSVParser(ifstream f) {
	this->bitmap = NULL;
	return;
}

CSVParser::CSVParser() 
{
	this->bitmap = NULL;

	return;
}

int CSVParser::GetHeight() {
	return this->height;
}

int CSVParser::GetWidth() {
	return this->width;
}

Bitmap* CSVParser::CsvFileToBitmap(string file){
	ifstream _file=get_file(file);
	string index = "";
	int cols = 0;
	int rows = 0;
	while (getline(_file, index, ',')){
		if (index.find("\n") != std::string::npos) {
			//std::cout << "new line\n";
			++rows;
			this->width = cols + 1;
			cols = 0;
		}
		//cout << index << endl;
		++cols;
	}

	this->height = rows;
	_file.close();
	return NULL;
}

ifstream CSVParser::get_file(string file)
{
	ifstream   MyFile(file);
	return MyFile;

}







