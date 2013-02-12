#include "sandbox.h"
#include <sstream>

// data flag is initialized in constructor
Sandbox::Sandbox(){
	hasData = false;
}

// set the program file stream
void Sandbox::setProg(char *file){
	fp.open(file);
	if(!fp.is_open()){
		errAbort(Error_ProgFile);
	}
}
// set the data file stream
void Sandbox::setData(char *file){
	fd.open(file);
	if(!fd.is_open()){
		errAbort(Error_DataFile);
	}
	hasData = true;
}

// print the instruction along with a line number
string Sandbox::printLineInst(Inst *pinst){
	return printLine(pinst) + ": " + printInst(pinst);
}
// print the instruction in a readable format
string Sandbox::printInst(Inst *pinst){
	if(pinst!=NULL)
		return pinst->text();
	else
		return "";
}
// print the line number of an instruction
string Sandbox::printLine(Inst *pinst){
	if(pinst!=NULL){
		stringstream ss;
		ss << pinst->line();
		string ret;
		ss >> ret;
		ret = "at line " + ret;
		return ret;
	}
	else
		return "at UNKNOWN line";
}

// clean up when sandbox terminates
void Sandbox::cleanUp(){
	for(unsigned int i=0; i<ivec.size(); i++){
		delete ivec[i];
	}
	for(unsigned int i=0; i<vvec.size(); i++){
		delete vvec[i];
	}
	delete pvarConst;
	delete pConst;
}

// print a variable onto the screen
void Sandbox::printv(Var *pvar){
	if(pvar->len()>1){
		// array
		cout << *pvar->arrayp() << endl;
	}else{
		cout << *pvar->p() << endl;
	}
}

// read an integer from data
int Sandbox::readData(){
	if(!fd.is_open()) errAbort(Error_DataFile);
	int val;
	fd >> val;
	if(val>MAXINT || val<-MAXINT) errAbort(Error_UnboundedNum);
	if(fd.fail()) errAbort(Error_NoData);
	return val;
}

void Sandbox::exec(){
	// initialize sandbox
	init();
	// parse the program
	parse();
	// run the program
	run();
	// cleanup after sandbox terminates
	cleanUp();
	// program terminates without errors
	cout << "Program exits gracefully" << endl;
}