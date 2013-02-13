#include "sandbox.h"

void Sandbox::jumppoint(string name, int linenum){
	checkValidName(name);
	map<string, int>::iterator rj;
	if((rj=jmap.find(name))!=jmap.end()){
		// already exists a jump point of the given name
		// check if it points to the same line, if not, we have duplicated jump points with the same name
		if((*rj).second!=linenum)
			errAbort(Error_JptDuplicated);
	}
	jmap.insert(make_pair(name, linenum)); 
}

int Sandbox::universal_jump(string name, int linenum, JumpType type, Var* pvar1, Var* pvar2){
	int retline;
	bool retlineSet = false;
	map<string, int>::iterator rj;

	if(checkInteger(name)){
		int line = parseInteger(name);
		retline = line + linenum;
		retlineSet = true;
	}else{
		if((rj=jmap.find(name))==jmap.end()){
			errAbort(Error_JptNotFound);
		}
	}

	if(type==Jump_DIRECT){
		if(retlineSet) return retline;
		return (*rj).second;	// direct jump
	}

	int *p1 = pvar1->p();
	int *p2 = pvar2->p();
	if(pvar1->len()>1) p1 = pvar1->arrayp();
	if(pvar2->len()>1) p2 = pvar2->arrayp();

	// conditional jump
	bool jumpSuccess = false;
	switch(type){
	case Jump_E:
		if(*p1 == *p2) jumpSuccess = true;
		break;
	case Jump_NE:
		if(*p1 != *p2) jumpSuccess = true;
		break;
	case Jump_L:
		if(*p1 < *p2) jumpSuccess = true;
		break;
	case Jump_LE:
		if(*p1 <= *p2) jumpSuccess = true;
		break;
	case Jump_G:
		if(*p1 > *p2) jumpSuccess = true;
		break;
	case Jump_GE:
		if(*p1 >= *p2) jumpSuccess = true;
		break;
	}
	if(jumpSuccess){
		if(retlineSet) return retline;
		return (*rj).second;
	}
	return JUMPNCON;
}
