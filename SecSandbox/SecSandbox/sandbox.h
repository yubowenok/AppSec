#ifndef SANDBOX_H
#define SANDBOX_H

#include <iostream>
#include <vector>
#include <map>
#include <fstream>

#include "inst.h"
#include "error.h"

using namespace std;

#define MEMSIZE 65536
#define BUFSIZE 257
#define MAXINT 1000000000
#define MAXLINE 100000

class Sandbox{

public:
	// data flag is initialized in constructor
	Sandbox();

	// set the program file stream
	void setProg(char *file);
	// set the data file stream
	void setData(char *file);
	// execute the sandbox
	void exec();
private:

	// sandbox memory
	int mem[MEMSIZE];
	// memory count
	int memUsed;
	// read buffer
	char buf[BUFSIZE];
	// whether program has data input
	bool hasData;
	// file stream for program and data
	ifstream fp, fd;
	// current instruction looked at
	Inst *curInst;
	// free memory pointer
	int *freeMem;
	// number of variables
	int numVar;
	// const variable, used for constant involved instructions
	Var *pvarConst;
	int *pConst;

	// instruction map
	map<string, InstType> imap;
	// variable map
	map<string, Var *> vmap;
	// jump point map
	map<string, Inst *> jmap;
	// instruction list
	vector<Inst *> ivec;
	// variable list
	vector<Var *> vvec;

	// initialize
	void init();
	// run the program
	void run(){}
	// parse the program
	void parse();
	// clean up when sandbox terminates
	void cleanUp();

	// initialize the instruction map
	void initMap();
	// abort with error
	void errAbort(ErrorType e);
	// print the instruction along with a line number
	string Sandbox::printLineInst(Inst *pinst);
	// print the instruction in a readable format
	string printInst(Inst *pinst);
	// print the line number of an instruction
	string printLine(Inst *pinst);

	// parse an instruction name
	InstType parseInst(string name);
	// alloc a variable
	string allocVar(string name);
	// dealloc a variable
	string deallocVar(string name);
	// parse an integer
	int parseInteger(string str);
	// find a variable pointer
	Var *getVar(string name);
	
	// check if the string is an integer
	bool checkInteger(string str);
	// check if the string references an array
	bool checkArray(string str, string &arrayname, int &offset);

	// arithmetic
	void setv(Var *pvar1, Var *pvar2);
	void add(Var *pvar1, Var *pvar2);
	void sub(Var *pvar1, Var *pvar2);
	void mul(Var *pvar1, Var *pvar2);
	void div(Var *pvar1, Var *pvar2);
	void inc(Var *pvar);
	void dec(Var *pvar);

	// print a variable onto the screen
	void printv(Var *pvar);

	// parse a comment
	void parseComment(string str);

	// check valid variable name
	void checkValidName(string name);
	// check valid character
	void checkValidChar(char c);

	// read an integer from data
	int readData();
};

#endif