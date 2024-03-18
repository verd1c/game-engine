#ifndef __CSV_PARSER__
#define __CSV_PARSER__

#include <iostream>
#include <fstream>
#include <string>
#include <allegro5/allegro.h>
typedef ALLEGRO_BITMAP Bitmap;

class CSVParser {
private:
	int width;
	int height;

	Bitmap* bitmap;
public:
	CSVParser(std::ifstream f);
	CSVParser();
	Bitmap* CsvFileToBitmap(std::string file);
	std::ifstream get_file(std::string file);

	int GetWidth();
	int GetHeight();
};

#endif