#include "sandbox.h"

void Sandbox::jumppoint(string name, int linenum){
	checkValidName(name);
	map<string, int>::iterator rj;
	if((rj=jmap.find(name))!=jmap.end()){
		// already exists a jump point of the given name
		errAbort(Error_JptDuplicated);
	}
	jmap.insert(make_pair(name, linenum)); 
}

int Sandbox::jump(string name){
	map<string, int>::iterator rj;
	if((rj=jmap.find(name))==jmap.end()){
		errAbort(Error_JptNotFound);
	}
	return (*rj).second;
}

int Sandbox::jumpe(string name, Var* pvar1, Var* pvar2){
	map<string, int>::iterator rj;
	if((rj=jmap.find(name))==jmap.end()){
		errAbort(Error_JptNotFound);
	}
	int *p1 = pvar1->p();
	int *p2 = pvar2->p();
	if(pvar1->len()>1) p1 = pvar1->arrayp();
	if(pvar2->len()>1) p2 = pvar2->arrayp();
	if(*p1 == *p2) return (*rj).second;
	return 0;
}

int Sandbox::jumpg(string name, Var* pvar1, Var* pvar2){
	map<string, int>::iterator rj;
	if((rj=jmap.find(name))==jmap.end()){
		errAbort(Error_JptNotFound);
	}
	int *p1 = pvar1->p();
	int *p2 = pvar2->p();
	if(pvar1->len()>1) p1 = pvar1->arrayp();
	if(pvar2->len()>1) p2 = pvar2->arrayp();
	if(*p1 > *p2) return (*rj).second;
	return 0;
}

int Sandbox::jumpge(string name, Var* pvar1, Var* pvar2){
	map<string, int>::iterator rj;
	if((rj=jmap.find(name))==jmap.end()){
		errAbort(Error_JptNotFound);
	}
	int *p1 = pvar1->p();
	int *p2 = pvar2->p();
	if(pvar1->len()>1) p1 = pvar1->arrayp();
	if(pvar2->len()>1) p2 = pvar2->arrayp();
	if(*p1 >= *p2) return (*rj).second;
	return 0;
}

int Sandbox::jumpl(string name, Var* pvar1, Var* pvar2){
	map<string, int>::iterator rj;
	if((rj=jmap.find(name))==jmap.end()){
		errAbort(Error_JptNotFound);
	}
	int *p1 = pvar1->p();
	int *p2 = pvar2->p();
	if(pvar1->len()>1) p1 = pvar1->arrayp();
	if(pvar2->len()>1) p2 = pvar2->arrayp();
	if(*p1 < *p2) return (*rj).second;
	return 0;
}

int Sandbox::jumple(string name, Var* pvar1, Var* pvar2){
	map<string, int>::iterator rj;
	if((rj=jmap.find(name))==jmap.end()){
		errAbort(Error_JptNotFound);
	}
	int *p1 = pvar1->p();
	int *p2 = pvar2->p();
	if(pvar1->len()>1) p1 = pvar1->arrayp();
	if(pvar2->len()>1) p2 = pvar2->arrayp();
	if(*p1 <= *p2) return (*rj).second;
	return 0;
}
