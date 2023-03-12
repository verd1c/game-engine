#include <iostream>
#include <fstream>
#include <stdio.h>
#include "game/App.h"

SuperMario* gApp;

int main(){
	std::cout << "sss\n";
	SuperMario smApp;
	gApp = &smApp;
	smApp.Main();

	int a;
	std::cin >> a;
	return 0;
}