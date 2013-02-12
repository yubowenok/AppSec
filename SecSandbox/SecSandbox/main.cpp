#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <cstdlib>

#include "sandbox.h"

using namespace std;

int main(int argc, char *argv[]){

	Sandbox box;
	// check the arguments
	if(argc!=2 && argc!=3){
		cout << "Insufficient args. Usage: ./sandbox argprog argdata" << endl;
	    exit(0);
    }else if(argc==2){
		cout << "Running program \"" << argv[1] << "\"..." << endl;
	}else if(argc==3){
		cout << "Running program \"" << argv[1] << "\" with data \"" << argv[2] << "\"..." << endl;
		box.setData(argv[2]);
	}
	box.setProg(argv[1]);

	// execute the sandbox
	box.exec();

    return 0;
}
