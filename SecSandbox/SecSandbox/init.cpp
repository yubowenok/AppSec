#include "sandbox.h"

// sandbox initialization (after arguments have been checked)
void Sandbox::init(){
	curInst = NULL;
	memUsed = 0;
	numVar = 0;
	freeMem = mem;
	initMap();
	pConst = new int;
	pvarConst = new Var();
	pvarConst->setP(pConst);
}

// initialize the instruction map
void Sandbox::initMap(){
	imap.insert(make_pair("ALLOC", Inst_ALLOC));
	imap.insert(make_pair("DEALLOC", Inst_DEALLOC));
	imap.insert(make_pair("SET", Inst_SET));
	imap.insert(make_pair("ADD", Inst_ADD));
	imap.insert(make_pair("SUB", Inst_SUB));
	imap.insert(make_pair("MUL", Inst_MUL));
	imap.insert(make_pair("DIV", Inst_DIV));
	imap.insert(make_pair("INC", Inst_INC));
	imap.insert(make_pair("DEC", Inst_DEC));
	imap.insert(make_pair("JPT", Inst_JPT));
	imap.insert(make_pair("JMP", Inst_JMP));
	imap.insert(make_pair("JE", Inst_JE));
	imap.insert(make_pair("JL", Inst_JL));
	imap.insert(make_pair("JLE", Inst_JLE));
	imap.insert(make_pair("JG", Inst_JG));
	imap.insert(make_pair("JGE", Inst_JGE));
	imap.insert(make_pair("PRT", Inst_PRT));
	imap.insert(make_pair("RD", Inst_RD));
}