#include "sandbox.h"
#include <sstream>

void Sandbox::parse(){
	Inst *pinst;
	int lineNum = 1;
	int jmplineNum = 0;
	int maxLineNum = 0; // maximum line number that has been ever reached
	string nameinst, namejpt, namevar1, namevar2;
	string var1, var2;
	Var *pvar1, *pvar2;

	while(1){
		maxLineNum = lineNum>maxLineNum? lineNum:maxLineNum;
		if(maxLineNum>=MAXLINE) errAbort(Error_LineLimitExceeded);	// exceed maximum number of lines allowed

		stringstream ss(stringstream::in | stringstream::out);
		if(lineNum>(int)ivec.size()-1){	
			// check ivec.size(): attempt to read a line that has not been processed yet
			// read new line from program file

			// read a program line into the buffer
			fp.getline(buf, BUFSIZE);
			if(fp.fail()){
				if(!fp.eof()) errAbort(Error_LineTooLong);	// if EOF not reached, we have a read-error line
				else{
					if(jmplineNum!=JUMPNCON && jmplineNum>lineNum){
						errAbort(Error_InvalidJump);	// no more lines that fit the jump
					}
					break;	// else we encounter the end of the program normally, there are no more instructions and we finish parsing
				}
			}

			pinst = new Inst(lineNum);
			ivec.push_back(pinst);

			// scan from left to right to identify the instruction
			pinst->setText(string(buf));
			ss << buf;  // dump line into a string stream
		}else{
			// jump to a previous line
			pinst = ivec[lineNum];
			ss << pinst->text();
		}

		if(jmplineNum!=JUMPNCON && jmplineNum>lineNum){
			// jump to a future line
			lineNum ++;
			continue;
		}

		// clear last instruction
		nameinst = "";

		// reset to no jump
		jmplineNum = JUMPNCON;	

		// set the current instruction to enable error logging
		curInst = pinst;

		// parse instruction type
		ss >> nameinst;
		removePrefixSpace(nameinst);
		if(nameinst=="" || (nameinst.length()>=2 && nameinst.substr(0,2)=="//") ) pinst->setType(Inst_CMT);
		else{
			for(unsigned int i=0; i<nameinst.length(); i++){
				// change to upper case for instructions
				char c = nameinst[i];
				if(c>='a' && c<='z'){
					nameinst[i] = toupper(c);
				}
			}
			pinst->setType(parseInst(nameinst));
		}

		if(debugMode){
			cout << lineNum << " " << pinst->text() << endl;
		}

		// parse variables
		switch(pinst->type()){
		case Inst_ALLOC:
			ss >> namevar1;
			var1 = allocVar(namevar1);
			pinst->setVar1(var1);
			break;
		case Inst_DEALLOC:
			ss >> namevar1;
			var1 = deallocVar(namevar1);
			pinst->setVar1(var1);
			break;
		case Inst_SET:
			ss >> namevar1 >> namevar2;
			pvar1 = getVar(namevar1);
			pvar2 = getVar(namevar2);
			pinst->setVar1(namevar1);
			pinst->setVar2(namevar2);
			setv(pvar1, pvar2);
			break;
		case Inst_ADD:
			ss >> namevar1 >> namevar2;
			pvar1 = getVar(namevar1);
			pvar2 = getVar(namevar2);
			pinst->setVar1(namevar1);
			pinst->setVar2(namevar2);
			add(pvar1, pvar2);
			break;
		case Inst_SUB:
			ss >> namevar1 >> namevar2;
			pvar1 = getVar(namevar1);
			pvar2 = getVar(namevar2);
			pinst->setVar1(namevar1);
			pinst->setVar2(namevar2);
			sub(pvar1, pvar2);
			break;
		case Inst_MUL:
			ss >> namevar1 >> namevar2;
			pvar1 = getVar(namevar1);
			pvar2 = getVar(namevar2);
			pinst->setVar1(namevar1);
			pinst->setVar2(namevar2);
			mul(pvar1, pvar2);
			break;
		case Inst_DIV:
			ss >> namevar1 >> namevar2;
			pvar1 = getVar(namevar1);
			pvar2 = getVar(namevar2);
			pinst->setVar1(namevar1);
			pinst->setVar2(namevar2);
			div(pvar1, pvar2);
			break;
		case Inst_INC:
			ss >> namevar1;
			pvar1 = getVar(namevar1);
			pinst->setVar1(namevar1);
			*pConst = 1;
			add(pvar1, pvarConst);
			break;
		case Inst_DEC:
			ss >> namevar1;
			pvar1 = getVar(namevar1);
			pinst->setVar1(namevar1);
			*pConst = 1;
			sub(pvar1, pvarConst);
			break;
		case Inst_JPT:
			ss >> namejpt;
			jumppoint(namejpt, lineNum);
			break;
		case Inst_JMP:
			ss >> namejpt;
			jmplineNum = jump(namejpt, lineNum);
			break;
		case Inst_JE:
			ss >> namejpt >> namevar1 >> namevar2;
			pvar1 = getVar(namevar1);
			pvar2 = getVar(namevar2);
			pinst->setVar1(namevar1);
			pinst->setVar2(namevar2);
			jmplineNum = jumpe(namejpt, lineNum, pvar1, pvar2);
			break;
		case Inst_JNE:
			ss >> namejpt >> namevar1 >> namevar2;
			pvar1 = getVar(namevar1);
			pvar2 = getVar(namevar2);
			pinst->setVar1(namevar1);
			pinst->setVar2(namevar2);
			jmplineNum = jumpne(namejpt, lineNum, pvar1, pvar2);
			break;
		case Inst_JLE:
			ss >> namejpt >> namevar1 >> namevar2;
			pvar1 = getVar(namevar1);
			pvar2 = getVar(namevar2);
			pinst->setVar1(namevar1);
			pinst->setVar2(namevar2);
			jmplineNum = jumple(namejpt, lineNum, pvar1, pvar2);
			break;
		case Inst_JL:
			ss >> namejpt >> namevar1 >> namevar2;
			pvar1 = getVar(namevar1);
			pvar2 = getVar(namevar2);
			pinst->setVar1(namevar1);
			pinst->setVar2(namevar2);
			jmplineNum = jumpl(namejpt, lineNum, pvar1, pvar2);
			break;
		case Inst_JGE:
			ss >> namejpt >> namevar1 >> namevar2;
			pvar1 = getVar(namevar1);
			pvar2 = getVar(namevar2);
			pinst->setVar1(namevar1);
			pinst->setVar2(namevar2);
			jmplineNum = jumpge(namejpt, lineNum, pvar1, pvar2);
			break;
		case Inst_JG:
			ss >> namejpt >> namevar1 >> namevar2;
			pvar1 = getVar(namevar1);
			pvar2 = getVar(namevar2);
			pinst->setVar1(namevar1);
			pinst->setVar2(namevar2);
			jmplineNum = jumpg(namejpt, lineNum, pvar1, pvar2);
			break;
		case Inst_PRT:
			ss >> namevar1;
			pvar1 = getVar(namevar1);
			pinst->setVar1(namevar1);
			printv(pvar1);
			break;
		case Inst_RD:
			ss >> namevar1;
			pvar1 = getVar(namevar1);
			pinst->setVar1(namevar1);
			*pConst = readData();
			setv(pvar1, pvarConst);
			break;
		case Inst_CMT:
			break;
		case Inst_EXIT:
			errAbort(Error_ProgramExit);
			break;
		}

		ss.getline(buf, BUFSIZE);	// read the rest of the line
		if(!ss.eof()){
			// not the end of line, then check if tailing text is too long
			if(ss.fail()) errAbort(Error_LineTooLong);
		}
		if(pinst->type()!=Inst_CMT){
			// deal with the tailing text
			string cmt(buf);
			parseComment(cmt);
		}

		if(jmplineNum!=JUMPNCON){
			// a jump instruction has taken effect
			if(jmplineNum<=0){
				errAbort(Error_InvalidJump);
			}

			if(maxLineNum<jmplineNum){
				// jump to a future line, set to the nearest future line
				lineNum = maxLineNum + 1;
			}else{
				// jump to a previous line, set to that previous line
				lineNum = jmplineNum;
			}
		}else{
			// JUMPNCON means jump condition not satisfied, just increase line number
			lineNum ++;
		}
	}
}

