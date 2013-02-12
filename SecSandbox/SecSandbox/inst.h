#ifndef INST_H
#define INST_H

#include <string>
#include "var.h"

using namespace std;

// instruction types 
enum InstType{
	Inst_ALLOC = 0,
	Inst_DEALLOC,
	Inst_SET,
	Inst_ADD, Inst_SUB, Inst_MUL, Inst_DIV,
	Inst_INC, Inst_DEC,
	Inst_JPT,
	Inst_JMP, Inst_JE, Inst_JL, Inst_JLE, Inst_JG, Inst_JGE,
	Inst_PRT,
	Inst_RD,
	Inst_CMT	// comment
};

// instruction class
class Inst{
public:
	Inst(){}
	Inst(int line) { _line = line; }
	string var1(){ return _var1; }
	string var2(){ return _var2; }
	void setVar1(string var){ _var1 = var; }
	void setVar2(string var){ _var2 = var; }
	int line(){ return _line; }
	int setLine(int line){ _line = line; } 
	string text(){ return _text; }
	void setText(string text){ _text = text; }
	InstType type(){ return _type; }
	void setType(InstType type){ _type = type; }
private:
	int _line;
	InstType _type;
	string _var1, _var2;
	string _text;
};

#endif