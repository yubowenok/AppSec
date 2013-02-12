#include "sandbox.h"
#include <sstream>

void Sandbox::parse(){
	Inst *pinst;
	int lineNum = 1;
	int maxLineNum = 0; // maximum line number that has been ever reached
	string nameinst, namevar1, namevar2;
	string var1, var2;
	Var *pvar1, *pvar2;

	while(1){
		maxLineNum = lineNum>maxLineNum? lineNum:maxLineNum;
		if(maxLineNum>=MAXLINE) errAbort(Error_LineLimitExceeded);	// exceed maximum number of lines allowed

		pinst = new Inst(lineNum);
		ivec.push_back(pinst);

		// set the current instruction to enable error logging
		curInst = pinst;

		stringstream ss(stringstream::in | stringstream::out);
		if(lineNum==maxLineNum){
			// read new line from program file

			// read a program line into the buffer
			fp.getline(buf, BUFSIZE);
			if(fp.fail()){
				errAbort(Error_LineTooLong);
			}

			// scan from left to right to identify the instruction
			pinst->setText(string(buf));
			ss << buf;  // dump line into a string stream
		}else{
			// jump to a previous line
			pinst = ivec[lineNum];
			ss << pinst->text();
		}

		// parse instruction type
		ss >> nameinst;
		pinst->setType(parseInst(nameinst));

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
		case Inst_JMP:
			break;
		case Inst_JPT:
			break;
		case Inst_JE:
			break;
		case Inst_JLE:
			break;
		case Inst_JL:
			break;
		case Inst_JGE:
			break;
		case Inst_JG:
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
		}

		ss.getline(buf, BUFSIZE);	// read the rest of the line
		if(!ss.eof()){
			// not the end of line, then deal with the tailing text
			if(ss.fail()) errAbort(Error_LineTooLong);
			string cmt(buf);
			parseComment(cmt);
		}


		// encounters the end of the program, finish parsing
		if(fp.eof()){
			break;
		}

		// increase line number
		lineNum ++;
	}
}

// get a variable pointer
Var *Sandbox::getVar(string name){

	if(checkInteger(name)){
		int val = parseInteger(name);
		*pConst = val;
		return pvarConst;
	}
	string arrayname;
	int offset;
	bool isArray = false;
	if(isArray = checkArray(name, arrayname, offset)){
		name = arrayname;
	}
	checkValidName(name);	// first check if the name is valid

	map<string, Var*>::iterator rv;
	if((rv=vmap.find(name))==vmap.end()){
		errAbort(Error_VarNotFound);
	}
	if(isArray){
		Var *v = (*rv).second;
		if(offset>=v->len()) errAbort(Error_IndexOutOfRange);	// array index out of range
		((*rv).second)->setArrayindex(offset);
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

// deallocate a variable
string Sandbox::deallocVar(string name){
	map<string, Var *>::iterator ri;
	if((ri=vmap.find(name))==vmap.end()){
		// variable not exist
		errAbort(Error_DeallocNull);
	}
	string varname = name;
	checkValidName(varname);
	Var *pvar = (*ri).second;
	vmap.erase(ri);

	vector<Var *>::iterator rv, rvf;
	rv = vvec.begin() + pvar->index();
	// decrease the indices of the variables in the vector that follow the deallocated variable 
	for(rvf=rv+1; rvf!=vvec.end(); rvf++){
		int index = (*rvf)->index();
		(*rvf)->setIndex(index-1);
	}

	// free the memory
	memUsed = (*rv)->len();	
	freeMem -= (*rv)->len();
	numVar --;
	delete *rv;
	vvec.erase(rv);	// removing an element in a vector is slow, but it's ok if we don't have many variables

	return varname;
}

// allocate a variable
string Sandbox::allocVar(string name){
	map<string, Var *>::iterator ri;
	if((ri=vmap.find(name))!=vmap.end()){
		// variable already existing
		errAbort(Error_Realloc);
	}
	string varname;

	int arraylen;
	string arrayname;
	bool isArray = false;
	if(isArray=checkArray(name, arrayname, arraylen)){
		varname = arrayname;
	}
	checkValidName(varname);	// check if the name is valid

	Var *newvar;
	if(!isArray){
		// single variable allocation
		if(memUsed+1>MEMSIZE) errAbort(Error_MemLimitExceeded);	// not enough memory for even one variable

		newvar = new Var(numVar);
		newvar->setLen(1);
		newvar->setP(freeMem);
		freeMem++;

		vvec.push_back(newvar);
		vmap.insert(make_pair(varname, newvar));
	}else{
		// array allocation
		if(arraylen<=0) errAbort(Error_InvalidArray);	// invalid array length
		if(memUsed+arraylen>MEMSIZE) errAbort(Error_MemLimitExceeded);	// not enough memory for such large array

		newvar = new Var(numVar);
		newvar->setLen(arraylen);
		newvar->setP(freeMem);
		freeMem+=arraylen;

		vvec.push_back(newvar);
		vmap.insert(make_pair(varname, newvar));
	}
	numVar++;
	return varname;
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
bool Sandbox::checkArray(string str, string &arrayname, int &offset){
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
			if(!isArray) isArray = true;
			else errAbort(Error_InvalidArray);  // incorrect '[' follows ']'
			int j;
			for(j=i+1; j<l; j++){
				if(name[j]==']'){
					string intstr = name.substr(i+1, j-i-1);
					if(!checkInteger(intstr)) errAbort(Error_InvalidNum);
					offset = parseInteger(intstr);
					i = j+1;
					break;
				}
			}
		}
		if(i<l) checkValidChar(c);
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
	int ret = 0;
	bool neg = false;
	int ie=0;
	if(str[0]=='-'){
		neg = true;
		ie = 1;
	}
	int b = 1;
	for(int i=str.length()-1; i>=ie; i--){
		char c = str[i];
		int v = (int)(c-'0');
		ret += v*b;
		if(ret>MAXINT) errAbort(Error_UnboundedNum); 
		b*=10;
	}
	if(neg) ret = -ret;
	return ret;
}