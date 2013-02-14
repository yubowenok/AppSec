#ifndef SANDBOX_H
#define SANDBOX_H

#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <fstream>

#include "inst.h"
#include "error.h"

using namespace std;

#define MEMSIZE		16384
#define BUFSIZE		257
#define MAXINT		1000000000
#define MAXLINE		100000
#define MAXDEPTH	1000
#define JUMPNCON	200000

class Sandbox{

public:
	// data flag is initialized in constructor
	Sandbox();

	// set debug mode
	void setDebugMode();
	// set the program file stream
	void setProg(char *file);
	// set the data file stream
	void setData(char *file);
	// execute the sandbox
	void exec();
private:
	// debug mode
	bool debugMode;
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
	// variable stack used for recursive array reference
	stack<Var *> vstk;

	// instruction map
	map<string, InstType> imap;
	// variable map
	map<string, Var *> vmap;
	// jump point map to line number
	map<string, int> jmap;
	// instruction list
	vector<Inst *> ivec;
	// variable list
	vector<Var *> vvec;

	// initialize
	void init();
	// parse & run the program
	void parse();
	// clean up when sandbox terminates
	void cleanUp();

	// initialize the instruction map
	void initInstMap();
	// abort with error
	void errAbort(ErrorType e);
	// print the instruction along with a line number
	string printLineInst(Inst *pinst);
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
	Var *getVar(string name, int depth=0);
	
	// check if the string is an integer
	bool checkInteger(string str);
	// check if the string references an array
	bool checkArray(string str, string &arrayname, int &offset, int depth=0);

	// arithmetic
	void setv(Var *pvar1, Var *pvar2);
	void add(Var *pvar1, Var *pvar2);
	void sub(Var *pvar1, Var *pvar2);
	void mul(Var *pvar1, Var *pvar2);
	void div(Var *pvar1, Var *pvar2);
	void inc(Var *pvar);
	void dec(Var *pvar);

	// jump
	enum JumpType{
		Jump_DIRECT = 0,
		Jump_E, Jump_NE, Jump_G, Jump_GE, Jump_L, Jump_LE
	};
	// set jump point
	void jumppoint(string name, int linenum);	
	// conditional jump returns the line number being jumped to, 0 if condition not satisfied
	int universal_jump(string name, int linenum, JumpType type, Var* pvar1=NULL, Var* pvar2=NULL);
	int jump(string name, int linenum){ return universal_jump(name, linenum, Jump_DIRECT); }
	int jumpe(string name, int linenum, Var* pvar1, Var* pvar2){ return universal_jump(name, linenum, Jump_E, pvar1, pvar2); }
	int jumpne(string name, int linenum, Var* pvar1, Var* pvar2){ return universal_jump(name, linenum, Jump_NE, pvar1, pvar2); }
	int jumpg(string name, int linenum, Var* pvar1, Var* pvar2){ return universal_jump(name, linenum, Jump_G, pvar1, pvar2); }
	int jumpge(string name, int linenum, Var* pvar1, Var* pvar2){ return universal_jump(name, linenum, Jump_GE, pvar1, pvar2); }
	int jumpl(string name, int linenum, Var* pvar1, Var* pvar2){ return universal_jump(name, linenum, Jump_L, pvar1, pvar2); }
	int jumple(string name, int linenum, Var* pvar1, Var* pvar2){ return universal_jump(name, linenum, Jump_LE, pvar1, pvar2); }

	// print a variable onto the screen
	void printv(Var *pvar);

	// parse a comment
	void parseComment(string str);

	// check valid variable name
	void checkValidName(string name);
	// check valid character
	void checkValidChar(char c);

	// remove the prefix space and tabs
	void removePrefixSpace(string &name);

	// read an integer from data
	int readData();
};

#endif