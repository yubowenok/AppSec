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
	bool argErr = false;
	string opt;
	char *pprog, *pdata;
	pprog = pdata = NULL;
	// deal with args
	if(argc>=2){
		opt = string(argv[1]);
		if(opt=="-d"){
			box.setDebugMode();
			if(argc<3){
				argErr = true;
			}else{
				pprog = argv[2];
				if(argc>=4){
					pdata = argv[3];
				}
			}
		}else{
			pprog = argv[1];
			if(argc>=3){
				pdata = argv[2];
			}
		}
	}else{
		argErr = true;
	}
	if(argErr){
		cout << "Incorrect sandbox arguments. Usage: ./sandbox [-d] argprog argdata" << endl;
	    exit(0);
    }else if(pdata==NULL){
		cout << "Running program \"" << pprog << "\"..." << endl;
	}else{
		cout << "Running program \"" << pprog << "\" with data \"" << pdata << "\"..." << endl;
	}
	// set program and data file
	box.setProg(pprog);
	box.setData(pdata);

	// execute the sandbox
	box.exec();

    return 0;
}