// get a variable pointer
Var *Sandbox::getVar(string name, int depth){
	removePrefixSpace(name);
	if(checkInteger(name)){
		int val = parseInteger(name);
		*pConst = val;
		return pvarConst;
	}
	string arrayname;
	int offset;
	bool isArray = false;
	if(isArray = checkArray(name, arrayname, offset, depth)){
		name = arrayname;
	}
	checkValidName(name);	// first check if the name is valid

	map<string, Var*>::iterator rv;
	if((rv=vmap.find(name))==vmap.end()){
		errAbort(Error_VarNotFound);
	}
	Var *v = (*rv).second;
	if(isArray){
		if(offset>=v->len()) errAbort(Error_IndexOutOfRange);	// array index out of range
		v->setArrayindex(offset);
	}else{
		if(v->len()>1) errAbort(Error_InvalidArray);
	}
	return (*rv).second;
}

// parse an instruction name
InstType Sandbox::parseInst(string name){
	map<string, InstType>::iterator rit;
	if((rit=imap.find(name))==imap.end()){
		errAbort(Error_InstNotFound);
	}
	return (*rit).second;
}

// check valid character
void Sandbox::checkValidChar(char c){
	if(! ((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_') ){
		errAbort(Error_InvalidChar);
	}
}
// check valid variable name
void Sandbox::checkValidName(string name){
	int l = name.length();
	if(l==0) errAbort(Error_InvalidName);
	for(int i=0; i<l; i++){
		char c = name[i];
		if(i==0){
			if(! ((c>='a'&&c<='z')||(c>='A'&&c<='Z')) ){
				// not begin with letter
				errAbort(Error_InvalidName);
			}
		}
		else{
			if(! ((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_') ){
				errAbort(Error_InvalidName);
			}
		}
	}
	if(name.length()>32) errAbort(Error_InvalidName);	// name too long
}

// parse comment
void Sandbox::parseComment(string str){
	int l = str.length();
	for(int i=0; i<l; i++){
		char c = str[i];
		if(c=='/'){
			if(i+1<l && str[i+1]=='/'){
				// find a comment
				break;
			}else{
				// wrong symbol
				errAbort(Error_InvalidChar);
			}
		}
		else if(c!=' '&&c!='\n'&&c!='\r'&&c!='\t'){
			// unknown symbol
			errAbort(Error_InvalidChar);
		}
	}
}

// check if a variable name reference an array
bool Sandbox::checkArray(string str, string &arrayname, int &offset, int depth){
	if(depth==MAXDEPTH) errAbort(Error_MaxRecursive);
	if(depth==0) 
		while(!vstk.empty()) vstk.pop();  // clear the var stack
	bool isArray = false;
	int i;
	int l = str.length();
	string name = str;
	for(i=0; i<l; i++){
		char c = name[i];
		if(isArray){
			errAbort(Error_InvalidArray);  // incorrect chars follow ']'
		}
		if(c=='['){
			arrayname = name.substr(0, i);  // get variable name
			if(name[l-1]!=']') errAbort(Error_InvalidArray);  // mismatch brackets
			if(!isArray) isArray = true;

			string intstr = name.substr(i+1, l-1-i-1);

			Var *pvar = getVar(intstr, depth+1);
			int *p = pvar->p();
			if(pvar->len()>1) p = pvar->arrayp();
			offset = *p;
		}
		if(isArray) break;
	}
	if(isArray) return true;
	return false;
}

// check if the string is an integer
bool Sandbox::checkInteger(string str){
	int i=0;
	if(str[0]=='-') i=1;
	int l = str.length();
	for(;i<l;i++){
		char c = str[i];
		if(! (c>='0'&&c<='9')) return false;
	}
	return true;
}

// parse an integer
int Sandbox::parseInteger(string str){
	long long ret = 0;
	bool neg = false;
	int ie=0;
	if(str[0]=='-'){
		neg = true;
		ie = 1;
	}
	int b = 1;
	if(str.substr(1).length()>10) errAbort(Error_UnboundedNum); 
	for(int i=str.length()-1; i>=ie; i--){
		char c = str[i];
		int v = (int)(c-'0');
		ret += v*b;
		if(ret>MAXINT) errAbort(Error_UnboundedNum); 
		b*=10;
	}
	if(neg) ret = -ret;
	return (int)ret;
}

// remove prefix spaces and tabs in a string
void Sandbox::removePrefixSpace(string &name){
	unsigned int i;
	for(i=0; i<name.length(); i++){
		if(name[i]!=' ' && name[i]!='\t') break;
	}
	name = name.substr(i);
}